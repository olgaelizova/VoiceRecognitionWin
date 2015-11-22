#include "stdafx.h"
#include "MainMenuSection.h"


MainMenuSection::MainMenuSection()
{
}


MainMenuSection::~MainMenuSection()
{
}

void MainMenuSection::Init(HWND parent, int visualAttr, HINSTANCE inst)
{
	InitControlPackageSize(4);
	InitCommonControlParams(parent, visualAttr, inst);

	AddControl("Edit", "Etalon", 20, 20, 80, 20);
	AddControl("Button", "Recognize", 20, 120, 220, 50);
	AddControl("Button", "Recognize", 200, 120, 100, 50);
	AddControl("Button", "Recognize", 300, 120, 324, 50);
}

int MainMenuSection::ProcessMsg(WPARAM wParam, LPARAM lParam)
{
	int result = 0;
	int s;

	for (int i = 0; i < mControlCount; i++)
	{
		// check if the notification came from this section's control
		if (mControlPack[i].mControl == (HWND)lParam)
		{

			switch ( HIWORD(wParam) )
			{
			case BN_CLICKED:
				SendMessage(mControlPack[0].mParent, WM_COMMAND, MAKEWPARAM(0, EM_GETLINE), (LPARAM)mControlPack[0].mControl);
				break;

			case EM_GETLINE:
				char * text = (char*)lParam;
				bool re = true;
				break;
			}
		}
	}

	return 1;
	//return result;
}