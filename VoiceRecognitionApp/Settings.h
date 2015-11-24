#pragma once

#include "SettingsButtons.h"
#include "MainButtons.h"

LRESULT CALLBACK SetProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Settings
{
public:
	Settings();
	~Settings();

	HWND settings;

	TCHAR szSettingsClass[100];			// имя класса

	ATOM SettingsClass(HINSTANCE hInstance);

	void init(HINSTANCE hInstance, TCHAR szWindowClass[], HWND hWnd);

	void Show();

	void HiddenWindow();

};

