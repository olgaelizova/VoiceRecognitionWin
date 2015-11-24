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


	buttonRecognizeWritedVoice = CreateWindow(TEXT("button"), "Write voice for recognition", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 300, 185, 200, 100, hwnd, (HMENU)EH_RECOGNIZEWRITEVOICE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	buttonRecognizeWAV = CreateWindow(TEXT("button"), "Recognize voice from WAV file", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 300, 255, 200, 100, hwnd, (HMENU)EH_RECOGNIZEWAV, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	ShowWindow(buttonRecognizeWritedVoice, 0);
	ShowWindow(buttonRecognizeWAV, 0);

}

int VoiceRecognition::ProcessMsg(int msg)
{
	switch (msg)
	{

	case EH_TABCONTROL1:
		return tabCtrl1Click();
		break;

	case EH_TABCONTROL2:
		return tabCtrl2Click();
		break;

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

int VoiceRecognition::tabCtrl1Click()
{
	RecognizeWritedVoice();
	return 1;
}

int VoiceRecognition::tabCtrl2Click()
{
	RecognizeWAV();
	return 1;
}


int VoiceRecognition::newWindow()
{
	//ShowWindow(hThisWnd,1);
	ShowWindow(hTabCtrl, 1);
	ShowWindow(hTabCtrl2, 1);

	//RecognizeWritedVoice();
	//RecognizeWAV();

	return 1;
}

int VoiceRecognition::RecognizeWritedVoice()
{
	ShowWindow(buttonRecognizeWritedVoice, 1);
	ShowWindow(buttonRecognizeWAV, 0);

	return 1;
}

int VoiceRecognition::RecognizeWAV()
{
	ShowWindow(buttonRecognizeWAV, 1);
	ShowWindow(buttonRecognizeWritedVoice, 0);

	return 1;
}
