#pragma once
#include <minwindef.h>

#include <windows.h>
#include <commctrl.h>

#define EH_RESULT	0x1015

class WAVRecognition
{
public:
	WAVRecognition();
	~WAVRecognition();

	HWND result;

	HWND hwnd;

	void init(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	void initWindow(LPARAM);

	int newWindow();

	int ProcessMsg(int msg);

};


#include <stdio.h>
#include <math.h>

#include "FFTAnalysis.h"
#include "ReadEtalons.h"
#include "SpectrumAnalysis.h"
#include "RegrKoefAnalysis.h"

#include "InputParameters.h"

//#include "FindingMaxKoef.h"

#include "PushButtonHandler.h"

#include "VoiceWrite.h"

using namespace std;

int showResult(HWND);