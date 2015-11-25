#include "stdafx.h"
#include "Recognition.h"
#include <commctrl.h>

VoiceRecognition::VoiceRecognition()
{
}


VoiceRecognition::~VoiceRecognition()
{
}

void VoiceRecognition::initWindow(LPARAM lParam)
{
	buttonRecognizeWritedVoice = CreateWindowA("button", "Write voice for recognition", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 300, 185, 300, 50, hwnd, (HMENU)EH_RECOGNIZEWRITEVOICE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	buttonRecognizeWAV = CreateWindowA("button", "Recognize voice from WAV file", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 300, 255, 300, 50, hwnd, (HMENU)EH_RECOGNIZEWAV, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	ShowWindow(buttonRecognizeWritedVoice, 0);
	ShowWindow(buttonRecognizeWAV, 0);

}

int VoiceRecognition::ProcessMsg(int msg)
{
	switch (msg)
	{
	case EH_RECOGNIZEWRITEVOICE:
		return RecognizeWritedVoice();
		break;

	case EH_RECOGNIZEWAV:
		return RecognizeWAV();
		break;

	default:
		return 0;
	}
}

int VoiceRecognition::newWindow()
{
	ShowWindow(buttonRecognizeWritedVoice, 1);
	ShowWindow(buttonRecognizeWAV, 1);
	return 1;
}

int VoiceRecognition::RecognizeWritedVoice()
{
	ShowWindow(buttonRecognizeWritedVoice, 0);
	ShowWindow(buttonRecognizeWAV, 0);

	voicewrite->newWindow();

	return 1;
}

int VoiceRecognition::RecognizeWAV()
{
	ShowWindow(buttonRecognizeWAV, 0);
	ShowWindow(buttonRecognizeWritedVoice, 0);

	wavrec->newWindow();

	return 1;
}
