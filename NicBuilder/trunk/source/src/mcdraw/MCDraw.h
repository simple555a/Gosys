// MCDraw.h : main header file for the MCDRAW application
//

#if !defined(AFX_MCDRAW_H__D40060CC_A626_4CFF_A36F_9BF68198918D__INCLUDED_)
#define AFX_MCDRAW_H__D40060CC_A626_4CFF_A36F_9BF68198918D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMCDrawApp:
// See MCDraw.cpp for the implementation of this class
//

class CMCDrawApp : public CWinApp
{
public:
	CString m_path;
	CMCDrawApp();
	~CMCDrawApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCDrawApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL SaveAllModified();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CMCDrawApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CMCDrawApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCDRAW_H__D40060CC_A626_4CFF_A36F_9BF68198918D__INCLUDED_)
