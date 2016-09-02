#pragma once

#include <windef.h>
#include <afx.h>

struct PMCUI_API CDny{
	bool m_Used;
	CString m_TagLable;
	CString m_TagName;
};

struct PMCUI_API CDnyInputD:CDny{
	CString m_OnMsg;
	CString m_OffMsg;
	CString m_SetPrm;
	CString m_ResetPrm;
	CString m_WndPrm;
};

struct PMCUI_API CDnyInputA:CDny{
	float m_MinVal;
	float m_MaxVal;
	CString m_WndPrm;
};

struct PMCUI_API CDnySize:CDny{
	int m_Ref;
	float m_LTO;
	float m_LTT;
	float m_RBO;
	float m_RBT;
};

struct PMCUI_API CDnyButtonD:CDny{
	int m_Action;
	int m_pulseWidth;
};

struct PMCUI_API CDnyButtonA:CDny{
	int m_Fuction;
	float m_UpLimit;
	float m_DownLimit;
	float m_Delta;
};

struct PMCUI_API CDnyButtonWnd:CDny{
	int m_Mode;
};

struct PMCUI_API CDnyValueD:CDny{
	CString m_OnMsg;
	CString m_OffMsg;
};

struct PMCUI_API CDnyValueA:CDny{
	CString m_Format;
};

struct PMCUI_API CDnyColorD:CDny{
	COLORREF m_ColorOn;
	COLORREF m_ColorOff;
};

struct PMCUI_API CDnyColorDA:CDny{
	COLORREF m_ColorNomal;
	COLORREF m_ColorAlarm;
};

struct PMCUI_API CDnyColorA:CDny{
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

struct PMCUI_API CDnyColorAA:CDny{
	COLORREF m_ColorL2;
	COLORREF m_ColorL1;
	COLORREF m_ColorN;
	COLORREF m_ColorH1;
	COLORREF m_ColorH2;
	COLORREF m_ColorR;
};

struct PMCUI_API CDnyVisible:CDny{
	bool m_Visble;
};

struct PMCUI_API CDnyEnable:CDny{
	bool m_Enable;
};

struct PMCUI_API CDnyBlink:CDny{
	bool m_VisibleAtOn;
	int m_BlinkSpeed;
	COLORREF m_ColorPen;
	COLORREF m_ColorBrush;
	COLORREF m_ColorText;
};

struct PMCUI_API CDnyDirection:CDny{
	float m_MinClockO;
	float m_MaxClockO;
	float m_MinClockT;
	float m_MaxClockT;
	float m_XBias;
	float m_YBias;
};

//动态属性
class PMCUI_API CDnyAttr{
public:
	long m_Rank;
	void TagToLab();
	void Load(MSXML::IXMLDOMElementPtr pDynamic);
	void Save(MSXML::IXMLDOMDocumentPtr pDoc,MSXML::IXMLDOMElementPtr pDynamic);
	CDnyAttr();
	virtual ~CDnyAttr();
	//链接了动态属性
	BOOL m_Linked;
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
