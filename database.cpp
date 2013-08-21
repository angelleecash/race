#include "file_input_stream.h"
#include "database.h"
#include "util.h"
#include "my_assert.h"

namespace race
{


	Database::Database(string path):_path(path)
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

	/*
	Object Database::GetRoot()
	{
	return Object();
	}
	*/

	MyObject Database::GetMyObject(const char* name)
	{
		return MyObject();
	}

	MyObject Database::ReadMyObject(const char* name, int objectIndex, int structIndex, int dataOffset)
	{
		FieldType fieldType = GetFieldType(name, objectIndex, structIndex, dataOffset);

		ASSERT(fieldType.primitiveType != PrimitiveTypeUnknown, "Unknown type");

		const char* data = GetData(name);

		return Read(data, name, fieldType);
	}

	const char* Database::GetData(const char* name)
	{
		return 0;
	}

	MyObject Database::Read(const char* data, const char* name, const FieldType& fieldType)
	{
		return MyObject();
	}

	ObjectHeaderDefinition* Database::GetObjectHeaderDefinition(unsigned int objectIndex)
	{
		ASSERT(objectIndex >=0 && objectIndex < _objectHeaderDefinitions.size(), "Out of bounds");

		return &_objectHeaderDefinitions[objectIndex];
	}

	FieldType Database::GetFieldType(const char* name, int objectIndex, int structIndex, int dataOffset)
	{
		//TODO how c data are initialized
		int nameIndex = GetCData(name);

		ASSERT(nameIndex >= 0, "Field name index should be greater than or equal to 0");

		ObjectHeaderDefinition* objectHeaderDefinition = GetObjectHeaderDefinition(objectIndex);

		FieldDefinition* fieldDefinition = NULL;

		if(structIndex != -1)
		{
			GetStructuredObjectField(objectHeaderDefinition, structIndex, dataOffset, nameIndex, &fieldDefinition);
		}
		else
		{
			GetUnstructuredObjectField(objectHeaderDefinition, nameIndex, &fieldDefinition);
		}

		if(fieldDefinition)
		{
			return fieldDefinition->GetType();
		}

		return FieldType(PrimitiveTypeUnknown, 0);
	}

	char* Database::GetStructuredObjectField(ObjectHeaderDefinition* objectHeaderDefinition, int structIndex, int dataOffset, int nameIndex, FieldDefinition** fieldDefinitionResult)
	{
		char* data = GetObjectData(objectHeaderDefinition);
		data += GetObjectDefinitionSize(objectHeaderDefinition) + dataOffset;
		StructDefinition* structDefinition = GetStructDefinition(structIndex);
		FieldDefinition* fieldDefinition = GetFieldDefinitionByName(structDefinition, nameIndex);

		if(!fieldDefinition)
		{
			return NULL;
		}

		*fieldDefinitionResult = fieldDefinition;
		return data + fieldDefinition->GetOffset();
	}

	FieldDefinition* Database::GetFieldDefinitionByName(StructDefinition* structDefinition, int nameIndex)
	{
		FieldDefinition* fieldDefinition = &_fieldDefinitions[structDefinition->GetFieldOffset()];
		for(int i=0; i<structDefinition->GetFieldCount(); i++, fieldDefinition++)
		{
			if(fieldDefinition->GetName() == nameIndex)
			{
				return fieldDefinition;
			}
		}

		return NULL;
	}

	char* Database::GetUnstructuredObjectField(ObjectHeaderDefinition* objectHeaderDefinition, int nameIndex, FieldDefinition** fieldDefinitionResult)
	{
		ASSERT(objectHeaderDefinition->GetType() == ObjectTypeUnstructured, "Wrong type");

		char* data = GetObjectData(objectHeaderDefinition);
		UnstructuredObjectDefinition* unstructuredObjectDefinition = reinterpret_cast<UnstructuredObjectDefinition*>(data);

		int size = unstructuredObjectDefinition->GetObjectSize();
		int fieldOffset = GetObjectDefinitionSize(objectHeaderDefinition);
		while(fieldOffset < size)
		{
			FieldDefinition* fieldDefinition = reinterpret_cast<FieldDefinition*>(data+fieldOffset);
			int valueOffset = fieldDefinition->GetOffset();
			if(fieldDefinition->GetName() == nameIndex)
			{
				*fieldDefinitionResult = fieldDefinition;
				return data+valueOffset;
			}

			fieldOffset = valueOffset + GetFieldSize(*fieldDefinition);
			fieldOffset = Align(fieldOffset, 2);
		}

		return NULL;
	}

