#include "stdafx.h"
#include "MainMenuSection.h"
#include <windowsx.h>


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

	ControlPackage copy = mCommonControlParams;
	mCommonControlParams.visualAttr |= CBS_DROPDOWN;
	AddControl("ComboBox", "", 20, 20, 200, 200);
	AddControl("ComboBox", "", 50, 20, 200, 200);

	mCommonControlParams = copy;
	AddControl("Button", "Recognize", 20, 230, 220, 50);

	InitDropdown();
}

void MainMenuSection::InitDropdown()
{
	TCHAR Freq[3][10] =
	{
		TEXT("11025"), TEXT("22050"), TEXT("44100")
	};

	WCHAR A[16];
	int  k = 0;

	memset(&A, 0, sizeof(A));
	for (k = 0; k < 3; k++)
	{
		wcscpy_s(A, sizeof(A) / sizeof(WCHAR), (WCHAR*)Freq[k]);

		// Add string to combobox.
		SendMessage(mControlPack[0].mControl, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	// Send the CB_SETCURSEL message to display an initial item 
	//  in the selection field  
	SendMessage(mControlPack[0].mControl, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);


	///////////////////////

	TCHAR Hamming[2][17] =
	{
		TEXT("Hamming Window"), TEXT("Some other stuff")
	};

	memset(&A, 0, sizeof(A));
	for (k = 0; k < 2; k++)
	{
		wcscpy_s(A, sizeof(A) / sizeof(WCHAR), (WCHAR*)Hamming[k]);

		// Add string to combobox.
		SendMessage(mControlPack[1].mControl, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	// Send the CB_SETCURSEL message to display an initial item 
	//  in the selection field  
	SendMessage(mControlPack[1].mControl, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

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
				// button clicked
			case BN_CLICKED:
				//SendMessage(mControlPack[0].mParent, WM_COMMAND, MAKEWPARAM(0, EM_GETLINE), (LPARAM)mControlPack[0].mControl);
				//char * text = new char[23];
				//Edit_GetLine(mControlPack[0].mControl, 0, LPTSTR(text), 23);

				break;

			}
		}
	}

	return 1;
	//return result;
}