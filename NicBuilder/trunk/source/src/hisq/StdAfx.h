// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2376E06A_5DBB_4E38_9C6D_463AD563CE60__INCLUDED_)
#define AFX_STDAFX_H__2376E06A_5DBB_4E38_9C6D_463AD563CE60__INCLUDED_

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

#include "config.h"
#include "hisq.h"
#include "mainfrm.h"
#include "hisproxy.h"

extern RTK_HANDLE archiveCache;
extern archive_cache_info_t cacheInfo;
extern TAG_NAME names[10];

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2376E06A_5DBB_4E38_9C6D_463AD563CE60__INCLUDED_)

