// SsiShl.cpp : Defines the entry point for the DLL application.
//

#include "precomp.h"
#include "rtkgui.h"

// ȫ�ֱ���
HINSTANCE g_hInst; // ģ��ʵ�����

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


