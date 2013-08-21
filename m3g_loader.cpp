#include "m3g_loader.h"
#include "file_input_stream.h"
#include "my_assert.h"

namespace race
{

	const Byte M3gLoader::FILE_IDENTIFIER[] = {0xAB, 0x4A, 0x53, 0x52, 0x31, 0x38, 0x34, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};

	M3gLoader::M3gLoader(string path):path(path)
	{
		FileInputStream fis(path);
		int ok = 1;
		for(int i=0; i < 12; i++)
		{
			if(fis.data[i] != FILE_IDENTIFIER[i])
			{
				ok = 0;
				printf("File Identifier not match at %d read %c supposed to be %c \n", i, fis.data[i], FILE_IDENTIFIER[i]);
			}
		}

		ASSERT(ok, "file identifier not match");

		printf("");
	}

	string& M3gLoader::GetPath()
	{
		return path;
	}
}