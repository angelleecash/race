#include <D3DX9.h>
#include "file_input_stream.h"
#include <vector>
#include <string>
#include "m3g_loader.h"

using namespace std;


IDirect3DDevice9* device = 0;
IDirect3DIndexBuffer9 *indexBuffer = 0;
IDirect3DVertexBuffer9 * vertexBuffer = 0;

int width = 800;
int height = 600;

FileInputStream inputStream("./bmw_m3_e92_2009.dae");


struct Vertex
{
	Vertex(float x, float y, float z)
	{
		_x = x;
		_y = y;
		_z = z;
	}

	float _x, _y, _z;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
				return 0;
			}
			break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int InitD3d(HWND hwnd)
{
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;

	/*
	if(!d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps))
	{
		return 0;
	}
	*/
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	
	int vertexProcessType = -1;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessType = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS param;
	param.BackBufferWidth = width;
	param.BackBufferHeight = height;
	param.BackBufferFormat = D3DFMT_A8R8G8B8;
	param.BackBufferCount = 1;
	param.MultiSampleType = D3DMULTISAMPLE_NONE;
	param.MultiSampleQuality = 0;
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	param.hDeviceWindow = hwnd;
	param.Windowed = TRUE;
	param.EnableAutoDepthStencil = true;
	param.AutoDepthStencilFormat = D3DFMT_D24S8;
	param.Flags = 0;
	param.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	param.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	

	/*
	if(!d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &param, &device))
	{
		return 0;
	}
	*/

	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &param, &device);

	return 1;
}

void GameInit();
void GameMain();

int WINAPI WinMain2(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	M3gLoader loader("./m.m3g");
	
	return 0;
	const char CLASS_NAME[]  = "Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hinstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW & (~WS_MINIMIZEBOX) & (~WS_MAXIMIZEBOX) & (~WS_SIZEBOX),            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 
		width, height,
		//CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hinstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, showCmd);

	if(!InitD3d(hwnd))
	{
		MessageBox(0, LPCSTR("Fail to init d3d"), 0, 0);

		return 0;
	}

	GameInit();

	//MessageBox(0, LPCSTR(":)"), 0, 0);
	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}

		GameMain();
	}

	return 0;
}

void GameInit()
{
	return;
	if(FAILED( device->CreateVertexBuffer(8*sizeof(Vertex), 
							   D3DUSAGE_WRITEONLY,
							   Vertex::FVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   0)))
	{
		MessageBox(0, LPCSTR("Vertex Buffer fail"), 0, 0);
		return;
	}

	if(FAILED( device->CreateIndexBuffer(36*sizeof(WORD),
							  D3DUSAGE_WRITEONLY,
							  D3DFMT_INDEX16,
							  D3DPOOL_MANAGED,
							  &indexBuffer,
							  0)))
	{
		MessageBox(0, LPCSTR("Index Buffer fail"), 0, 0);
		return;
	}

	Vertex* vertices = 0;
	vertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f,  1.0f, -1.0f);
	vertices[2] = Vertex( 1.0f,  1.0f, -1.0f);
	vertices[3] = Vertex( 1.0f, -1.0f, -1.0f);

	vertices[4] = Vertex(-1.0f, -1.0f,  1.0f);
	vertices[5] = Vertex(-1.0f,  1.0f,  1.0f);
	vertices[6] = Vertex( 1.0f,  1.0f,  1.0f);
	vertices[7] = Vertex( 1.0f, -1.0f,  1.0f);

	vertexBuffer->Unlock();


	WORD* indices = 0;
	indexBuffer->Lock(0, 0, (void**) &indices, 0);

	indices[0] = 0;indices[1] = 1;indices[2] = 2;
	indices[3] = 0;indices[4] = 2;indices[5] = 3;

	indices[6] = 0;indices[7] = 6;indices[0] = 5;
	indices[9] = 0;indices[10] = 7;indices[0] = 6;

	indices[12] = 4;indices[13] = 5;indices[14] = 1;
	indices[15] = 4;indices[16] = 1;indices[17] = 0;

	indices[18] = 3;indices[19] = 2;indices[20] = 6;
	indices[21] = 3;indices[22] = 6;indices[23] = 7;

	indices[24] = 1;indices[25] = 5;indices[26] = 6;
	indices[27] = 1;indices[28] = 6;indices[29] = 2;

	indices[30] = 4;indices[31] = 0;indices[32] = 3;
	indices[33] = 4;indices[34] = 3;indices[35] = 7;

	indexBuffer->Unlock();

	D3DXVECTOR3 cameraPosition(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 lookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &cameraPosition, &lookAt, &up);

	device->SetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI*0.5f, (float)(width*1.0/height), 1.0f, 1000.0f);

	device->SetTransform(D3DTS_PROJECTION, &projection);

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	
}

void GameMain()
{
	return;
	D3DXMATRIX yRotation;

	static float yRotationAngle = 0.0f;

	D3DXMatrixRotationY(&yRotation, yRotationAngle);
	yRotationAngle+= 0.0002f;

	if(yRotationAngle >= 6.28)
	{
		yRotationAngle = 0;
	}

	device->SetTransform(D3DTS_WORLD, &yRotation);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->Present(0, 0, 0, 0);

	device->BeginScene();

	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetIndices(indexBuffer);
	device->SetFVF(Vertex::FVF);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	device->EndScene();

	device->Present(0, 0, 0, 0);
}