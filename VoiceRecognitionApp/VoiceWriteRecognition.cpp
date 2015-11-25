#include "stdafx.h"
#include "VoiceWriteRecognition.h"


VoiceWriteRecognition::VoiceWriteRecognition()
{
}


VoiceWriteRecognition::~VoiceWriteRecognition()
{
}

void VoiceWriteRecognition::init(HWND hwnd)
{
	this->hwnd = hwnd;

	pWaveHdr1 = reinterpret_cast <PWAVEHDR> (malloc(sizeof(WAVEHDR)));
	pWaveHdr2 = reinterpret_cast <PWAVEHDR> (malloc(sizeof(WAVEHDR)));

	// Allocate memory for save buffer
	pSaveBuffer = reinterpret_cast <PBYTE> (malloc(1));
}

void VoiceWriteRecognition::initWindow(LPARAM lParam)
{
	buttonRecord = CreateWindowA("button", "Record", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 50, 450, 100, 50, hwnd, (HMENU)EH_RECORD, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	buttonSave = CreateWindowA("button", "Save", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 200, 450, 100, 50, hwnd, (HMENU)EH_SAVE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	buttonStop = CreateWindowA("button", "Stop", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 50, 450, 100, 50, hwnd, (HMENU)EH_STOP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	buttonRecognize = CreateWindowA("button", "Recognize", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 350, 450, 100, 50, hwnd, (HMENU)EH_RECOGNIZEBTN, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	buttonPlay = CreateWindowA("button", "Play", WS_VISIBLE | WS_CHILD | ES_LEFT |
		1, 550, 450, 100, 50, hwnd, (HMENU)EH_PLAY, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

	ShowWindow(buttonRecord, 0);
	ShowWindow(buttonSave, 0);
	ShowWindow(buttonStop, 0);
	ShowWindow(buttonPlay, 0);
	ShowWindow(buttonRecognize, 0);

}

int VoiceWriteRecognition::ProcessMsg(int msg)
{
	switch (msg)
	{
	case EH_RECORD:
		return Record();
		break;

	case EH_SAVE:
		return Save();
		break;

	case EH_STOP:
		return Stop();
		break;

	case EH_PLAY:
		return Play();
		break;

	case EH_RECOGNIZEBTN:
		return Recognize();
		break;

	default:
		return 0;
	}
}

int VoiceWriteRecognition::newWindow()
{
	ShowWindow(buttonRecord, 1);
	ShowWindow(buttonSave, 1);
	ShowWindow(buttonRecognize, 1);
	ShowWindow(buttonPlay, 1);

	EnableWindow(buttonRecord, TRUE);
	EnableWindow(buttonSave, FALSE);
	EnableWindow(buttonRecognize, FALSE);
	EnableWindow(buttonPlay, FALSE);
	EnableWindow(buttonStop, FALSE);

	return 1; 
}

int VoiceWriteRecognition::Record()
{
	ShowWindow(buttonStop, 1);
	ShowWindow(buttonRecord, 0);

	EnableWindow(buttonRecord, FALSE); // knopky nelz nazat
	EnableWindow(buttonSave, FALSE);
	EnableWindow(buttonRecognize, FALSE);
	EnableWindow(buttonPlay, FALSE);
	EnableWindow(buttonStop, TRUE);  // knopky mozno nazat

	pBuffer1 = reinterpret_cast <PBYTE> (malloc(INP_BUFFER_SIZE));
	pBuffer2 = reinterpret_cast <PBYTE> (malloc(INP_BUFFER_SIZE));

	if (!pBuffer1 || !pBuffer2)
	{
		if (pBuffer1) free(pBuffer1);
		if (pBuffer2) free(pBuffer2);
		//MessageBox(hwnd, szMemError, szAppName,
		//	MB_ICONEXCLAMATION | MB_OK);
		return TRUE;
	}
	// Open waveform audio for input

	waveform.wFormatTag = WAVE_FORMAT_PCM;
	waveform.nChannels = 1;
	waveform.nSamplesPerSec = RATE;
	waveform.nAvgBytesPerSec = RATE;
	waveform.nBlockAlign = 1;
	waveform.wBitsPerSample = BYTERATE;
	waveform.cbSize = 0;

	if (waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,
		(DWORD)hwnd, 0, CALLBACK_WINDOW))
	{
		free(pBuffer1);
		free(pBuffer2);
	}
	// Set up headers and prepare them

	pWaveHdr1->lpData = reinterpret_cast <CHAR*>(pBuffer1);
	pWaveHdr1->dwBufferLength = INP_BUFFER_SIZE;
	pWaveHdr1->dwBytesRecorded = 0;
	pWaveHdr1->dwUser = 0;
	pWaveHdr1->dwFlags = 0;
	pWaveHdr1->dwLoops = 1;
	pWaveHdr1->lpNext = NULL;
	pWaveHdr1->reserved = 0;

	waveInPrepareHeader(hWaveIn, pWaveHdr1, sizeof(WAVEHDR));

	pWaveHdr2->lpData = reinterpret_cast <CHAR*>(pBuffer2);
	pWaveHdr2->dwBufferLength = INP_BUFFER_SIZE;
	pWaveHdr2->dwBytesRecorded = 0;
	pWaveHdr2->dwUser = 0;
	pWaveHdr2->dwFlags = 0;
	pWaveHdr2->dwLoops = 1;
	pWaveHdr2->lpNext = NULL;
	pWaveHdr2->reserved = 0;

	waveInPrepareHeader(hWaveIn, pWaveHdr2, sizeof(WAVEHDR));

	return 1;
}

int VoiceWriteRecognition::Stop()
{
	ShowWindow(buttonStop, 0);
	ShowWindow(buttonRecord, 1);

	EnableWindow(buttonRecord, TRUE);
	EnableWindow(buttonSave, TRUE);
	EnableWindow(buttonRecognize, TRUE);
	EnableWindow(buttonPlay, TRUE);
	EnableWindow(buttonStop, FALSE);

	// Reset input to return last buffer
	bEnding = TRUE;

	//save wav file to temp.wav
	int chunksize, pcmsize, NumSamples, subchunk1size;
	int audioFormat = 1;
	int numChannels = 1;
	int bitsPerSample = BYTERATE;

	NumSamples = ((long)(NUMPTS / sampleRate) * 1000);
	pcmsize = sizeof(PCMWAVEFORMAT);

	subchunk1size = 16;
	int byteRate = (sampleRate*numChannels)*(bitsPerSample / 8);
	int blockAlign = numChannels*bitsPerSample / 8;
	int subchunk2size = pWaveHdr1->dwBufferLength*numChannels;

	chunksize = (36 + subchunk2size);
	fstream myFile("temp.wav", ios::out | ios::binary);

	// write the wav file per the wav file format
	myFile.seekp(0, ios::beg);
	myFile.write("RIFF", 4);						// chunk id
	myFile.write((char*)&chunksize, 4);	        	// chunk size (36 + SubChunk2Size))
	myFile.write("WAVE", 4);						// format
	myFile.write("fmt ", 4);						// subchunk1ID
	myFile.write((char*)&subchunk1size, 4);			// subchunk1size (16 for PCM)
	myFile.write((char*)&audioFormat, 2);			// AudioFormat (1 for PCM)
	myFile.write((char*)&numChannels, 2);			// NumChannels
	myFile.write((char*)&sampleRate, 4);			// sample rate
	myFile.write((char*)&byteRate, 4);				// byte rate (SampleRate * NumChannels * BitsPerSample/8)
	myFile.write((char*)&blockAlign, 2);			// block align (NumChannels * BitsPerSample/8)
	myFile.write((char*)&bitsPerSample, 2);			// bits per sample
	myFile.write("data", 4);						// subchunk2ID
	myFile.write((char*)&subchunk2size, 4);			// subchunk2size (NumSamples * NumChannels * BitsPerSample/8)

	myFile.write(pWaveHdr1->lpData, pWaveHdr1->dwBufferLength);	// data

	return 1;
}

int VoiceWriteRecognition::Play()
{
	EnableWindow(buttonRecord, FALSE);
	EnableWindow(buttonSave, TRUE);
	EnableWindow(buttonRecognize, FALSE);
	EnableWindow(buttonPlay, TRUE);

	bPlaying = TRUE;

	EnableWindow(buttonPlay, FALSE);
	waveform.wFormatTag = WAVE_FORMAT_PCM;
	waveform.nChannels = 1;
	waveform.nSamplesPerSec = RATE;
	waveform.nAvgBytesPerSec = RATE;
	waveform.nBlockAlign = 1;
	waveform.wBitsPerSample = BYTERATE;
	waveform.cbSize = 0;
	waveInReset(hWaveIn);

	if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveform,
		(DWORD)hwnd, 0, CALLBACK_WINDOW))
	{
		//MessageBox(hwnd, szOpenError, szAppName,
		//	MB_ICONEXCLAMATION | MB_OK);
	}

	Wav("temp.wav");
	// Open waveform audio for output

	RECT rc;
	GetClientRect(hwnd, &rc);
	PLAY = TRUE;
	InvalidateRect(hwnd, &rc, TRUE);

	return 1;
}

int VoiceWriteRecognition::Save()
{

	int chunksize, pcmsize, NumSamples, subchunk1size;
	int audioFormat = 1;
	int numChannels = 1;
	int bitsPerSample = BYTERATE;

	NumSamples = ((long)(NUMPTS / sampleRate) * 1000);
	pcmsize = sizeof(PCMWAVEFORMAT);

	;
	subchunk1size = 16;
	int byteRate = sampleRate*numChannels*bitsPerSample / 8;
	int blockAlign = numChannels*bitsPerSample / 8;
	int subchunk2size = pWaveHdr1->dwBufferLength*numChannels;

	chunksize = (36 + subchunk2size);
	fstream myFile("test.wav", ios::out | ios::binary);

	// write the wav file per the wav file format
	myFile.seekp(0, ios::beg);
	myFile.write("RIFF", 4);					// chunk id
	myFile.write((char*)&chunksize, 4);	        	// chunk size (36 + SubChunk2Size))
	myFile.write("WAVE", 4);					// format
	myFile.write("fmt ", 4);					// subchunk1ID
	myFile.write((char*)&subchunk1size, 4);			// subchunk1size (16 for PCM)
	myFile.write((char*)&audioFormat, 2);			// AudioFormat (1 for PCM)
	myFile.write((char*)&numChannels, 2);			// NumChannels
	myFile.write((char*)&sampleRate, 4);			// sample rate
	myFile.write((char*)&byteRate, 4);			// byte rate (SampleRate * NumChannels * BitsPerSample/8)
	myFile.write((char*)&blockAlign, 2);			// block align (NumChannels * BitsPerSample/8)
	myFile.write((char*)&bitsPerSample, 2);			// bits per sample
	myFile.write("data", 4);					// subchunk2ID
	myFile.write((char*)&subchunk2size, 4);			// subchunk2size (NumSamples * NumChannels * BitsPerSample/8)

	myFile.write(pWaveHdr1->lpData, pWaveHdr1->dwBufferLength);	// data

	myFile.close();

	//TO DO: Realize buffer for automatic recognition

/*	unsigned int* buf = new unsigned int[10];
	fstream myFile2("test.wav", ios::in | ios::binary);
	myFile2.seekp(45, ios::beg);
	myFile2.read((char*)buf, 80);
	// myFile2.close();

	double bbuf = ((double)(short)buf) / 32768;
*/

	return 1;
}

int VoiceWriteRecognition::Recognize()
{
	return 1;
}

void VoiceWriteRecognition::Wav(char *c)
{
	// Read the temporary wav file
	filename = new char[strlen(c) + 1];
	strcpy(filename, c);
	// open filepointer readonly
	stream = fopen(filename, "r");
	if (stream == NULL)
	{
		cout << "Could not open " << filename << endl;
	}
	else
	{
		// declare a char buff to store some values in
		char *buff = new char[5];
		buff[4] = '\0';
		// read the first 4 bytes
		fread((void *)buff, 1, 4, stream);
		// the first four bytes should be 'RIFF'
		if (strcmp((char *)buff, "RIFF") == 0)
		{

			// read byte 8,9,10 and 11
			fseek(stream, 4, SEEK_CUR);
			fread((void *)buff, 1, 4, stream);
			// this should read "WAVE"
			if (strcmp((char *)buff, "WAVE") == 0)
			{
				// read byte 12,13,14,15
				fread((void *)buff, 1, 4, stream);
				// this should read "fmt "
				if (strcmp((char *)buff, "fmt ") == 0)
				{
					fseek(stream, 20, SEEK_CUR);
					// final one read byte 36,37,38,39
					fread((void *)buff, 1, 4, stream);
					if (strcmp((char *)buff, "data") == 0)
					{


						// Now we know it is a wav file, rewind the stream
						rewind(stream);
						// now is it mono or stereo ?
						fseek(stream, 22, SEEK_CUR);
						fread((void *)buff, 1, 2, stream);
						if (buff[0] == 0x02)
						{
							mono = false;
						}
						else
						{
							mono = true;
						}
						// read the sample rate
						fread((void *)&s_rate, 1, 4, stream);
						fread((void *)&byte_sec, 1, 4, stream);
						byte_samp = 0;
						fread((void *)&byte_samp, 1, 2, stream);
						bit_samp = 0;
						fread((void *)&bit_samp, 1, 2, stream);
						fseek(stream, 4, SEEK_CUR);
						fread((void *)&length, 1, 4, stream);
					}
				}
			}
		}
		delete buff;

	}
}

int VoiceWriteRecognition::readSample(int number, bool leftchannel)
{
	/*
	Reads sample number, returns it as an int, if
	this.mono==false we look at the leftchannel bool
	to determine which to return.

	number is in the range [0,length/byte_samp]

	returns 0xefffffff on failure
	*/

	if (number >= 0 && number<length / byte_samp)
	{
		// go to beginning of the file
		rewind(stream);

		// we start reading at sample_number * sample_size + header length
		int offset = number * 1 + 44;

		// unless this is a stereo file and the rightchannel is requested.
		//if (!mono && !leftchannel)
		//{
		// offset += byte_samp/2;
		//  }

		// read this many bytes;
		int amount;
		amount = byte_samp;

		fseek(stream, offset, SEEK_CUR);
		short sample = 0;
		fread((void *)&sample, 1, amount, stream);

		return sample;
	}
	else
	{
		// return 0xefffffff if failed
		return (int)0xefffffff;
	}
}