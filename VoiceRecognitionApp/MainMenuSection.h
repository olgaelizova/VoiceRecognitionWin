#pragma once
#include "CommonSection.h"

#define EDIT_TEST_CMD	1000
#define BUTTON_TEST_CMD	1001

class MainMenuSection :
	public CommonSection
{

protected:
	void InitDropdown();
public:
	MainMenuSection();
	~MainMenuSection();

	void Init(HWND parent, int visualAttr, HINSTANCE inst);
	int ProcessMsg(WPARAM wParam, LPARAM lParam);

	static int TestFunctinButton();


	TCHAR szBuffer[100];
};

