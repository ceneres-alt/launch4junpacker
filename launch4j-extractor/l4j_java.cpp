#include "l4j_java.h"
#include "l4j_file_output.h"

l4j_java::l4j_java(l4j_file_handler* file_handler_obj, std::wstring Path) :
	m_file_handler_obj(file_handler_obj), m_output_path(Path)
{
}

bool l4j_java::GetL4JJavaFile()
{
	BYTE* FileHandlerBuffer = m_file_handler_obj->GetBuffer();
	BYTE* JavaFile = GetJavaFileOffset(FileHandlerBuffer);

	if (!JavaFile)
	{
		return false;
	}

	// Extract the Java file here

	DWORD Launch4jExecutableSize = (DWORD)((DWORD_PTR)JavaFile - (DWORD_PTR)FileHandlerBuffer);
	DWORD JavaFileSize = m_file_handler_obj->GetSize() - Launch4jExecutableSize;

	// Allocate space for the java file and copy it in the buffer

	BYTE* WritePtr = (BYTE*)VirtualAlloc(0, JavaFileSize, MEM_COMMIT, PAGE_READWRITE);

	if (!WritePtr)
	{
		printf("(Java file allocation) VirtualAlloc failed with error code %d\n", GetLastError());
		return false;
	}

	memcpy(WritePtr, JavaFile, JavaFileSize);

	// Write the file on disk

	if (!l4j_file_output::OutputFile(m_output_path, WritePtr, JavaFileSize))
	{
		return false;
	}
	else
	{ 
		return true;
	}
}

BYTE* l4j_java::GetJavaFileOffset(BYTE* Buffer)
{
	// Get DOS and NT header from the PE
	PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)Buffer;

	if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		printf("The opened file is not a valid PE file.\n");
		return nullptr;
	}

	PIMAGE_NT_HEADERS NtHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)DosHeader + DosHeader->e_lfanew);

	if (NtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		printf("The opened file is not a valid PE file (Invalid NT signature).\n");
		return nullptr;
	}

	// Get the first section of the PE file
	PIMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(NtHeader);

	// Go section by section until the .rsrc section is found.

	BYTE* JavaFileOffset = 0;

	// fixme: The .rsrc may not be the last section in the PE.

	for (int i = 0; i < NtHeader->FileHeader.NumberOfSections; i++)
	{
		if (!strcmp((const char*)Section[i].Name, ".rsrc"))
		{
			DWORD SectAddress = (DWORD_PTR)DosHeader + Section[i].PointerToRawData;
			printf(".rsrc section found at 0x%08x: RawAddr: 0x%08x, RawSize: %d\n", 
				SectAddress, Section[i].PointerToRawData, Section[i].SizeOfRawData);

			JavaFileOffset = (BYTE*)(SectAddress + Section[i].SizeOfRawData);

			printf("Java file should be at address %p.\n", JavaFileOffset);
		}
	}

	if (!JavaFileOffset)
	{
		printf("The .rsrc section was not found.\n");
		return nullptr;
	}

	// Check if JavaFileOffset points to a Java file by checking if the first word is equal to 'PK'

	if (*(WORD*)(JavaFileOffset) != 0x4b50)
	{
		WORD SignatureFound = *(WORD*)(JavaFileOffset);

		printf("Could not find a valid .java file at %p: are you sure this is a Launch4j executable? (Found sig = 0x%04x).\n",
			JavaFileOffset, SignatureFound);

		return nullptr;
	}

	printf("Java file found at %p.\n", JavaFileOffset);

	return JavaFileOffset;
}
