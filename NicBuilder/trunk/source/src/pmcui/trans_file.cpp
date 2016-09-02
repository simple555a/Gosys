#include "stdafx.h"
#pragma hdrstop
PMCUI_API bool g_bLoadRedirections;
PMCUI_API bool (_cdecl *redirect_file)(const CString & rawname, CString & translated_name);
PMCUI_API bool (_cdecl *save_redirected_file)(const CString & rawname, const CString & translated_name);
PMCUI_API bool (_cdecl *load_redirected_file)(const CString & rawname, const CString & translated_name);
