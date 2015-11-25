#include "stdafx.h"
#include "MainButtons.h"
#include "Recognition.h"

MainButtons::MainButtons()
{
}


MainButtons::~MainButtons()
{
}

void MainButtons::initButtons(LPARAM lParam)
{
	buttonCreateEtalon = CreateWindowA("button", "Create Etalon", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 300, 185, 200, 50, hwnd, (HMENU)EH_CREATEETALON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	buttonRecognize = CreateWindowA("button", "Recognize Voice", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 300, 255, 200, 50, hwnd, (HMENU)EH_RECOGNIZE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
	
}

int MainButtons::ProcessMsg(int msg)
{
	if (recognition->ProcessMsg(msg) == 0)
		switch (msg)
	{
		case EH_CREATEETALON:
			return CreateEtalon();
			break;

		case EH_RECOGNIZE:
			return RecognizeSound();
			break;

		default:
			return 0;
	}
	else
		return 0;
}

int MainButtons::CreateEtalon()
{
	ShowWindow(buttonCreateEtalon, 1);
	ShowWindow(buttonRecognize, 0);
	ShowWindow(buttonCreateEtalon, 0);

	return 1;
}

int MainButtons::RecognizeSound()
{
	ShowWindow(buttonRecognize, 1);
	ShowWindow(buttonCreateEtalon, 0);
	ShowWindow(buttonRecognize, 0);

	recognition->newWindow();

	return 1;
}