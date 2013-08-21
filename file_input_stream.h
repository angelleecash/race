#ifndef __FILE_INPUT_STREAM__
#define __FILE_INPUT_STREAM__

#include <stdio.h>
#include <string>
#include "data_type.h"

using namespace std;

namespace race
{

	class FileInputStream
	{
	public:
		FileInputStream(string path);

		int Read(Byte* buffer, int n);

		template<class T>
		bool ReadPrimitive(T& value)
		{
			Uint32 bytes = sizeof(T);

			int bytesRead = Read(reinterpret_cast<Byte*>(&value), bytes);

			return bytesRead == bytes;
		}

		template<class T>
		bool ReadPrimitives(T* value, int n)
		{
			bool success = true;

			for(int i=0; i<n; i++)
			{
				success &&= Read(*T);

				if(!success)
				{
					break;
				}
				value++;
			}

			return success;
		}

		int GetPosition();
		int SetPosition(int position);

		int BytesLeft()
		{
			return _size - _position;
		}

		int GetSize();

		virtual ~FileInputStream();

		FILE* file;
		Byte* data;

	private:
		Int32 _size;
		Int32 _position;
	};

}

#endif