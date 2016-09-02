#pragma once

#include "stdafx.h"

#define INPUT_A 1
#define INPUT_D 2
#define BUTTON_A 3
#define BUTTON_D 4
#define BUTTON_WND 5
#define SLIDE_H 6
#define SLIDE_V 7
#define TREND_LEFT 8
#define TREND_RIGHT 9
#define TREND_TIME 10
#define TREND_VALUE 11

struct CDny{
	bool m_Used;
	CString m_TagLable;
	CString m_TagName;
	struct pmc_value_t m_TagValue;
	void * action_proc;
	/* Mar 2005, prepare for script support */
#if 0
	char * script;
	__u8 * compiledScript;
#endif
	CDny()
	{
		action_proc = 0;
#if 0
		script=0;
		compiledScript=0;
#endif
	}
};

struct CDnyInputD:CDny{
	CString m_OnMsg;
	CString m_OffMsg;
	CString m_SetPrm;
	CString m_ResetPrm;
	CString m_WndPrm;
};

struct CDnyInputA:CDny{
	float m_MinVal;
	float m_MaxVal;
	CString m_WndPrm;
};

struct CDnySize:CDny{
	int m_Ref;
	float m_LTO;
	float m_LTT;
	float m_RBO;
	float m_RBT;
};

struct CDnyButtonD:CDny{
	int m_Action;
	int m_pulseWidth;
};

struct CDnyButtonA:CDny{
	int m_Fuction;
	float m_UpLimit;
	float m_DownLimit;
	float m_Delta;
};

struct CDnyButtonWnd:CDny{
	int m_Mode;
};

struct CDnyValueD:CDny{
	CString m_OnMsg;
	CString m_OffMsg;
};

struct CDnyValueA:CDny{
	CString m_Format;
};

struct CDnyColorD:CDny{
	COLORREF m_ColorOn;
	COLORREF m_ColorOff;
};

struct CDnyColorDA:CDny{
	COLORREF m_ColorNomal;
	COLORREF m_ColorAlarm;
};

struct CDnyColorA:CDny{
	float m_Split1;
	float m_Split2;
	float m_Split3;
	float m_Split4;
	COLORREF m_Color0;
	COLORREF m_Color1;
	COLORREF m_Color2;
	COLORREF m_Color3;
	COLORREF m_Color4;
};

struct CDnyColorAA:CDny{
	COLORREF m_ColorL2;
	COLORREF m_ColorL1;
	COLORREF m_ColorN;
	COLORREF m_ColorH1;
	COLORREF m_ColorH2;
	COLORREF m_ColorR;
};

struct CDnyVisible:CDny{
	bool m_Visble;
};

struct CDnyEnable:CDny{
	bool m_Enable;
};

struct CDnyBlink:CDny{
	bool m_VisibleAtOn;
	int m_BlinkSpeed;
	COLORREF m_ColorPen;
	COLORREF m_ColorBrush;
	COLORREF m_ColorText;
};

struct CDnyDirection:CDny{
	float m_MinClockO;
	float m_MaxClockO;
	float m_MinClockT;
	float m_MaxClockT;
	float m_XBias;
	float m_YBias;
};

//动态属性
class CDnyAttr{
public:
	long m_rank;
	bool m_vi;
	bool m_en;
	bool m_na;
	long m_push;
	void TagToLab();
	void Load(MSXML::IXMLDOMElementPtr pDynamic);
	void Save(MSXML::IXMLDOMDocumentPtr pDoc,MSXML::IXMLDOMElementPtr pDynamic);
	CDnyAttr();
	virtual ~CDnyAttr();
	//链接了动态属性
	bool m_Linked;
	//触动链接
	CDnyInputD m_InputD;
	CDnyInputA m_InputA;
	CDnySize m_SlideH;
	CDnySize m_SlideV;
	CDnyButtonD m_ButtonD;
	CDnyButtonA m_ButtonA;
	CDnyButtonWnd m_ButtonWnd;
	BOOL m_ClsPop;
	//显示链接
	CDnyValueD m_ValueD;
	CDnyValueA m_ValueA;
	CDnyColorD m_ColorPenD;
	CDnyColorA m_ColorPenA;
	CDnyColorDA m_ColorPenDA;
	CDnyColorAA m_ColorPenAA;
	CDnyColorD m_ColorBrushD;
	CDnyColorA m_ColorBrushA;
	CDnyColorDA m_ColorBrushDA;
	CDnyColorAA m_ColorBrushAA;
	CDnyColorD m_ColorTextD;
	CDnyColorA m_ColorTextA;
	CDnyColorDA m_ColorTextDA;
	CDnyColorAA m_ColorTextAA;
	CDnySize m_SizeH;
	CDnySize m_SizeV;
	CDnySize m_PosH;
	CDnySize m_PosV;
	CDnySize m_FillH;
	CDnySize m_FillV;
	CDnyVisible m_Visible;
	CDnyBlink m_Blink;
	CDnyDirection m_Direction;
	CDnyEnable m_Enable;
};
