#pragma once
#include "l4j_file_handler.h"

class l4j_java
{
public:
	l4j_java(l4j_file_handler* file_handler_obj, std::wstring Path);
	bool GetL4JJavaFile();

private:
	BYTE* GetJavaFileOffset(BYTE* Buffer);

	std::wstring m_output_path;
	l4j_file_handler* m_file_handler_obj;
};

