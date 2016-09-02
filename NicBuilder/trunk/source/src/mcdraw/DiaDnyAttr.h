#if !defined(AFX_DIADNYATTR_H__81A5D002_54E1_11D5_A636_00106075B53B__INCLUDED_)
#define AFX_DIADNYATTR_H__81A5D002_54E1_11D5_A636_00106075B53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaDnyAttr.h : header file
//

#include "diablink.h"
#include "diabuttona.h"
#include "diabuttond.h"
#include "diabuttonwnd.h"
#include "diacolora.h"
#include "diacoloraa.h"
#include "diacolord.h"
#include "diacolorda.h"
#include "diadirection.h"
#include "diaenable.h"
#include "diafillh.h"
#include "diafillv.h"
#include "diainputa.h"
#include "diainputd.h"
#include "diaposh.h"
#include "diaposv.h"
#include "diasizeh.h"
#include "diasizev.h"
#include "diaslideh.h"
#include "diaslidv.h"
#include "diavaluea.h"
#include "diavalued.h"
#include "diavisibility.h"
#include <pmc_ui.h>

/////////////////////////////////////////////////////////////////////////////
// CDiaDnyAttr dialog

class CDiaDnyAttr : public CDialog
{
// Construction
public:
	void IniShow();
	CDnyAttr m_DnyAttr;
	CDiaDnyAttr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiaDnyAttr)
	enum { IDD = IDD_DNYATTR };
	BOOL	m_Blink;
	BOOL	m_BrushColorA;
	BOOL	m_BrushColorAA;
	BOOL	m_BrushColorD;
	BOOL	m_BrushColorDA;
	BOOL	m_ButtonA;
	BOOL	m_ButtonD;
	BOOL	m_ButtonWnd;
	BOOL	m_Direction;
	BOOL	m_Disable;
	BOOL	m_FillH;
	BOOL	m_FillV;
	BOOL	m_InputA;
	BOOL	m_InputD;
	BOOL	m_PenColorA;
	BOOL	m_PenColorAA;
	BOOL	m_PenColorD;
	BOOL	m_PenColorDA;
	BOOL	m_PosH;
	BOOL	m_PosV;
	BOOL	m_SizeH;
	BOOL	m_SizeV;
	BOOL	m_SlideH;
	BOOL	m_SlideV;
	BOOL	m_TextColorA;
	BOOL	m_TextColorAA;
	BOOL	m_TextColorD;
	BOOL	m_TextColorDA;
	BOOL	m_ValueA;
	BOOL	m_ValueD;
	BOOL	m_Visible;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaDnyAttr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaDnyAttr)
	afx_msg void OnBlink();
	afx_msg void OnBrushcolorA();
	afx_msg void OnBrushcolorAa();
	afx_msg void OnBrushcolorD();
	afx_msg void OnBrushcolorDa();
	afx_msg void OnButtonA();
	afx_msg void OnButtonD();
	afx_msg void OnButtonWnd();
	afx_msg void OnDirection();
	afx_msg void OnDisable();
	afx_msg void OnFillH();
	afx_msg void OnFillV();
	afx_msg void OnInputA();
	afx_msg void OnPencolorA();
	afx_msg void OnPencolorAa();
	afx_msg void OnPencolorD();
	afx_msg void OnPencolorDa();
	afx_msg void OnPosH();
	afx_msg void OnPosV();
	afx_msg void OnSizeH();
	afx_msg void OnSizeV();
	afx_msg void OnSlideH();
	afx_msg void OnSlideV();
	afx_msg void OnTextcolorA();
	afx_msg void OnTextcolorAa();
	afx_msg void OnTextcolorD();
	afx_msg void OnTextcolorDa();
	afx_msg void OnValueA();
	afx_msg void OnValueD();
	afx_msg void OnVisible();
	afx_msg void OnInputD();
	afx_msg void OnCinputD();
	afx_msg void OnCinputA();
	afx_msg void OnCslideH();
	afx_msg void OnCslideV();
	afx_msg void OnCbuttonD();
	afx_msg void OnCbuttonA();
	afx_msg void OnCbuttonWnd();
	afx_msg void OnCvalueD();
	afx_msg void OnCvalueA();
	afx_msg void OnCpencolorD();
	afx_msg void OnCpencolorA();
	afx_msg void OnCpencolorDa();
	afx_msg void OnCpencolorAa();
	afx_msg void OnCbrushcolorD();
	afx_msg void OnCbrushcolorA();
	afx_msg void OnCbrushcolorDa();
	afx_msg void OnCbrushcolorAa();
	afx_msg void OnCtextcolorD();
	afx_msg void OnCtextcolorA();
	afx_msg void OnCtextcolorDa();
	afx_msg void OnCtextcolorAa();
	afx_msg void OnCsizeH();
	afx_msg void OnCposH();
	afx_msg void OnCfillH();
	afx_msg void OnCsizeV();
	afx_msg void OnCposV();
	afx_msg void OnCfillV();
	afx_msg void OnCvisible();
	afx_msg void OnCenable();
	afx_msg void OnCblink();
	afx_msg void OnCdirection();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_rank0;
	afx_msg void OnBnClickedRank0();
	afx_msg void OnBnClickedRank1();
	afx_msg void OnBnClickedRank2();
	afx_msg void OnBnClickedRank3();
	afx_msg void OnBnClickedRank4();
	afx_msg void OnBnClickedRank5();
	afx_msg void OnBnClickedRank6();
	afx_msg void OnBnClickedRank7();
	afx_msg void OnBnClickedRank8();
	afx_msg void OnBnClickedRank9();
	BOOL m_rank1;
	BOOL m_rank2;
	BOOL m_rank3;
	BOOL m_rank4;
	BOOL m_rank5;
	BOOL m_rank6;
	BOOL m_rank7;
	BOOL m_rank8;
	BOOL m_rank9;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIADNYATTR_H__81A5D002_54E1_11D5_A636_00106075B53B__INCLUDED_)
