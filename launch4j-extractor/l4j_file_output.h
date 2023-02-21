#pragma once
#include <Windows.h>
#include <string>

static class l4j_file_output
{
public:
	static bool OutputFile(std::wstring Path, BYTE* Buffer, DWORD Size);
};

