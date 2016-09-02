// keycDlg.cpp : implementation file
//

#include "stdafx.h"
#include "keyc.h"
#include "keycDlg.h"
#include "dlghotkey.h"
#include <utils.h>

#pragma comment(lib, "libutils.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeycDlg dialog

CKeycDlg::CKeycDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeycDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeycDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bModified=false;
}

void CKeycDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeycDlg)
	DDX_Control(pDX, IDC_KEY, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeycDlg, CDialog)
	//{{AFX_MSG_MAP(CKeycDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_KEY, OnGetdispinfoKey)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_KEY, OnColumnclickKey)
	ON_NOTIFY(NM_DBLCLK, IDC_KEY, OnDblclkKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeycDlg message handlers

BOOL CKeycDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//get file path
    char lpFilename[_MAX_PATH];
    memset(lpFilename,0,_MAX_PATH);
    GetModuleFileName( AfxGetInstanceHandle(),lpFilename,_MAX_PATH);
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath( lpFilename, drive, dir, fname, ext );
    CString sAppDrive=drive;
    CString sAppPath=dir;
	m_path=sAppDrive+sAppPath+"file\\";

	InitList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeycDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeycDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeycDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKeycDlg::OnGetdispinfoKey(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	long pt=pDispInfo->item.lParam;

    if(pDispInfo->item.mask & LVIF_TEXT)
    {
        LPTSTR& pszText = pDispInfo->item.pszText;
        if(0==pDispInfo->item.iSubItem){
			pszText=key2view[pt].view;
		}else if(1==pDispInfo->item.iSubItem){
			pszText=key2view[pt].key;
        }
    }
	
	*pResult = 0;
}

void CKeycDlg::InitList()
{
	int i;

	//ini columns
	LV_COLUMN listColumn;
    listColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
    listColumn.fmt = LVCFMT_LEFT;
    listColumn.cx = 150;
    listColumn.pszText = "图形界面";
    listColumn.iSubItem = 0;
    m_list.InsertColumn(0, &listColumn);
	listColumn.cx=50;
	listColumn.pszText = "快捷键";
    listColumn.iSubItem = 1;
    m_list.InsertColumn(1, &listColumn);

    CString strFilePath=m_path+"*.gdf";

	//get items
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
    CString sTemp;
	hFind = FindFirstFile((LPCTSTR)strFilePath, &FindFileData);
	nv=0;
	if(hFind!=INVALID_HANDLE_VALUE){
        sTemp=FindFileData.cFileName;
        sTemp=sTemp.Left(sTemp.GetLength()-4);
		memset(key2view[nv].view,0,sizeof(key2view[nv].view));
		strncpy(key2view[nv].view,(LPCTSTR)sTemp,sizeof(key2view[nv].view));
		sprintf(key2view[nv].key,"");
		nv++;
		while(FindNextFile(hFind,&FindFileData)){
            sTemp=FindFileData.cFileName;
            sTemp=sTemp.Left(sTemp.GetLength()-4);
			memset(key2view[nv].view,0,sizeof(key2view[nv].view));
			strncpy(key2view[nv].view,(LPCTSTR)sTemp,sizeof(key2view[nv].view));
			sprintf(key2view[nv].key,"");
			nv++;
			if(nv>=MAX_COUNT) break;
		}
	}
	FindClose(hFind);

	//load old shortkeys
	char line[1024];
	CArgs args;
	CString sConf=m_path+"shortkey.conf";
	FILE *pFile=fopen((LPCTSTR)sConf,"rt");
	if(NULL!=pFile){
		//load new data
		while(NULL!=fgets(line,1024,pFile)){
			parse_arg_ey(line,&args,",\n");
			for(i=0;i<nv;i++){
				if(0==strcmp(args.argv[0],key2view[i].view)){
					sprintf(key2view[i].key,"%s",args.argv[1]);
				}
			}
		}

		fclose(pFile);
	}

	//sort items
	SortByView();

	//added items
	LV_ITEM listItem;
    listItem.mask = LVIF_TEXT|LVIF_PARAM;
    listItem.mask = LVIF_TEXT|LVIF_PARAM;
	for(i=0;i<nv;i++){
		listItem.iSubItem = 0;
		listItem.iItem = m_list.GetItemCount();
		listItem.pszText = LPSTR_TEXTCALLBACK;
		listItem.cchTextMax = _MAX_FNAME;
		listItem.lParam = (LPARAM)i;
		m_list.InsertItem(&listItem);
		m_list.SetItemText(listItem.iItem,1, LPSTR_TEXTCALLBACK);
	}

}

void CKeycDlg::OnColumnclickKey(NMHDR* pNMHDR, LRESULT* pResult) 
{
	static bool vAsc=true,kAsc=true;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(0==pNMListView->iSubItem){
		SortByView(vAsc);
		vAsc=!vAsc;
	}else{
		SortByKey(kAsc);
		kAsc=!kAsc;
	}
	
	m_list.RedrawWindow();
	*pResult = 0;
}

inline void Switch2Item(SKey2View & s2v1,SKey2View & s2v2)
{
	SKey2View sk2v;
	sk2v=s2v1;
	s2v1=s2v2;
	s2v2=sk2v;
}

void CKeycDlg::SortByView(bool asc)
{
	int i,j;
	for(i=0;i<nv-1;i++){
		for(j=i+1;j<nv;j++){
			if(asc){
				if(strcmp(key2view[i].view,key2view[j].view)>0){
					Switch2Item(key2view[i],key2view[j]);
				}
			}else{
				if(strcmp(key2view[i].view,key2view[j].view)<0){
					Switch2Item(key2view[i],key2view[j]);
				}
			}
		}
	}
}

void CKeycDlg::SortByKey(bool asc)
{
	int i,j;
	for(i=0;i<nv-1;i++){
		for(j=i+1;j<nv;j++){
			if(asc){
				if(strcmp(key2view[i].key,key2view[j].key)>0){
					Switch2Item(key2view[i],key2view[j]);
				}
			}else{
				if(strcmp(key2view[i].key,key2view[j].key)<0){
					Switch2Item(key2view[i],key2view[j]);
				}
			}
		}
	}
}

void CKeycDlg::OnDblclkKey(NMHDR* pNMHDR, LRESULT* pResult) 
{
    POSITION pos = m_list.GetFirstSelectedItemPosition();
    if (pos == NULL) return;

    int nItem = m_list.GetNextSelectedItem(pos);
	CDlgHotkey dlg;
	dlg.m_key=key2view[nItem].key;
	if(IDOK==dlg.DoModal()){
		sprintf(key2view[nItem].key,"%s",dlg.m_key);
		m_list.RedrawItems(nItem,nItem);
		bModified=true;
	}

	*pResult = 0;
}

void CKeycDlg::OnOK() 
{
	if(CheckDual()) return;

	//save shortkey to file
	CString sConf=m_path+"shortkey.conf";
	FILE *pFile=fopen((LPCTSTR)sConf,"wt");
	if(NULL!=pFile){
		CString sKey2View;
		for(int i=0;i<nv;i++){
			if('\0'!=key2view[i].key[0]){
				sKey2View.Format("%s,%s",key2view[i].view,key2view[i].key);
				fprintf(pFile,"%s\n",sKey2View);
			}
		}
		fclose(pFile);
		bModified=false;
	}
	
	//CDialog::OnOK();
}

bool CKeycDlg::CheckDual()
{

	int i,j,k;
	for(i=0;i<nv-1;i++){
		for(j=i+1;j<nv;j++){
			if('\0'!=key2view[i].key[0]){
				if(0==strcmp(key2view[i].key,key2view[j].key)){
					CString sErr;
					sErr.Format("键<%s>存在重复定义",key2view[i].key);
					MessageBox(sErr,"错误",MB_OK|MB_ICONSTOP);
					SKey2View k2v=key2view[i];
					SortByKey();
					m_list.RedrawWindow();
					for(k=0;k<nv-1;k++){
						if(0==strcmp(k2v.key,key2view[k].key)) break;
					}
					m_list.EnsureVisible(k+1,false);
					return true; 
				}
			}
		}
	}

	return false;
}

void CKeycDlg::OnCancel() 
{
	if(bModified){
		if(IDYES==MessageBox("退出前是否保存",NULL,MB_YESNO|MB_ICONQUESTION)){
			OnOK();
			if(bModified) return;
		}
	}
	
	CDialog::OnCancel();
}
