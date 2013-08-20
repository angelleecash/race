#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string>
#include "file_input_stream.h"

using std::string;

class FileIdentifier
{
	char id[4];
};

class Database
{
public:
	Database(string path);

	bool ReadSection(FileInputStream& stream);

	~Database();
private:
	string path;
};

#endif