#include "config.h"

#if PLATFORM == PLATFORM_WINDOWS

#include <windows.h>
#include "device.h"

#include "application.h"

using namespace race;

static HWND window = 0;
static DWORD windowStyle = 0;
static HDC hdc = 0;
static HINSTANCE instance;
static bool running = true;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			if (lParam == 0)
			{
				/*
				size_t id = LOWORD(wParam);
				if (id >= 0 && id < s_MenuOptions.size())
				{
				const MenuOption& option = s_MenuOptions[id];
				SetOptionValue(option.m_Name, option.m_Value);
				}
				*/
			}
		}
	case WM_PAINT:
		break;
	case WM_CLOSE:
		running = false;
		PostQuitMessage(0);
		return 0;
	case WM_QUIT:
		return 0;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		return 0;
	case WM_SETFOCUS:
		//OnActivate
		return 0;
	case WM_KILLFOCUS:
		//OnDeactivate
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			default:
				//OnSimulatorKeyEvent(message, wParam, lParam);
				break;
			}

			break;
		}
	case WM_DROPFILES:
		{
			//OnSimulatorDropFiles(wParam);
			return 0;
		}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

static void RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= 0;//LoadIcon(m_hInstance, _T("ICON"));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName		= 0;
	wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszClassName	= "MyWindow";

	RegisterClass(&wc);
}



static void CreateSimulatorWindow()
{
	DeviceInfo deviceInfo = race::GetDeviceInfo();
	int width = deviceInfo.GetWidth();
	int height = deviceInfo.GetHeight();

	windowStyle = WS_VISIBLE|WS_CAPTION|WS_SYSMENU;
	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = width + windowRect.left;
	windowRect.bottom = height + windowRect.top;

	AdjustWindowRect(&windowRect, windowStyle, FALSE);

	window = CreateWindowEx(0, 
		"MyWindow",
		"Race",
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		0,
		0,
		instance,
		NULL);


	hdc = GetDC(window);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	

	instance = hInstance;

	RegisterWindowClass(instance);

	CreateSimulatorWindow();

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	Application& app = Application::GetInstance();
	app.Init();

	MSG msg;
	while (running)
	{
		app.Run();

		while(PeekMessage(&msg, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;																					 
}				
#endif