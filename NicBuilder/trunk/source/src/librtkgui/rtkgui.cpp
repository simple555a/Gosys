// SsiShl.cpp : Defines the entry point for the DLL application.
//

#include "precomp.h"
#include "rtkgui.h"

// 全局变量
HINSTANCE g_hInst; // 模块实例句柄

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  fdwReason, LPVOID lpReserved )
{
    switch(fdwReason)
    {
    case DLL_PROCESS_ATTACH: {
            g_hInst = (HINSTANCE)hModule;
        }
        break;         
    case DLL_THREAD_ATTACH: {
        }
        break;
    case DLL_PROCESS_DETACH: {
        }
        break;
    case DLL_THREAD_DETACH: {
        }
        break;
    default:
        break;
    }
    return TRUE;
}