	int Database::GetFieldSize(FieldDefinition& fieldDefinition)
	{
		return GetFieldSize(fieldDefinition.GetType());
	}

	int Database::GetFieldSize(FieldType& fieldType)
	{
		ASSERT(PrimitiveTypeUnknown == fieldType.primitiveType, "Wrong type");

		switch(fieldType.primitiveType)
		{
		case PrimitiveTypeInt8:
		case PrimitiveTypeUint8:
			return 1;

		case PrimitiveTypeInt16:
		case PrimitiveTypeUint16:
			return 2;
		case PrimitiveTypeInt32:
		case PrimitiveTypeUint32:
		case PrimitiveTypeEnum:
		case PrimitiveTypeBitField:
			return 4;

		case PrimitiveTypeInt64:
		case PrimitiveTypeUint64:
			return 8;
		case PrimitiveTypeBoolean:
			return 1;
		case PrimitiveTypeFloat:
			return 4;
		case PrimitiveTypeDouble:
			return 8;
		case PrimitiveTypeChar:
			return 2;
		case PrimitiveTypeString:
			return 2;
		case PrimitiveTypePOD:
			return fieldType.param;
		case PrimitiveTypeReference:
			return 4;
		case PrimitiveTypeArray:
			return 4;
		case PrimitiveTypeSymbol:
			return 2;
		case PrimitiveTypeInlineStruct:
			return GetStructSize(GetStructDefinition(fieldType.param));
		case PrimitiveTypeCData:
			return 2;
		case PrimitiveTypeBulkItem:
			return 4;
		}

		ASSERT(PrimitiveTypeUnknown == fieldType.primitiveType, "Unknown type");

		return -1;
	}

	int Database::GetStructSize(StructDefinition* structDefinition)
	{
		if(structDefinition->GetFieldCount() == 0)
		{
			return 0;
		}

		FieldDefinition* lastField = GetFieldDefinition(structDefinition, structDefinition->GetFieldCount() - 1);
		return lastField->GetOffset() + GetFieldSize(*lastField);
	}

	FieldDefinition* Database::GetFieldDefinition(StructDefinition* structDefinition, int fieldIndex)
	{
		return &_fieldDefinitions[structDefinition->GetFieldOffset() + fieldIndex];
	}

	StructDefinition* Database::GetStructDefinition(int structIndex)
	{
		return &_structDefinitions[structIndex];
	}

	int Database::GetObjectDefinitionSize(ObjectHeaderDefinition* objectHeaderDefinition)
	{
		switch(objectHeaderDefinition->GetType())
		{
		case ObjectTypeStructured:
			return sizeof(StructuredObjectDefinition);
		case ObjectTypeUnstructured:
			return sizeof(UnstructuredObjectDefinition);
		case ObjectTypeArray:
			return sizeof(ArrayObjectDefinition);
		}

		ASSERT(false, "Unknown object type");
		return -1;
	}

	char* Database::GetObjectData(ObjectHeaderDefinition* objectHeaderDefinition)
	{
		return &_objects[objectHeaderDefinition->GetOffset()];
	}

	int Database::GetCData(const char* name)
	{
		int len = strlen(name);

		for(unsigned int i=0; i < _characterDataHeaders.size(); i++)
		{
			CharacterDataHeader& header = _characterDataHeaders[i];

			if(header.GetLength() != len)
			{
				continue;
			}

			unsigned int offset = header.GetOffset();
			const char* str = &_characterData[offset];
			if(!strncmp(str, name, len))
			{
				return i;
			}
		}

		return -1;
	}

	Database::~Database()
	{

	}
}