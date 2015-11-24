#include "stdafx.h"
#include "CommonSection.h"


CommonSection::CommonSection()
{
}


CommonSection::~CommonSection()
{
}

void CommonSection::InitCommonControlParams(HWND parent, int visualAttr, HINSTANCE inst)
{
	mCommonControlParams.mParent		= parent;
	mCommonControlParams.visualAttr	= visualAttr;
	mCommonControlParams.mInst		= inst;
}



void CommonSection::AddControl(char * mCtrlClass, char* caption, int top, int left, int right, int bottom)
{
	if (mControlCount >= mCapacity)
		return;

	// fill in common parameters
	mControlPack[mControlCount].mInst		= mCommonControlParams.mInst;
	mControlPack[mControlCount].mParent		= mCommonControlParams.mParent;
	mControlPack[mControlCount].visualAttr	= mCommonControlParams.visualAttr;

	// fill in custom parameters
	mControlPack[mControlCount].mCtrlClass	= mCtrlClass;
	mControlPack[mControlCount].caption		= caption;
	mControlPack[mControlCount].top			= top;
	mControlPack[mControlCount].left		= left;
	mControlPack[mControlCount].right		= right;
	mControlPack[mControlCount].bottom		= bottom;

	// window creation
	mControlPack[mControlCount].mControl = CreateWindow(mCtrlClass, caption, mCommonControlParams.visualAttr,
		left, top, right, bottom, mCommonControlParams.mParent, NULL, mCommonControlParams.mInst, NULL);

	// increment for size
	mControlCount++;
}

void CommonSection::InitControlPackageSize(int size)
{
	mControlPack		= new ControlPackage[size];
	mCapacity		= size;
	mControlCount	= 0;
}

void CommonSection::ShowSection()
{
	for (int i = 0; i < mControlCount; i++)
	{
		ShowWindow(mControlPack[i].mControl, 1);
	}
}

void CommonSection::HideSection()
{
	for (int i = 0; i < mControlCount; i++)
	{
		ShowWindow(mControlPack[i].mControl, 0);
	}
}