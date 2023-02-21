#include "l4j_file_handler.h"

#include <stdio.h>

l4j_file_handler::l4j_file_handler(std::wstring path) :
	m_file_path(path), m_exe_handle(INVALID_HANDLE_VALUE),
	m_buffer(nullptr), m_file_size(0)
{

}

l4j_file_handler::~l4j_file_handler()
{
	CloseHandle(m_exe_handle);
	VirtualFree(m_buffer, 0, MEM_RELEASE);
}

bool l4j_file_handler::Init()
{
	// Open the Launch4j executable

	printf("Opening L4J executable...\n");
	m_exe_handle = CreateFile(m_file_path.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (m_exe_handle == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed with error code %d\n", GetLastError());
		return false;
	}

	printf("Handle to file opened: %p\n", m_exe_handle);

	// Get the L4J executable file size

	printf("Getting L4J file size...\n");

	m_file_size = GetFileSize(m_exe_handle, 0);

	printf("File size: %d bytes\n", m_file_size);

	// Allocate a buffer of memory to read the file

	m_buffer = (BYTE*)VirtualAlloc(0, m_file_size, MEM_COMMIT, PAGE_READWRITE);

	if (!m_buffer)
	{
		printf("VirtualAlloc failed with error code %d\n", GetLastError());
		return false;
	}

	// Read the file inside the buffer

	DWORD BytesRead = 0;
	if (!ReadFile(m_exe_handle, m_buffer, m_file_size, &BytesRead, 0))
	{
		printf("ReadFile failed with error code %d\n", GetLastError());
		return false;
	}

	// If everything was successful, print and return true
	printf("Read %d bytes in buffer at memory address %p\n", BytesRead, m_buffer);
	return true;
}
