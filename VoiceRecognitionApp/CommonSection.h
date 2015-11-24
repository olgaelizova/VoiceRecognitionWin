#pragma once

struct ControlPackage{

public:
	HWND		mControl;
	char *		mCtrlClass;
	char *		caption;
	int			visualAttr;
	int			top, left, bottom, right;
	HWND		mParent;
	HINSTANCE	mInst;

	//"Write voice for recognition", WS_VISIBLE | WS_CHILD | ES_LEFT |
	//	1, 300, 185, 200, 100, hwnd, (HMENU)EH_RECOGNIZEWRITEVOICE, ((LPCREATESTRUCT)lParam)->hInstance, NULL
};

typedef ControlPackage * PControlPackage;

class CommonSection
{

protected:

	ControlPackage	mCommonControlParams;
	PControlPackage	mControlPack;
	int				mControlCount;
	int				mCapacity;

public:

	void	InitControlPackageSize(int size);
	void	InitCommonControlParams(HWND parent, int visualAttr, HINSTANCE inst);
	void	AddControl(char * mCtrlClass, char* caption, int top, int left, int right, int bottom );
	virtual int		ProcessMsg(WPARAM wParam, LPARAM lParam) = 0;
	void	HideSection();
	void	ShowSection();

	virtual void Init(HWND parent, int visualAttr, HINSTANCE inst) = 0;

	CommonSection();
	~CommonSection();
};

