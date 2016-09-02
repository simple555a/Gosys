/********************************************************************
	created:	2002/08/26
	created:	26:8:2002   15:56
	filename: 	D:\dfvs2\SRC\VC\PUBLIC\SRC\LIBUTILS\appname.c
	file path:	D:\dfvs2\SRC\VC\PUBLIC\SRC\LIBUTILS
	file base:	appname
	file ext:	c
	author:		Jackie Hunter, jhunter@tsinghua.org.cn
	
	purpose:	从进程的pid获得创建进程的exe文件名字
	History :
	2002/8/26, first workable version, Jackie 和 Neo合作完成。
	2002/8/27, Jackie, support service and native applications.
*********************************************************************/
#include "precomp.h"
#pragma hdrstop 

#if 0

static int exename_by_pid_nt(DWORD pid, char buf[], int buf_len)
{
	wchar_t name[MAX_PATH];
	DWORD	pname;
	DWORD   read;
	HANDLE  hProc;

	*buf = 0;

	hProc = OpenProcess(PROCESS_VM_READ, FALSE, pid);
	if(!hProc){
		DEBUG_PRINTF(("Pid 0x%08x : <OpenProcess failed.>, error %d\n", pid, GetLastError()));
		return 0;
	}
	if( !ReadProcessMemory(hProc, (LPCVOID)0x2003c, &pname, sizeof(pname), &read) ){
		DEBUG_PRINTF(("Pid 0x%08x : <Read memory failed 1>, error %d.\n", pid, GetLastError()));
		CloseHandle(hProc);
		return 0;
	}
	if( !ReadProcessMemory(hProc, (LPCVOID)pname, name, sizeof(name), &read) ){
		DEBUG_PRINTF(("Pid 0x%08x : <Read memory failed>, error %d.\n", pid, GetLastError()));
		CloseHandle(hProc);
		return 0;
	}
	name[sizeof(name)/sizeof(name[0]) - 1] = 0;
	WideCharToMultiByte(CP_ACP, 0, name, -1, buf, buf_len, NULL, NULL);
	buf[buf_len-1] = 0;

	/*
		if it's native application
	*/
	if(strlen(buf)>4){
		if( *((DWORD*)buf) == 0x5c3f3f5c ){	// '\\??\\'不知道为什么不行
			char *p, *q;
			p = buf;
			q = buf+4;
			while(*q){
				*p = *q;
				q++;
				p++;
			}
			*p = 0;
		}
	}
	DEBUG_PRINTF(("Pid 0x%08x:%s\n", pid, buf));

	CloseHandle(hProc);

	return strlen(buf);
}

//---------------------------------------------------------------------------
static FARPROC _GetReallyProcAdd(FARPROC proc)
{
    DWORD radd = 0;
    DWORD tmp;
    if((*((DWORD*)proc) & 0xffff) == 0x25ff)
        radd = *(DWORD *)((DWORD)proc + 2);
    tmp = *((DWORD *)(radd));
    if(tmp < (DWORD)GetModuleHandle("kernel32.dll") && tmp > 0x80000000)
    {
        radd = tmp + 1;
    }
    return (FARPROC)(*(DWORD*)radd);
}

static HMODULE _LoadLibrary(char * name)
{
	HMODULE hModule;
	char szDllPath[MAX_PATH];
	hModule = LoadLibrary(name);
	if(hModule){
		return hModule;
	}
	GetModuleFileName(g_hModule, szDllPath, sizeof(szDllPath));
	extract_file_pathA(szDllPath);
	merge_pathA(szDllPath, name);
	hModule = LoadLibrary(szDllPath);
	return hModule;
}

//---------------------------------------------------------------------------
static FARPROC _GetKeProcAddr(const char * name)
{
    HMODULE hModule = NULL;
    OSVERSIONINFO info;
	FARPROC proc;

    info.dwOSVersionInfoSize = sizeof(info);
    GetVersionEx(&info);
    if(info.dwPlatformId == VER_PLATFORM_WIN32_NT)
        return NULL;

    __try{
        if((hModule = _LoadLibrary("sunvklib.dll")) == NULL)
            return 0;
        proc = (FARPROC)_GetReallyProcAdd(GetProcAddress(hModule, name));
    }__except(utils_exception_filter()){
        if(hModule){
            FreeLibrary(hModule);
            hModule = NULL;
        }
    }
    if(hModule){
		FreeLibrary(hModule);
    }
        
	return proc;
}

static PIMTE * GetGlobeModuleTableBase(void)
{
	DWORD (WINAPI *_GDIReallyCares)( HINSTANCE );
    PIMTE *pImte = 0;
    // Yes, this is really disgusting!
	*((FARPROC*)&_GDIReallyCares) = _GetKeProcAddr("KlibB");
	if(!_GDIReallyCares){
		return NULL;
	}
    if(_GDIReallyCares( GetModuleHandle(0) )){
		__asm   mov     pImte, ecx
    }
    return pImte;
}

static int exename_by_pid_98(DWORD pid, char buf[], int buf_len)
{
	PPROCESS_DATABASE pdb;
	PIMTE *imte;

    *buf=0;
	
	pdb = (PPROCESS_DATABASE)(pid ^ g_dwObsfucator);
	imte = GetGlobeModuleTableBase();
	if(!imte){
		return 0;
	}
	imte += pdb->MTEIndex;
	DEBUG_PRINTF(("Process 0x%08x, name=%s\n",pid , (*imte)->pszFileName));
	strncpy(buf, (*imte)->pszFileName, buf_len);
	buf[buf_len-1] = 0;
	return strlen(buf);
}

