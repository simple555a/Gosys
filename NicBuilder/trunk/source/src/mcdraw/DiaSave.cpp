// DiaSave.cpp : implementation file
//

#include "stdafx.h"
#include "mcdraw.h"
#include "DiaSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaSave dialog


CDiaSave::CDiaSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaSave)
	m_select = _T("");
	//}}AFX_DATA_INIT
}


void CDiaSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaSave)
	DDX_Control(pDX, IDC_OBJECTS, m_objects);
	DDX_Text(pDX, IDC_SELECT, m_select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaSave, CDialog)
	//{{AFX_MSG_MAP(CDiaSave)
	ON_LBN_SELCHANGE(IDC_OBJECTS, OnSelchangeObjects)
	ON_LBN_DBLCLK(IDC_OBJECTS, OnDblclkObjects)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaSave message handlers

void CDiaSave::OnSelchangeObjects() 
{
	m_objects.GetText(m_objects.GetCurSel(), m_select);
	UpdateData(false);
}

static int UTILS_API ep(
	const char * dir, 
	const char * fullPath, 
	const char * relaPath, 
	const void * con
	)
{
	CString sTemp;
	CDiaSave * dia;
	dia = (CDiaSave *)con;
	sTemp = relaPath;
	sTemp = sTemp.Left(sTemp.GetLength()-4);
	dia->m_objects.AddString(sTemp);
	return 1;
}

BOOL CDiaSave::OnInitDialog() 
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	CString sTemp;

	CDialog::OnInitDialog();
	
	sTemp.Format("保存(工程路径:%s)", theApp.m_path);
	SetWindowText(sTemp);
	if(m_selectMode == 0){
		enumerate_files(
			m_baseDir, 
			m_wildCards, 
			ep, 
			SEARCH_RECURSIVELY | SEARCH_HIDDEN, 
			this
			);
	}else{
		hFind = FindFirstFile((LPCTSTR)m_dir, &FindFileData);
		if(hFind!=INVALID_HANDLE_VALUE){
	        sTemp=FindFileData.cFileName;
	        sTemp=sTemp.Left(sTemp.GetLength()-4);
			m_objects.AddString(sTemp);
			while(FindNextFile(hFind,&FindFileData)){
				sTemp=FindFileData.cFileName;
				sTemp=sTemp.Left(sTemp.GetLength()-4);
				m_objects.AddString(sTemp);
			}
		}

		FindClose(hFind);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaSave::OnOK() 
{
	//先检查文件是否存在，然后判断是否是一个合法的文件名
	UpdateData();
	CString sFilePath=m_dir.Left(m_dir.GetLength()-5)+m_select+m_dir.Right(4);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile((LPCTSTR)sFilePath, &FindFileData);
	if(hFind!=INVALID_HANDLE_VALUE){
		CString sTemp;
		sTemp.Format("是否覆盖已有文件<%s%s>",m_select,m_dir.Right(4));
		if(MessageBox((LPCTSTR)sTemp, "选择", MB_YESNO)==IDNO){
			return;
		}
	}else{
		HANDLE hFile;  
		hFile = CreateFile(sFilePath,           // create MYFILE.TXT 
					 GENERIC_WRITE,                // open for writing 
					 0,                            // do not share 
					 NULL,                         // no security 
					 CREATE_ALWAYS,                // overwrite existing 
					 FILE_ATTRIBUTE_NORMAL |       // normal file 
					 FILE_FLAG_OVERLAPPED,         // asynchronous I/O 
					 NULL);                        // no attr. template 

		if (hFile == INVALID_HANDLE_VALUE) 
		{ 
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);
			// Process any inserts in lpMsgBuf.
			// ...
			// Display the string.
			MessageBox((LPCTSTR)lpMsgBuf, "错误", MB_OK | MB_ICONINFORMATION);
			// Free the buffer.
			LocalFree( lpMsgBuf );
			return;
			//ErrorHandler("Could not open file.");  // process error 
		}
		CloseHandle(hFile);
	}

	CDialog::OnOK();
}

void CDiaSave::OnDblclkObjects() 
{
	OnOK();
}
