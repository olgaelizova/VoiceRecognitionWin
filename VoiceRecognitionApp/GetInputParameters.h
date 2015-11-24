#pragma once
class GetInputParameters
{
public:
	GetInputParameters();
	~GetInputParameters();

	int  Frequency;
	char window[30];
	char FreqType[30];
	// TO DO : other parameters
};

extern GetInputParameters getParams; //global
