#pragma once

//#include "stdafx.h"

#include <stdio.h>
#include <iostream>
//using namespace std;

char* maxKoef(char** etalons, double* koeffs, int efilescounter, char* wavfile, char* resultStr);

// returns position of the symbol "\0" minus one 1
int getLengthV(char * str);

void appendStringV(char* dst, char* src);