// keyc.h : main header file for the KEYC application
//

#if !defined(AFX_KEYC_H__045C3498_0790_490C_B94C_B07A52EBEC9A__INCLUDED_)
#define AFX_KEYC_H__045C3498_0790_490C_B94C_B07A52EBEC9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeycApp:
// See keyc.cpp for the implementation of this class
//

class CKeycApp : public CWinApp
{
public:
	CKeycApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeycApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeycApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYC_H__045C3498_0790_490C_B94C_B07A52EBEC9A__INCLUDED_)
