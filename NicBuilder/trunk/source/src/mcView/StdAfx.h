// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6713E8E0_8C1C_477B_AA1E_ACB2C9D1FC97__INCLUDED_)
#define AFX_STDAFX_H__6713E8E0_8C1C_477B_AA1E_ACB2C9D1FC97__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#import "c:\windows\system32\msxml.dll" named_guids
//#import "msxml.dll" named_guids
#import "MsXml.Tlb" named_guids
using namespace MSXML;

#pragma warning(disable:4244)
#pragma warning(disable:4786)
#pragma warning(disable:4996)


#define TOTAL_WIDTH 6400
#define TOTAL_HEIGHT 4600

//#define HAVE_TIME          //added by Zander, 12th April, 2012

#include <pmc.h>
#include <pmc_hislog.h>
#include <pmcseeker.h>


void writeAnalog(const char * name, __r4 v);
void writeAnalog_x(const char * name, __r4 v);
__r4 getFloat(const struct pmc_value_t  & t);

extern HINSTANCE g_hScriptDlll;
extern int g_maxPopup;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6713E8E0_8C1C_477B_AA1E_ACB2C9D1FC97__INCLUDED_)

