// mcEvtQuery.h : main header file for the MCEVTQUERY application
//

#if !defined(AFX_MCEVTQUERY_H__EC543C48_AF87_461C_9A99_D0483BCF5598__INCLUDED_)
#define AFX_MCEVTQUERY_H__EC543C48_AF87_461C_9A99_D0483BCF5598__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMcEvtQueryApp:
// See mcEvtQuery.cpp for the implementation of this class
//

class CMcEvtQueryApp : public CWinApp
{
public:
	CString m_path;
	CString m_folder;
	CMcEvtQueryApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcEvtQueryApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CMcEvtQueryApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCEVTQUERY_H__EC543C48_AF87_461C_9A99_D0483BCF5598__INCLUDED_)