LIBUTILS_API int UTILS_API exename_by_pid(DWORD pid,char buf[],int buf_len)
{
	if(!buf || !buf_len){
		return 0;
	}
	__try{
		if(!is_winnt()){  
			return exename_by_pid_98(pid,buf,buf_len);
		}else{
			return exename_by_pid_nt(pid, buf, buf_len);
		}
	}__except(utils_exception_filter()){
		*buf = 0;
		return 0;
	}
}

/*
//---------------------------------------------------------------------------
void TFormHWND::GetWndHandleList(TListView *ListView, DWORD pid)
{
    TListItem  *ListItem;
    AnsiString WndTxt;
    AnsiString str;
    PPROCESS_DATABASE pPdb;
    PIMTE pImte;
    char tt[2];
    char buf[80];
    PWND32 pWnd32;
    DWORD thPID;
    DWORD thTID;
    DWORD i;
    int hndNum = 0;
// UserHeapBase + 0x10000 + 0x70 所指向的DWORD值 是句柄最大值
// UserHeapBase + 0x10000 + 0x20 是可用句柄数
    DWORD HandleMax = *(PDWORD)(UserHeapBase + 0x10000 + 0x70);

    ListViewHWND->SortType = stNone;
    ListViewHWND->Items->Clear();
    ProgBar->Visible = true;

    ProgBar->Max = HandleMax;

    tt[1] = 0;
    tt[0] = '"';
// 哈哈！ 又让我发现一个秘密，IsWindow如果返回成功的话，返回值就是一个
// 指向该WND结构的指针，而这个结构是未公开的，KAO，真是诡异哦
    for(i = 0x80; i < HandleMax; i += 4)
    {
        ProgBar->Position = i;
        pWnd32 = (PWND32)IsWindow((HWND)i);
        if ( !pWnd32 )
            continue;
        thTID = GetWindowThreadProcessId((HWND)i, &thPID);
        if(pid && thPID != pid)
            continue;
        hndNum ++;
        WndTxt = tt;
        ListItem = ListView->Items->Add();
        ListItem->Caption = str.sDEBUG_PRINTF(("0x%04X", i);
        int len = GetWindowText((HWND)i, buf, sizeof(buf));
        if(!len)
            buf[len] = 0;
        WndTxt += buf;
        WndTxt += tt;
        if(GetClassName((HINSTANCE)i, buf, sizeof(buf)))
        {
            WndTxt += buf;
            WndTxt += wndGetAtomName((HWND)i);
        }
        ListItem->SubItems->Add(WndTxt);
        ListItem->SubItems->Add(str.sDEBUG_PRINTF(("0x%08X", thPID));
        ListItem->SubItems->Add(str.sDEBUG_PRINTF(("0x%08X", thTID));
        pPdb = (PPROCESS_DATABASE)(thPID ^ process->Unobsfucator);
        WndTxt = pPdb->pEDB->pszCmdLine;
        if(!WndTxt.Length())
        {
            pImte = process->ppImte[pPdb->MTEIndex];
            if(!IsBadStringPtr(pImte->pszFileName, 255))
                WndTxt = pImte->pszFileName;
        }
        ListItem->SubItems->Add(WndTxt);
    }
    ListViewHWND->SortType = stBoth;
    i = *(PDWORD)(UserHeapBase + 0x10000 + 0x20);
    Caption = sTit + AnsiString(hndNum) + "/" +AnsiString(i);
    ProgBar->Visible = false;
}

//---------------------------------------------------------------------------
DWORD GetUserHeapBase(HMODULE hModUser)
{
    return (DWORD)ConvertFar16PtrToFlat( MAKELONG(0, hModUser) );
}
//---------------------------------------------------------------------------
PVOID ConvertFar16PtrToFlat( DWORD far16ptr )
{
    LDT_ENTRY selectorEntry;

    if ( !GetThreadSelectorEntry(GetCurrentThread(), HIWORD(far16ptr),
                                &selectorEntry) )
    {
        return 0;
    }

    return  (PVOID)(
            (selectorEntry.HighWord.Bytes.BaseHi << 24) +
            (selectorEntry.HighWord.Bytes.BaseMid << 16) +
            selectorEntry.BaseLow +
            LOWORD(far16ptr));
}
//---------------------------------------------------------------------------
WORD GetUserResources(HINSTANCE hInstUser16)
{
    char buffer[0x40];
    WORD retv = 0;
    GFSR_PROC pfnFreeSystemResources = 0;   // We don't want these as locals in
    pfnFreeSystemResources =
        (GFSR_PROC) GetProcAddress16(hInstUser16, "GetFreeSystemResources");
    if ( !pfnFreeSystemResources )
    {
//        printf( "GetProcAddress16() failed!\n" );
        return 0;
    }
    __asm {
        push    GFSR_USERRESOURCES
        mov     edx, [pfnFreeSystemResources]
    }
    QT_Thunk();
    __asm mov retv,ax
    return retv;
}
//---------------------------------------------------------------------------
WORD GetGdiResources(HINSTANCE hInstUser16)
{
    char buffer[0x40];
    WORD retv = 0;
    GFSR_PROC pfnFreeSystemResources = 0;   // We don't want these as locals in
    pfnFreeSystemResources =
        (GFSR_PROC) GetProcAddress16(hInstUser16, "GetFreeSystemResources");
    if ( !pfnFreeSystemResources )
    {
//        printf( "GetProcAddress16() failed!\n" );
        return 0;
    }
    __asm {
        push    GFSR_GDIRESOURCES
        mov     edx, [pfnFreeSystemResources]
    }
    QT_Thunk();
    __asm mov retv,ax
    return retv;
}
*/
#endif