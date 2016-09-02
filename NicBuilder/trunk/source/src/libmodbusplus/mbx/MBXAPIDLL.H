// MBXAPIDLL.h : main header file for the MBXAPIDLL DLL
//

#if !defined(AFX_MBXAPIDLL_H__AA846F17_FD15_11D2_88EE_0080C8ED2F3E__INCLUDED_)
#define AFX_MBXAPIDLL_H__AA846F17_FD15_11D2_88EE_0080C8ED2F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMBXAPIDLLApp
// See MBXAPIDLL.cpp for the implementation of this class
//

class CMBXAPIDLLApp : public CWinApp
{
public:
	CMBXAPIDLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMBXAPIDLLApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMBXAPIDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBXAPIDLL_H__AA846F17_FD15_11D2_88EE_0080C8ED2F3E__INCLUDED_)
