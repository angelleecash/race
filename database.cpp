#include "file_input_stream.h"
#include "database.h"
#include "util.h"
#include "my_assert.h"


Database::Database(string path)
{
	FileInputStream stream(path);

	int c = stream.GetPosition();
	char identifier[4];

	stream.ReadPrimitive(identifier);

	ASSERT(!strncmp(identifier, "SBIN", 4), "Only SBIN format supported right now");

	Uint32 version;
	stream.ReadPrimitive(version);

	P("%s version %d\n", path.c_str(), version);

	while(stream.GetPosition() != stream.GetSize())
	{
		bool success = ReadSection(stream);
		ASSERT(success, "Error reading section");
	}
}

bool Database::ReadSection(FileInputStream& stream)
{
	char identifier[4];

	int c = sizeof(identifier);

	bool ok;
	
	ok = stream.ReadPrimitive(identifier);
	ASSERT(ok, "Unable to read identifier for section");

	P("loading section %c%c%c%c \n", identifier[0], identifier[1], identifier[2], identifier[3]);

	int size;
	ok = stream.ReadPrimitive(size);

	ASSERT(ok, "Unable to read section size");

	ASSERT(stream.BytesLeft() >= size, "No enough data for section");

	Uint32 checksum;
	ok = stream.ReadPrimitive(checksum);
	ASSERT(ok, "Unable to read checksum");

	int align = Align(size, 4);
	int current = stream.GetPosition();
	stream.SetPosition(align + current);

	//P("Align from %d with %d to %d\n", current, align, stream.GetPosition());

	return true;
}

Database::~Database()
{

}