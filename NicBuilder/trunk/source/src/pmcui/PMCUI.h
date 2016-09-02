// PMCUI.h : main header file for the PMCUI DLL
//

#if !defined(AFX_PMCUI_H__E95F24A9_21D9_4186_B934_9D9C4A814A73__INCLUDED_)
#define AFX_PMCUI_H__E95F24A9_21D9_4186_B934_9D9C4A814A73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPMCUIApp
// See PMCUI.cpp for the implementation of this class
//

class CPMCUIApp : public CWinApp
{
public:
	CPMCUIApp();
	CString m_path;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPMCUIApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPMCUIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CPMCUIApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMCUI_H__E95F24A9_21D9_4186_B934_9D9C4A814A73__INCLUDED_)
