#include "stdafx.h"

#include "FindingMaxKoef.h"

using namespace std;

char* maxKoef(char** etalons, double* koeffs, int efilescounter, char* wavfiles, char * resultStr)
{

	int resultStrSize = 0;
	//


	int i=0;
	int etcounter = 0;
	double max = 0;

	char txtOut[1000] = "";

	char maxStr[10] = "";
	char spaceStr[2] = " ";

	for (i = 0; i < efilescounter; i++)
		if (koeffs[i]>max)
		{
			max = koeffs[i];
			etcounter = i;
		}
		
		sprintf(maxStr, "%f", max); // convert double to char

		// plus space
		resultStrSize += getLengthV(wavfiles) + 2;
		// for max string
		resultStrSize += 4;
		// plus space plus terminal null
		resultStrSize += getLengthV(etalons[etcounter]) + 2 + 2;

		resultStr = new char[resultStrSize];
		resultStr[0] = '\0';

		appendStringV(resultStr, wavfiles);
		appendStringV(resultStr, spaceStr);
		appendStringV(resultStr, etalons[etcounter]);
		appendStringV(resultStr, spaceStr);
		appendStringV(resultStr, maxStr);
		appendStringV(resultStr, spaceStr);

	//cout << "Etalon is " << etalons[etcounter] << " with koefficient " << max << endl;
		return resultStr;
}

void appendStringV(char* dst, char* src)
{
	int i = 0;
	int strPos = 0;

	while (dst[strPos] != '\0')
		strPos++;

	while (src[i] != '\0' && i != 1000)
	{
		dst[strPos + i] = src[i];
		i++;
	}

	dst[strPos + i] = '\0';

}

// returns position of the symbol "\0" minus one 1
int getLengthV(char * str)
{
	for (int i = 0;; i++)
	{
		if (i == 1000)
			return 0;

		if (str[i] == '\0')
			return i - 1;
	}

}