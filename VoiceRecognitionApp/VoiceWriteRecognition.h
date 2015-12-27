#pragma once

#include <mmsystem.h>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

#include <windows.h>
#pragma comment(lib,"winmm.lib") 

#define _CRT_SECURE_NO_WARNINGS

#define EH_RECORD		0x1010
#define EH_SAVE			0x1011
#define EH_STOP			0x1012
#define EH_RECOGNIZEBTN	0x1013
#define EH_PLAY			0x1014

class VoiceWriteRecognition
{

public:

	class SoundPictureWrapper
	{

	public :

		int		capacity, size;
		int *	xPoints;
		int *	yPoints;

		SoundPictureWrapper(int c)
		{
			this->capacity = c;
			this->size = 0;

			xPoints = new int[c];
			yPoints = new int[c];
		}

		void appendSoundPic(int * x, int * y, int size)
		{
			if (capacity > size + this->size)
			{
				for (int i = 0; i < this->size; i++)
				{
					xPoints[i+ this->size] = x[i];
					yPoints[i + this->size] = y[i];
				}

				this->size += size;

				return;
			}
			else
			{
				// TODO:
				return;
			}
		}
	};

	VoiceWriteRecognition();
	~VoiceWriteRecognition();

	HWND buttonRecord;
	HWND buttonSave;
	HWND buttonStop;
	HWND buttonRecognize;
	HWND buttonPlay;

	HWND hwnd;

	void init(HWND hwnd);
	//{
	//	this->hwnd = hwnd;
	//}

	void initWindow(LPARAM);

	int ProcessMsg(int msg);

	int newWindow();

	int Record();
	int Save();
	int Stop();
	int Play();
	int StopTempSave();
	int Recognize();

	void parseSoundPiece();

	// parameters for record //WHY????!!!!!
	#define RATE 11025   //22050
	#define BYTERATE 8   //16

	//Globals for sound wave visualistion
	int number, length, byte_samp, byte_sec, bit_samp;
	bool mono				= TRUE;
	bool PLAY				= FALSE;
	bool isBeingRecorded	= FALSE;
	int		currentPicturePoint;
	bool blackScreenAvailable;

	//SoundPictureWrapper
	SoundPictureWrapper spw;

	//errno_t wavfile;
	char * filename;
	int s_rate = RATE;
	double limit = 5000.0;
	FILE * stream;

	/* Declare procedures */
	int readSample(int number, bool leftchannel);
	int readSampleFromBuffer(int number, bool leftchannel);
	void Wav(char *c);
	void ResetPictureBuffer();

	/*declare descriptors*/
	#define NUM 20000

	// 16384
	#define INP_BUFFER_SIZE 16384 / 64
	#define INP_BUFFER_SIZE_RECORD 16384 / 64

	HPEN hPen;
	BOOL         bRecording, bPlaying, bEnding, bTerminating;
	DWORD        dwDataLength, dwRepetitions = 1;
	HWAVEIN      hWaveIn;
	HWAVEOUT     hWaveOut;
	PBYTE        pBuffer1, pBuffer2, pSaveBuffer, pNewBuffer;
	PWAVEHDR     pWaveHdr1, pWaveHdr2;
	WAVEFORMATEX waveform;
	int sampleRate = RATE;
	const int NUMPTS = RATE * 10;
	int currentSaveBufferPos;
	HDC hDC;
	POINT pt[NUM];
	POINT ptRecord[NUM];


};

