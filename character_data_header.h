#ifndef __CHARACTER_DATA_HEADER__
#define __CHARACTER_DATA_HEADER__

namespace race
{
	class CharacterDataHeader
	{
	public:
		CharacterDataHeader(unsigned int offset, unsigned int length):
							_offset(offset),
							_length(length)
							{
							}

		unsigned int GetOffset()
		{
			return _offset;
		}

		unsigned int GetLength()
		{
			return _length;
		}
	private:
		unsigned int _offset;
		unsigned int _length;
	};
}

#endif
