#ifndef __FIELD_TYPE__
#define __FIELD_TYPE__

namespace race
{
	typedef enum PrimitiveType
	{
		PrimitiveTypeUnknown,
		PrimitiveTypeInt8,
		PrimitiveTypeUint8,
		PrimitiveTypeInt16,
		PrimitiveTypeUint16,
		PrimitiveTypeInt32,
		PrimitiveTypeUint32,
		PrimitiveTypeInt64,
		PrimitiveTypeUint64,
		PrimitiveTypeBoolean, // stored in 1 byte
		PrimitiveTypeFloat,
		PrimitiveTypeDouble,
		PrimitiveTypeChar,
		PrimitiveTypeString,
		PrimitiveTypePOD,
		PrimitiveTypeReference,
		PrimitiveTypeInlineStruct,
		PrimitiveTypeArray,
		PrimitiveTypeEnum,
		PrimitiveTypeBitField,
		PrimitiveTypeSymbol,
		PrimitiveTypeCData,
		PrimitiveTypeBulkItem
	} PrimitiveType;

	class FieldType
	{
	public:
		FieldType(PrimitiveType primitiveType, int param)
		{
			this->primitiveType = primitiveType;
			this->param = param;
		}

		PrimitiveType primitiveType;
		int param;
	};

	typedef enum
	{
		ObjectTypeStructured,
		ObjectTypeUnstructured,
		ObjectTypeArray
	} ObjectType;

	class ObjectHeaderDefinition
	{
	public:
		ObjectHeaderDefinition(ObjectType type, unsigned int offset);
		ObjectType GetType()
		{
			return _type;
		}

		unsigned int GetOffset()
		{
			return _offset;
		}
	private:
		ObjectType _type:3;
		unsigned int _offset:29;
	};

	class FieldDefinition
	{
	public:
		FieldDefinition(unsigned short name, const FieldType& type, unsigned short offset)
			:_name(name),
			_type(type.primitiveType),
			_offset(offset),
			_param(type.param)
		{
		}

		unsigned short GetName()
		{
			return _name;
		}

		FieldType GetType()
		{
			return FieldType(_type, _param);
		}

		unsigned short GetOffset()
		{
			return _offset;
		}

		unsigned short GetParam()
		{
			return _param;
		}

	private:
		unsigned short _name;
		PrimitiveType _type;
		unsigned short _offset;
		unsigned short _param;
	};

	class StructuredObjectDefinition
	{
	public:
		StructuredObjectDefinition(int structIndex):_structIndex(structIndex)
		{
		}

		int GetStructIndex()
		{
			return _structIndex;
		}

		void SetStructIndex(int structIndex)
		{
			_structIndex = structIndex;
		}

	private:
		int _structIndex;
	};

	class UnstructuredObjectDefinition
	{
	public:
		UnstructuredObjectDefinition(int fieldCount):_fieldCount(fieldCount)
		{
		}

		int GetFieldCount()
		{
			return _fieldCount;
		}

		void SetFieldCount(int fieldCount)
		{
			_fieldCount = fieldCount;
		}

		int GetObjectSize()
		{
			return _objectSize;
		}

		void SetObjectSize(int objectSize)
		{
			_objectSize = objectSize;
		}

	private:
		int _fieldCount;
		int _objectSize;
	};

	class ArrayObjectDefinition
	{
	public:
		ArrayObjectDefinition(FieldType type, int size):_type(type.primitiveType),_param(type.param),_size(size)
		{
		}

		FieldType GetType()
		{
			return FieldType(_type,_param);
		}

		void SetType(FieldType& type)
		{
			_type = type.primitiveType;
			_param = type.param;
		}

		int GetSize()
		{
			return _size;
		}

		void SetSize(int size)
		{
			_size = size;
		}

	private:
		PrimitiveType _type;
		int _param;
		int _size;

	};

	class StructDefinition
	{
	public:
		StructDefinition(int name, int fieldOffset, int fieldCount):_name(name), _fieldOffset(fieldOffset),_fieldCount(fieldCount)
		{
		}

		int GetName()
		{
			return _name;
		}

		void SetName(int name)
		{
			_name = name;
		}

		int GetFieldOffset()
		{
			return _fieldOffset;
		}

		void SetFieldOffset(int fieldOffset)
		{
			_fieldOffset = fieldOffset;
		}

		int GetFieldCount()
		{
			return _fieldCount;
		}

		void SetFieldCount(int fieldCount)
		{
			_fieldCount = fieldCount;
		}

	private:
		int _name;
		int _fieldOffset;
		int _fieldCount;
	};

}
#endif