#include "l4j_file_output.h"

void Cleanup(HANDLE FileHandle, BYTE* Buffer)
{
	if (FileHandle != INVALID_HANDLE_VALUE)
		CloseHandle(FileHandle);

	VirtualFree(Buffer, 0, MEM_RELEASE);
}

bool l4j_file_output::OutputFile(std::wstring Path, BYTE* Buffer, DWORD Size)
{
	HANDLE OutputHandle = CreateFile(Path.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

	if (OutputHandle == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed with error code %d\n", GetLastError());

		Cleanup(OutputHandle, Buffer);
		return false;
	}

	DWORD BytesWritten = 0;

	if (!WriteFile(OutputHandle, Buffer, Size, &BytesWritten, 0))
	{
		printf("WriteFile failed with error code %d\n", GetLastError());

		Cleanup(OutputHandle, Buffer);
		return false;
	}

	wprintf(L"%ws successfully written! %d bytes.", Path.c_str(), BytesWritten);
	return true;
}
