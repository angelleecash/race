#include "main.h"

#define PLATFORM PLATFORM_WINDOWS

#if PLATFORM == PLATFORM_WINDOWS
#include "windows.h"
	#define ENTRY_POINT \
			int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)\
			{																								 \
				return 0;																					 \
			}																								 	
#elif PLATFORM == PLATFORM_IOS
#elif PLATFORM == PLATFORM_ANDROID
#elif PLATFORM == PLATFORM_MAC
#elif PLATFORM == PLATFORM_LINUX
#endif

ENTRY_POINT
