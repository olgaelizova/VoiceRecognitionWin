#include "stdafx.h"
#include "WAVRecognition.h"
#include "FindingMaxKoef.h"

WAVRecognition::WAVRecognition()
{
}


WAVRecognition::~WAVRecognition()
{
}

void WAVRecognition::initWindow(LPARAM lParam)
{
	result = CreateWindowA("ListBox", "Recognition Results", WS_VISIBLE | WS_CHILD | ES_LEFT |
			1, 100, 100, 600, 300, hwnd, (HMENU)EH_RESULT, ((LPCREATESTRUCT)lParam)->hInstance, NULL); 

	ShowWindow(result, 0);

}

int WAVRecognition::ProcessMsg(int msg)
{
	switch (msg)
	{
	case EH_RESULT:
		break;

	default:
		return 0;
	}
}

int WAVRecognition::newWindow()
{
	ShowWindow(result, 1);
	showResult(result);
	return 1;
}

int showResult(HWND result)
{
	//double maxResult = 0;
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

			wavfiles = inputWavFile(setsize, wavfilescounter, strpath, folderpath, str);

			//cout << "Start computing... " << endl << endl;

			char* txtOut = 0;

			for (int i = 0; i < wavfilescounter; i++) // i=0
			{
				strpath = folderpath + wavfiles[i];
				const char* fullpath = strpath.c_str();

				//SetWindowText(result, TEXT("Control string"));

				//cout << "Wav-file is: " << wavfiles[i] << endl << endl;

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

				//TODO: return char from function

				//maxResult = maxKoef(etalons, koeffs, efilescounter);

				txtOut = maxKoef(etalons, koeffs, efilescounter, wavfiles[i], txtOut);

				//sprintf(txtOut, "%f", maxResult); // convert double to char
				//char out[5] = "test";

				SendMessage(result, LB_ADDSTRING, 0, (LPARAM)(LPSTR)txtOut); //set strings into listbox
				//SendMessage(result, LB_ADDSTRING, 0, (LPARAM)(LPSTR)out); //set strings into listbox

				//SetWindowText(result, txtOut);
				//cout << endl;

				/////// ochistka pamyaty
				delete[] spectr;
				delete[] wavdata;
				delete[] koeffs;
			}

			flag = 0;

	return 1;
}