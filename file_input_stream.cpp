#include "file_input_stream.h"
#include <io.h>
#include <memory>
#include "data_type.h"
#include "my_assert.h"
#include "util.h"


FileInputStream::FileInputStream(string path):_position(0)
{
	file = NULL;	
	const char* p = path.c_str();
	printf("trying to load %s \n", p);
	fopen_s(&file, p, "rb");
	if(file == NULL)
	{
		printf("fail to load %s \n", p);
	}

	_size = _filelength(_fileno(file));
	
	data = (Byte*)malloc(sizeof(Byte) * _size);
	int bytesRead=0;
	while(bytesRead < _size)
	{
		int n = fread(data+bytesRead, sizeof(char), _size-bytesRead, file);
		bytesRead += n;
	}
	
	printf("%s read %d bytes.\n", path, bytesRead);
}

int FileInputStream::Read(Byte* buffer, int n)
{
	ASSERT(n > 0, "n should be positive");
	ASSERT(_position >= 0, "position should be positive");

	Byte* current = data + _position;

	errno_t error = memcpy_s(buffer, n, current, n);

	_position += n;
	//P("reading %d bytes, after position %d\n", n, _position);
	ASSERT(_position > 0, "position should be positive");

	if(error)
	{
		return -1;
	}
	else
	{
		return n;
	}
	
}

int FileInputStream::GetPosition()
{
	return _position;
}
int FileInputStream::SetPosition(int position)
{
	//this is bad
	//ASSERT(position < _size, "position out of bounds.");

	_position = min(position, _size);
	return 0;
}

int FileInputStream::GetSize()
{
	return _size;
}


FileInputStream::~FileInputStream()
{
	fclose(file);
	file = NULL;

	delete[] data;
	data = NULL;
}