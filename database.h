#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string>
#include <map>
#include <vector>

#include "file_input_stream.h"
#include "my_object.h"
#include "serialization_type.h"
#include "character_data_header.h"

using std::string;

namespace race
{

	class FileIdentifier
	{
		char id[4];
	};

	class Database
	{
	public:
		Database(string path);

		bool ReadSection(FileInputStream& stream);
		MyObject GetMyObject(const char* name);

		MyObject Database::ReadMyObject(const char* name, int objectIndex, int structIndex=-1, int dataOffset=0);

		const char* GetData(const char* name);

		int GetCData(const char* name);

		MyObject Read(const char* data, const char* name, const FieldType& fieldType);

		FieldType Database::GetFieldType(const char* name, int objectIndex, int structIndex, int dataOffset);

		ObjectHeaderDefinition* GetObjectHeaderDefinition(unsigned int objectIndex);

		char* GetStructuredObjectField(ObjectHeaderDefinition* objectHeaderDefinition, int strucIndex, int dataOffset, int nameIndex, FieldDefinition** fieldDefinitionResult);

		char* Database::GetObjectData(ObjectHeaderDefinition* objectHeaderDefinition);

		int GetObjectDefinitionSize(ObjectHeaderDefinition* objectHeaderDefinition);

		StructDefinition* GetStructDefinition(int structIndex);

		FieldDefinition* GetFieldDefinitionByName(StructDefinition* structDefinition, int nameIndex);

		char* GetUnstructuredObjectField(ObjectHeaderDefinition* objectHeaderDefinition, int nameIndex, FieldDefinition** fieldDefinitionResult);

		int GetFieldSize(FieldDefinition& fieldDefinition);
		int GetFieldSize(FieldType& fieldType);

		int GetStructSize(StructDefinition* structDefinition);
		FieldDefinition* GetFieldDefinition(StructDefinition* structDefinition, int fieldIndex);

		~Database();
	private:
		string _path;

		map<string, int> _stringMap;

		vector<CharacterDataHeader> _characterDataHeaders;
		vector<char> _characterData;

		vector<ObjectHeaderDefinition> _objectHeaderDefinitions;
		vector<char> _objects;
		vector<StructDefinition> _structDefinitions;
		vector<FieldDefinition> _fieldDefinitions;
	};
}

#endif