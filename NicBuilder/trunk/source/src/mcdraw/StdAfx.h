// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//


#if !defined(AFX_STDAFX_H__9B8EAFD0_E11F_43A4_831D_5663EA91D283__INCLUDED_)
#define AFX_STDAFX_H__9B8EAFD0_E11F_43A4_831D_5663EA91D283__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#import <msxml.dll> named_guids
#import <MsXml.Tlb> named_guids

//#import "c:\windows\system32\msxml.dll" named_guids
using namespace MSXML;
#pragma warning(disable:4244)
#pragma warning(disable:4786)
#pragma warning(disable:4996)


#include <pmc.h>
#include <pmc_ui.h>
#include <pmcseeker.h>

#include "mcdraw.h"

#define TOTAL_WIDTH 6400
#define TOTAL_HEIGHT 4600

extern char * docEncoding;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9B8EAFD0_E11F_43A4_831D_5663EA91D283__INCLUDED_)
