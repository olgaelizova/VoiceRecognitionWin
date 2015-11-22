#pragma once

#include <minwindef.h>

#include <windows.h>
#include <commctrl.h>

#define EH_RECOGNIZEWRITEVOICE	0x1002 // message on create etalon button
#define EH_RECOGNIZEWAV	0x1003 // message on recognize button
#define EH_TABCONTROL1 0x1004
#define EH_TABCONTROL2 0x1005

class VoiceRecognition
{

	HWND buttonRecognizeWritedVoice;
	HWND buttonRecognizeWAV;

	HWND hThisWnd;
	HWND hTabCtrl, hTabCtrl2;

	HWND hwnd;

public:
	VoiceRecognition();
	~VoiceRecognition();

	void init(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	void initWindow(LPARAM);
	void showFrame(bool show);

	int newWindow();

	int RecognizeWAV();
	int RecognizeWritedVoice();

	int ProcessMsg(int msg);

	///////////
	// TAB CONTROLS FUNCTIONS
	//////////

	int tabCtrl1Click();
	int tabCtrl2Click();
};
