#include "DirectX.h"


#define _CRT_SECURE_NO_WARNINGS

HWND InitWindow(HMODULE hModule);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HMODULE hModule, HMODULE hPrevModule, LPWSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	
	DX Dx;
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hModule);

	if (wndHandle)
	{
		Dx.OfflineCreation(hModule, &wndHandle);

		ShowWindow(wndHandle, nCmdShow);

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//if (Dx.isStartMenu == true)
				//{
				//	//Dx.startMenuLoop();
				//}
				/*else
				{*/
					Dx.Update();
			/*	}*/
			}
		}
		DestroyWindow(wndHandle);
	}

	FreeConsole();

	return (int)msg.wParam;
}

HWND InitWindow(HMODULE hModule)
{
	WNDCLASSEX wce = { 0 };
	wce.cbSize = sizeof(WNDCLASSEX);
	wce.style = CS_HREDRAW | CS_VREDRAW;
	wce.lpfnWndProc = WndProc;
	wce.hInstance = hModule;
	wce.lpszClassName = WLABEL;
	if (!RegisterClassEx(&wce))
	{
		return false;
	}

	RECT rc = { 0,0,HEIGHT,WIDTH };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);

	auto wWidth = rc.right - rc.left;
	auto wHeight = rc.bottom - rc.top;

	HWND handle = CreateWindow(
		WLABEL,
		WLABEL,
		WS_OVERLAPPEDWINDOW,
		((GetSystemMetrics(SM_CXSCREEN) / 2) - (wWidth / 2)),
		((GetSystemMetrics(SM_CYSCREEN) / 2) - (wHeight / 2)),
		wWidth,
		wHeight,
		nullptr,
		nullptr,
		hModule,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, msg, wp, lp);
}
