#include "stdafx.h"
#include "SettingsButtons.h"
#include "GetInputParameters.h"
#include <windowsx.h>

MainMenuSection::MainMenuSection()
{
}

MainMenuSection::~MainMenuSection()
{
}

void MainMenuSection::Init(HWND parent, int visualAttr, HINSTANCE inst)
{
	// TO DO: other fields for parameters
	InitControlPackageSize(4);
	InitCommonControlParams(parent, visualAttr, inst);

	ControlPackage copy = mCommonControlParams;
	mCommonControlParams.visualAttr |= CBS_DROPDOWN;
	AddControl("ComboBox", "", 20, 20, 200, 200);
	AddControl("ComboBox", "", 50, 20, 200, 200);

	mCommonControlParams = copy;
	AddControl("Button", "Save", 460, 330, 100, 50);
	AddControl("Button", "Close", 460, 450, 100, 50);

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
		TEXT("hann"), TEXT("hamming")
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
			char freq;
			switch ( HIWORD(wParam) )
			{
				// button clicked
			case BN_CLICKED:
				if (mControlPack[2].mControl == (HWND)lParam)
				{
					Edit_GetLine(mControlPack[0].mControl, 0, LPTSTR(&freq), 1);
					switch (freq)
					{
					case '1':
						getParams.Frequency = 11025;
						break;
					case '2':
						getParams.Frequency = 22050;
						break;
					case '4':
						getParams.Frequency = 44100;
						break;
					}
					Edit_GetLine(mControlPack[1].mControl, 0, LPTSTR(getParams.FreqType), 23);
					return 1;
					break;
				}
				else
					if (mControlPack[3].mControl == (HWND)lParam)
					{
						return 1;
					}
			}
		}
	}

	return 0;
}