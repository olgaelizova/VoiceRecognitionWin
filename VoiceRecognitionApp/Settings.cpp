#include "stdafx.h"
#include "Settings.h"
#include "Resource.h"

MainMenuSection mainMenu;
Settings settings;

Settings::Settings()
{
}

Settings::~Settings()
{
}

ATOM Settings::SettingsClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	LoadString(hInstance, IDC_SETTINGS, szSettingsClass, 100);

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = SetProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszClassName = szSettingsClass;
	wcex.lpszMenuName = NULL;

	return RegisterClassEx(&wcex);
}

void Settings::init(HINSTANCE hInstance, TCHAR szWindowClass[], HWND hWnd)
{
	settings = CreateWindowA(
		"SETTINGS",
		"Settings",      
		WS_CAPTION, 
		CW_USEDEFAULT,      
		CW_USEDEFAULT,      
		800,                 
		600,                 
		hWnd,
		NULL,               
		hInstance,      
		NULL                
		);

	mainMenu.Init(settings, WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, hInstance);

	//ShowWindow(settings, 0);
	//UpdateWindow(settings);
}

void Settings::Show()
{
	ShowWindow(settings, 1);
	UpdateWindow(settings);
}

void Settings::HiddenWindow()
{
	ShowWindow(settings, 0);
	UpdateWindow(settings);
}

LRESULT CALLBACK SetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	bool stop = true;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		if (mainMenu.ProcessMsg(wParam, lParam) == 0)
		{
			switch (wmId)
			{
			case 0000:
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		else
		{
			settings.HiddenWindow();
		}

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONUP:
		break;

	case WM_CREATE: // main buttons
		
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
