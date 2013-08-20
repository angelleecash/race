#include <assert.h>
#include "my_assert.h"

void ASSERT(int condition, string message)
{
	//assert(condition);
	if(!condition)
	{
		throw;
	}
	
}