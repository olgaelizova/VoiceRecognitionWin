#pragma once

#include <minwindef.h>

#include <windows.h>
#include <commctrl.h>

#include "VoiceWriteRecognition.h"
#include "WAVRecognition.h"

#define EH_RECOGNIZEWRITEVOICE	0x1002 // message on create etalon button
#define EH_RECOGNIZEWAV	0x1003 // message on recognize button

class VoiceRecognition
{

	HWND buttonRecognizeWritedVoice;
	HWND buttonRecognizeWAV;

	HWND hwnd;

public:
	VoiceRecognition();
	~VoiceRecognition();

	void init(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	void initWindow(LPARAM);
	//void showFrame(bool show);

	VoiceWriteRecognition * voicewrite;

	void initRecognitionVoiceWrite(VoiceWriteRecognition * record)
	{
		this->voicewrite = record;
	}

	WAVRecognition * wavrec;

	void initWAVRecognition(WAVRecognition * record)
	{
		this->wavrec = record;
	}

	int newWindow();

	int RecognizeWAV();
	int RecognizeWritedVoice();

	int ProcessMsg(int msg);
};
