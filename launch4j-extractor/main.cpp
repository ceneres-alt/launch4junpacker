#include "l4j_file_output.h"
#include "l4j_java.h"

int wmain(int argc, wchar_t *argv[ ])
{
	if (argc < 3)
	{
		printf("Usage: launch4j-extractor.exe [FilePath] [OutputPath]\nMade with <3 by oct0ling");
		return 0;
	}

	std::wstring FilePath = argv[1];
	std::wstring OutputPath = argv[2];

	l4j_file_handler* file_handler = new l4j_file_handler(FilePath);
	
	if (!file_handler->Init())
		return 0;

	l4j_java* launch4jparser = new l4j_java(file_handler, OutputPath);

	if (!launch4jparser->GetL4JJavaFile())
		return 0;

	return 1;
}