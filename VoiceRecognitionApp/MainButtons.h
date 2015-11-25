#pragma once

#include <minwindef.h>
#include "Recognition.h"

#define EH_CREATEETALON	0x1000 // message on create etalon button
#define EH_RECOGNIZE	0x1001 // message on recognize button

class MainButtons
{

	HWND buttonCreateEtalon;
	HWND buttonRecognize;

	HWND hwnd;
	

public:
	MainButtons();
	~MainButtons();

	VoiceRecognition * recognition;

	void init(HWND hwnd)
	{
		this->hwnd = hwnd;
	}

	void initButtons(LPARAM );

	void initRecognitionTab(VoiceRecognition * rec)
	{
		this->recognition = rec;
	}

	int RecognizeSound();
	int CreateEtalon();
	
	int ProcessMsg(int msg);
};

