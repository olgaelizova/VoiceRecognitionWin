#include "stdafx.h"
#include "WavReader.h"

wav_header_t	header;
chunk_t			chunk;

double* wavread(const char* fullpath,int &samples_count)
{
	FILE* f = 0;

	f = fopen(fullpath, "rb"); // wavfile[i]

	if (f == NULL)
	{
		printf("File no open\n");
		system("pause");
		return 0;
	}

	// schitaem razmer faila
	int fileSize = 0;
	fseek(f, 0, SEEK_END);  // smeshaet kursor ot 0 do konza faila
	fileSize = ftell(f);	// vozvrash. tekushyu poziciu v potoke
	fseek(f, 0, SEEK_SET); // vozvrashaem ukazatel na nachalo faila
	//
	fread(&header, sizeof(header), 1, f); // chitaem inf v structuru header

	//// proverca na format PCM
	if (header.wFormatTag != 1)
		printf("Not PCM format\n");
	fseek(f, header.fLen - 16, SEEK_CUR); //skip wExtraFormatBytes & extra format bytes
	while (true) //go to data chunk //tut propuskaem polya LIST i INFO
	{
		fread(&chunk, sizeof(chunk), 1, f);
		if (*(IDWORD *)&chunk.id == 0x61746164) break;
		fseek(f, chunk.size, SEEK_CUR); //skip chunk data bytes
	}

	// schitaem chislo samplov 
	int sample_size = header.wBitsPerSample / 8;
	samples_count = chunk.size * 8 / header.wBitsPerSample;

	// pamyat pod massiv znacheniy iz data 
	IDWORD *value = new IDWORD[samples_count];
	memset(value, 0, sizeof(IDWORD)* samples_count);  // dlya nee nygna memory.h

	for (int i = 0; i < samples_count; i++)
	{
		fread(&value[i], sample_size, 1, f);  // poluchili massiv
	}

	fclose(f);

	// poluchim massiv (double) nashix znachenyi
	double* data_chunk = new double[samples_count];

	for (int i = 0; i < samples_count; i++)
	{
		//for 16 bits per sample only: delim na 0x8000 chto ravno 32768.0;
		double x = (double)(IINT16)value[i] / 32768.0;
		data_chunk[i] = x;
		//cout << data_chunk[i] << endl;
	}

	delete[] value;  // ochistka pamyaty

	return data_chunk;
}