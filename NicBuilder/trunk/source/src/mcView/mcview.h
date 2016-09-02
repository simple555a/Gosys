// mcview.h : main header file for the MCVIEW application
//

#if !defined(AFX_MCVIEW_H__F5DB8C4D_9C99_48D1_BAFB_A7685BE24B20__INCLUDED_)
#define AFX_MCVIEW_H__F5DB8C4D_9C99_48D1_BAFB_A7685BE24B20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

extern bool g_bEnableNoSwitch;
extern char * g_startFile;
extern bool bEnableSoundAlarm;
extern bool bRaiseSoundAlarm;

/////////////////////////////////////////////////////////////////////////////
// CMcviewApp:
// See mcview.cpp for the implementation of this class
//

class CMcviewApp : public CWinApp
{
public:
	CString m_path;
	CMcviewApp();
	~CMcviewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMcviewApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMcviewApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

extern CMcviewApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCVIEW_H__F5DB8C4D_9C99_48D1_BAFB_A7685BE24B20__INCLUDED_)
