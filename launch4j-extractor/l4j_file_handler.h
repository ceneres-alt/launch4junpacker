#pragma once
#include <Windows.h>
#include <string>

class l4j_file_handler
{
public:
	l4j_file_handler(std::wstring path);
	~l4j_file_handler();

	inline BYTE* GetBuffer() { return m_buffer; }
	inline DWORD GetSize()	 { return m_file_size; }

	bool Init();

private:
	std::wstring m_file_path;
	HANDLE		 m_exe_handle;
	BYTE*		 m_buffer;
	DWORD		 m_file_size;
};

