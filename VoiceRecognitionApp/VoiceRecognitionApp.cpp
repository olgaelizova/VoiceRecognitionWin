// VoiceRecognitionApp.cpp: определяет точку входа для приложения.
//
#include "stdafx.h"

#ifdef _WIN32

#include "VoiceRecognitionApp.h"
#include "GetInputParameters.h"
#include "MainButtons.h"
#include "Recognition.h"
#include "VoiceWriteRecognition.h"
#include "WAVRecognition.h"

#include "Settings.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

MainButtons mainButtons;
VoiceRecognition recognition;
VoiceWriteRecognition voicewrite;
WAVRecognition wavrec;
extern Settings settings;

GetInputParameters getParams;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VOICERECOGNITIONAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	settings.SettingsClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VOICERECOGNITIONAPP));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VOICERECOGNITIONAPP));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_VOICERECOGNITIONAPP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


	HWND hWnd;

	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	//hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	hWnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szWindowClass,         /* Classname */
		szTitle,       /* Title Text */
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		800,                 /* The programs width */
		600,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /*use class menu */
		hInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
		);

	if (!hWnd)
	{
		return FALSE;
	}

	settings.init(hInstance, szWindowClass, hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	bool stop = true;

	// DEBUG a
	unsigned short a;


	switch (message)
	{
		/*messages for writing voice*/
	case MM_WIM_OPEN:
		// Shrink down the save buffer

		voicewrite.pSaveBuffer = reinterpret_cast <PBYTE>(realloc(voicewrite.pSaveBuffer, 1));
		// Add the buffers

		waveInAddBuffer(voicewrite.hWaveIn, voicewrite.pWaveHdr1, sizeof(WAVEHDR));
		waveInAddBuffer(voicewrite.hWaveIn, voicewrite.pWaveHdr2, sizeof(WAVEHDR));

		// Begin sampling

		voicewrite.bRecording = TRUE;
		voicewrite.bEnding = FALSE;
		voicewrite.dwDataLength = 0;
		waveInStart(voicewrite.hWaveIn);
		return TRUE;

	case MM_WIM_DATA:

		// Reallocate save buffer memory

		voicewrite.pNewBuffer = reinterpret_cast <PBYTE> (realloc(voicewrite.pSaveBuffer, voicewrite.dwDataLength +
			((PWAVEHDR)lParam)->dwBytesRecorded));

		if (voicewrite.pNewBuffer == NULL)
		{
			waveInClose(voicewrite.hWaveIn);

			//MessageBox(voicewrite.hwnd, voicewrite.szMemError, voicewrite.szAppName,
			//	MB_ICONEXCLAMATION | MB_OK);
			return TRUE;
		}

		voicewrite.pSaveBuffer = voicewrite.pNewBuffer;
		CopyMemory(voicewrite.pSaveBuffer + voicewrite.dwDataLength, ((PWAVEHDR)lParam)->lpData,
			((PWAVEHDR)lParam)->dwBytesRecorded);

		voicewrite.currentSaveBufferPos += voicewrite.dwDataLength;

		voicewrite.dwDataLength += ((PWAVEHDR)lParam)->dwBytesRecorded;

		if (voicewrite.bEnding)
		{
			waveInClose(voicewrite.hWaveIn);
			return TRUE;
		}
		// Send out a new buffer


		waveInAddBuffer(voicewrite.hWaveIn, (PWAVEHDR)lParam, sizeof(WAVEHDR));

		voicewrite.parseSoundPiece();

		RECT rc;
		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, TRUE);

		return TRUE;

	case MM_WIM_CLOSE:
		// Free the buffer memory

		waveInUnprepareHeader(voicewrite.hWaveIn, voicewrite.pWaveHdr1, sizeof(WAVEHDR));
		waveInUnprepareHeader(voicewrite.hWaveIn, voicewrite.pWaveHdr2, sizeof(WAVEHDR));

		free(voicewrite.pBuffer1);
		free(voicewrite.pBuffer2);

		// Enable and disable buttons
		if (voicewrite.dwDataLength > 0)
		{
			EnableWindow(voicewrite.buttonRecognize, TRUE);
		}
		voicewrite.bRecording = FALSE;

		if (voicewrite.bTerminating)
			SendMessage(voicewrite.hwnd, WM_SYSCOMMAND, SC_CLOSE, 0L);

		return TRUE;

	case MM_WOM_OPEN:

		// Set up header

		voicewrite.pWaveHdr1->lpData = reinterpret_cast <CHAR*>(voicewrite.pSaveBuffer);
		voicewrite.pWaveHdr1->dwBufferLength = voicewrite.dwDataLength;
		voicewrite.pWaveHdr1->dwBytesRecorded = 0;
		voicewrite.pWaveHdr1->dwUser = 0;
		voicewrite.pWaveHdr1->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		voicewrite.pWaveHdr1->dwLoops = voicewrite.dwRepetitions;
		voicewrite.pWaveHdr1->lpNext = NULL;
		voicewrite.pWaveHdr1->reserved = 0;

		// Prepare and write

		waveOutPrepareHeader(voicewrite.hWaveOut, voicewrite.pWaveHdr1, sizeof(WAVEHDR));
		waveOutWrite(voicewrite.hWaveOut, voicewrite.pWaveHdr1, sizeof(WAVEHDR));

		voicewrite.bEnding = FALSE;
		voicewrite.bPlaying = TRUE;
		return TRUE;

	case MM_WOM_DONE:

		waveOutUnprepareHeader(voicewrite.hWaveOut, voicewrite.pWaveHdr1, sizeof(WAVEHDR));
		waveOutClose(voicewrite.hWaveOut);
		EnableWindow(voicewrite.buttonRecognize, TRUE);
		EnableWindow(voicewrite.buttonRecord, TRUE);
		EnableWindow(voicewrite.buttonPlay, TRUE);
		return TRUE;

	case MM_WOM_CLOSE:

		voicewrite.dwRepetitions = 1;
		voicewrite.bPlaying = FALSE;
		return TRUE;
		/*end commsnds vriting voice*/

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		if (mainButtons.ProcessMsg(wmId) == 0)
		{
			if (voicewrite.ProcessMsg(wmId) == 0)
			{
				if (wavrec.ProcessMsg(wmId) == 0)
				{
					switch (wmId)
					{
					case IDM_ABOUT:
						DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
						break;

					case IDM_SETTINGS:
						//TODO:
						settings.Show();
						break;

					case IDM_EXIT:
						DestroyWindow(hWnd);
						break;

					default:
						return DefWindowProc(hWnd, message, wParam, lParam);
					}
					break;
				}
			}
		}

	case WM_PAINT:

		if (voicewrite.PLAY == FALSE && voicewrite.isBeingRecorded == FALSE && voicewrite.blackScreenAvailable)
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: добавьте любой код отрисовки...
			RECT rc;
			rc.top = 15;
			rc.left = 0;
			rc.bottom = 400;
			rc.right = 800;
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
			DeleteDC(hdc);
			EndPaint(hWnd, &ps);
		}

		else if (voicewrite.PLAY == FALSE && voicewrite.isBeingRecorded == FALSE )
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: добавьте любой код отрисовки...
			EndPaint(hWnd, &ps);
		}
		/*for voice record*/
		if (voicewrite.PLAY == TRUE)
		{
			PAINTSTRUCT 	ps;

			voicewrite.hDC = BeginPaint(voicewrite.hwnd, &ps);
		
			if (voicewrite.hDC)
			{
				RECT rc;
				rc.top = 15;
				rc.left = 0;
				rc.bottom = 400;
				rc.right = 800;
				FillRect(voicewrite.hDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

				
				if (voicewrite.PLAY == TRUE)
				{
					FillRect(voicewrite.hDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
					voicewrite.hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
					SelectObject(voicewrite.hDC, voicewrite.hPen);

					SetMapMode(voicewrite.hDC, MM_ISOTROPIC);
					SetWindowExtEx(voicewrite.hDC, 430, 315, NULL);
					SetViewportExtEx(voicewrite.hDC, 200, 200, NULL);
					SetViewportOrgEx(voicewrite.hDC, 0, 0, NULL);

					MoveToEx(voicewrite.hDC, 0, 200, NULL);

					// first draw existing points
					//if ( voicewrite.currentPicturePoint < 200)
					for (int i = 0; i < voicewrite.currentPicturePoint; i++)
					{
						LineTo(voicewrite.hDC, voicewrite.ptRecord[i].x, voicewrite.ptRecord[i].y);
					}

				}

				/*

				if (voicewrite.PLAY == TRUE)
				{
					FillRect(voicewrite.hDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
					voicewrite.Wav("stopTemp.wav");
					voicewrite.hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
					SelectObject(voicewrite.hDC, voicewrite.hPen);

					SetMapMode(voicewrite.hDC, MM_ISOTROPIC);
					SetWindowExtEx(voicewrite.hDC, 430, 315, NULL);
					SetViewportExtEx(voicewrite.hDC, 200, 200, NULL);
					SetViewportOrgEx(voicewrite.hDC, 0, 0, NULL);

					int sample = 0;
					int i = 1;
					int num = 20000;
					sample = voicewrite.readSample(i, TRUE);
					// scale the sample

					voicewrite.pt[i].x = i / 20;
					voicewrite.pt[i].y = (int)((sample)* 2) + 100;
					MoveToEx(voicewrite.hDC, voicewrite.pt[i].x, voicewrite.pt[i].y, NULL);
					while (i < num && sample != (int)0xefffffff)
					{
						// scale the sample

						voicewrite.pt[i].x = i / 20;
						voicewrite.pt[i].y = (int)((sample)* 2) + 100;

						LineTo(voicewrite.hDC, voicewrite.pt[i].x, voicewrite.pt[i].y);
						i++;
						sample = voicewrite.readSample(i, TRUE);

					}
				}

				*/
				
				DeleteObject(voicewrite.hPen);
				DeleteDC(voicewrite.hDC);
				EndPaint(voicewrite.hwnd, &ps);
			}
			return 0;
		}


		// 1 == 0 never true
		else if (voicewrite.isBeingRecorded == TRUE )
		{

			PAINTSTRUCT 	ps;

			voicewrite.hDC = BeginPaint(voicewrite.hwnd, &ps);

			if (voicewrite.hDC)
			{
				RECT rc;
				rc.top = 15;
				rc.left = 0;
				rc.bottom = 400;
				rc.right = 800;
				FillRect(voicewrite.hDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
				//if (voicewrite.PLAY == TRUE)
				{
					FillRect(voicewrite.hDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
					voicewrite.hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
					SelectObject(voicewrite.hDC, voicewrite.hPen);

					SetMapMode(voicewrite.hDC, MM_ISOTROPIC);
					SetWindowExtEx(voicewrite.hDC, 430, 315, NULL);
					SetViewportExtEx(voicewrite.hDC, 200, 200, NULL);
					SetViewportOrgEx(voicewrite.hDC, 0, 0, NULL);

					MoveToEx(voicewrite.hDC, 0, 200, NULL);

					// first draw existing points
					//if ( voicewrite.currentPicturePoint < 200)
					for (int i = 0; i < voicewrite.currentPicturePoint; i++)
					{
						LineTo(voicewrite.hDC, voicewrite.ptRecord[i].x, voicewrite.ptRecord[i].y);
					}


				}
				DeleteObject(voicewrite.hPen);
				DeleteDC(voicewrite.hDC);
				EndPaint(voicewrite.hwnd, &ps);
			}
			return 0;

		}

		/*end of voice record*/
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONUP:
		break;

	case WM_CREATE: // main buttons
		mainButtons.init(hWnd);
		mainButtons.initButtons(lParam);
		recognition.init(hWnd);
		recognition.initWindow(lParam);
		mainButtons.initRecognitionTab(&recognition);
		recognition.initRecognitionVoiceWrite(&voicewrite);
		voicewrite.init(hWnd);
		voicewrite.initWindow(lParam);
		wavrec.init(hWnd);
		wavrec.initWindow(lParam);
		recognition.initWAVRecognition(&wavrec);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#else
#ifdef __unix__
//TODO realization for UNIX console
#include <stdio.h>

#include <math.h>

#include "FFTAnalysis.h"
#include "ReadEtalons.h"
#include "SpectrumAnalysis.h"
#include "RegrKoefAnalysis.h"

#include "InputParameters.h"

#include "FindingMaxKoef.h"

#include "PushButtonHandler.h"

#include "VoiceWrite.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

int main(void)
{
	///// flag for button handler 
	int flag = 0;
	/////

	//// default values for spectrum analysis
	int samples_count = 0;
	int Fs = 22050;   // default - ==header.nSamplesPerSec
	double overlap = 0.0;
	double NFFT = 512.0;
	char win[] = "hann";  // okno hanna, vtoroi variant s oknom hamminga
	char type[] = "hz";
	////

	//// tut znachenia takie, chto bu poluchit odinakovuy matricy s etalonom
	int Nfrb = 35;
	int Nfrm = 17;
	////

	//// tut parametry dlya etalonov i evaluate koef of regression
	int len = 0;
	int len_etalona = 0;
	int Nfrm_etalona = 17;
	int Nfrb_etalona = 35;
	int col_vect = 2; // skolko vectorov: (shum), etalon, sravn_zapis
	////

	//// for input files functions
	int setsize = 2;

	int wavfilescounter = 0;
	int efilescounter = 0;

	string strpath;
	string folderpath;
	string str;

	string estrpath;
	string efolderpath;
	string estr;
	////

	char** wavfiles = 0;
	char** etalons = 0;

	etalons = inputEtalons(setsize, efilescounter, estrpath, efolderpath, estr);

	cout << endl;
	cout << "Press Enter for voice recording or Space for choise folder with wav files" << endl;

	pushButtonHandler(flag);

	if (flag == 13)  // Enter button
	{
		double* wavdata;

		wavdata = VoiceWrite();

		cout << "Start computing... " << endl << endl;

		cout << "Wav-file is: voice record " << endl << endl;

		double **spectr;
		spectr = speval_eq(wavdata, Nfrm, overlap, Fs, Nfrb, win, type);  //my_function speval_eq s oknom hanna v hz

		double* koeffs = new double[efilescounter];

		//chtenie etalonov iz txt faila
		for (int j = 0; j < efilescounter; j++)
		{
			estrpath = efolderpath + etalons[j];

			const char* efullpath = estrpath.c_str();

			double* etalon = readDoubles((char*)efullpath, len);

			//////vityagivanie matrici spectralnogo analiza v vector
			double* lin_spectr;

			lin_spectr = matrINvect(spectr, Nfrm, Nfrb);

			//sravnenie faila s etalonami
			len_etalona = len;

			koeffs[j] = koef_of_regr(lin_spectr, etalon, 1, col_vect, len_etalona);

			///chistim stroky s putem k faily
			estrpath.clear();
			///

			/// ochistka pamyaty
			delete[] etalon;
			delete[] lin_spectr;
			///
		}

		maxKoef(etalons, koeffs, efilescounter);
		cout << endl;

		/////// ochistka pamyaty
		delete[] spectr;
		delete[] wavdata;
		delete[] koeffs;

		flag = 0;

		system("pause");
	}

	if (flag == 32)  // Space button
	{
		wavfiles = inputWavFile(setsize, wavfilescounter, strpath, folderpath, str);

		cout << "Start computing... " << endl << endl;

		for (int i = 0; i < wavfilescounter; i++) // i=0
		{
			strpath = folderpath + wavfiles[i];
			const char* fullpath = strpath.c_str();

			cout << "Wav-file is: " << wavfiles[i] << endl << endl;

			double* wavdata;

			wavdata = wavread(fullpath, samples_count);

			///chistim stroky s putem k faily
			strpath.clear();
			///

			//spectral analysis
			double **spectr;
			spectr = speval_eq(wavdata, Nfrm, overlap, Fs, Nfrb, win, type);  //my_function speval_eq s oknom hanna v hz

			double* koeffs = new double[efilescounter];

			//chtenie etalonov iz txt faila
			for (int j = 0; j < efilescounter; j++)
			{
				estrpath = efolderpath + etalons[j];

				const char* efullpath = estrpath.c_str();

				double* etalon = readDoubles((char*)efullpath, len);

				//////vityagivanie matrici spectralnogo analiza v vector
				double* lin_spectr;

				lin_spectr = matrINvect(spectr, Nfrm, Nfrb);

				//sravnenie faila s etalonami
				len_etalona = len;

				koeffs[j] = koef_of_regr(lin_spectr, etalon, 1, col_vect, len_etalona);

				///chistim stroky s putem k faily
				estrpath.clear();
				///

				/// ochistka pamyaty
				delete[] etalon;
				delete[] lin_spectr;
				///
			}

			maxKoef(etalons, koeffs, efilescounter);
			cout << endl;

			/////// ochistka pamyaty
			delete[] spectr;
			delete[] wavdata;
			delete[] koeffs;
		}

		flag = 0;

		system("pause");
	}

	return 0;
}
#else
#error Unsupported Implementation
#endif
#endif