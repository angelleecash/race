#ifndef __M3G_LOADER_H__
#define __M3G_LOADER_H__

#include <string>
#include <iostream>
#include "data_type.h"

using namespace std;

namespace race
{

	class M3gLoader
	{
	public:
		M3gLoader(string path);
		int Load();
		int LoadSection();
		void LoadFileIdentifier();

		string& GetPath();
		static const Byte FILE_IDENTIFIER[] ;//= {0xAB, 0x4A, 0x53, 0x52, 0x31, 0x38, 0x34, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};
		//static const Byte FILE_IDENTIFIER[12] = {'«', 'J', 'S', 'R', '1', '8', '4', '»', '\r', '\n', '\x1A', '\n'};
	private:
		string path;
	};
}
#endif