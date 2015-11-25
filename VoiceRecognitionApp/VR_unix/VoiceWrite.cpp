#include "stdafx.h"

#include "VoiceWrite.h"

#ifdef _WIN32

using namespace std;

const int NUMPTS = 22050 * 2 * 5;   // 5 seconds
int sampleRate = 22050;
short int waveIn[NUMPTS];

double* VoiceWrite()
{
	cout << "VoiceWrite function" << endl;
	/*
	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;     // simple, uncompressed format
	pFormat.nChannels = 2;                    //  1=mono, 2=stereo
	pFormat.wBitsPerSample = 16;              //  16 for high quality, 8 for telephone-grade
	pFormat.nSamplesPerSec = sampleRate;
	pFormat.nAvgBytesPerSec = sampleRate * pFormat.nChannels * pFormat.wBitsPerSample / 8;
	pFormat.nBlockAlign = pFormat.nChannels * pFormat.wBitsPerSample / 8;
	pFormat.cbSize = 0;

	HWAVEIN hWaveIn;
	WAVEHDR waveInHdr;

	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	waveInHdr.lpData = (LPSTR)waveIn;
	waveInHdr.dwBufferLength = NUMPTS * 2;
	waveInHdr.dwBytesRecorded = 0;
	waveInHdr.dwUser = 0L;
	waveInHdr.dwFlags = 0L;
	waveInHdr.dwLoops = 0L;

	waveInPrepareHeader(hWaveIn, &waveInHdr, sizeof(WAVEHDR));

	waveInAddBuffer(hWaveIn, &waveInHdr, sizeof(WAVEHDR));

	waveInStart(hWaveIn);

	// Wait until finished recording
	do{} while (waveInUnprepareHeader(hWaveIn, &waveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);

	waveInClose(hWaveIn);

	HWAVEOUT hWaveOut;
	WAVEHDR WaveOutHdr;

	waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	WaveOutHdr.lpData = (LPSTR)waveIn;
	WaveOutHdr.dwBufferLength = NUMPTS * 2;
	WaveOutHdr.dwBytesRecorded = 0;
	WaveOutHdr.dwUser = 0L;
	WaveOutHdr.dwFlags = 0L;
	WaveOutHdr.dwLoops = 1L;

	waveOutPrepareHeader(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));

	waveOutSetVolume(hWaveOut, 0xFFFFFFFF);

	waveOutWrite(hWaveOut, &WaveOutHdr, sizeof(WAVEHDR));

	waveOutClose(hWaveOut);

	system("pause");
	*/
	return 0;
}

#else
#ifdef __unix__
//TODO realization for UNIX
double* VoiceWrite()
{

}
#else
#error Unsupported Implementation
#endif
#endif