// DiaSelect.cpp : implementation file
//

#include "stdafx.h"
#include "MCDraw.h"
#include "DiaSelect.h"

#pragma hdrstop

#include <string>
#include <list>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaSelect dialog

class CMetaCache{
public:
	void Play(CDC *dc, const char * file, const RECT * r)
	{
		CCache::iterator it;
		it = FindEntry(file);
		if(it == c.end()){
			m_iMiss++;
			it = CacheEntry(file);
		}else{
			m_iHits++;
		}
		if(it != c.end()){
			dc->PlayMetaFile(it->handle, r);
		}
	}
	CMetaCache()
	{
		m_iMiss = m_iHits = 0;
		c.clear();
	}
	~CMetaCache()
	{
		utils_trace(
			"Metafile cache : %d hits(%.3f), %d misses\n",
			m_iHits,
			(double)m_iHits / (m_iHits + m_iMiss),
			m_iMiss
			);
		// utils_getc();
		Destroy();
	}
	void Destroy()
	{
		CCache::iterator it;
		for(it = c.begin(); it != c.end(); it++){
			DeleteEnhMetaFile(it->handle);
		}
		c.clear();
	}
protected:
	struct CCacheEntry{
		string			file;
		HENHMETAFILE	handle;
		FILETIME		birth;
	};
	typedef list<CCacheEntry> CCache;
	CCache c;
	enum{
		MAX_ITEMS = 16,
	};
	CCache::iterator FindEntry(const char * f)
	{
		CCache::iterator it;
		for(it=c.begin(); it != c.end(); it++){
			if(!stricmp(it->file.data(), f)){
				return it;
			}
		}
		return it;
	}
	CCache::iterator CacheEntry(const char * f)
	{
		CCacheEntry e;
		CCache::iterator it;

		e.handle = (HENHMETAFILE)pmcui_open_metafile(f);
		if(!e.handle){
			return c.end();
		}

		e.file = f;
		if(c.size() >= MAX_ITEMS){
			DeleteEnhMetaFile(c.back().handle);
			c.pop_back();
		}
		c.push_front(e);
		return c.begin();
	}
	int	m_iHits;
	int	m_iMiss;
};

static CMetaCache g_Cache;

CDiaSelect::CDiaSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaSelect)
	//}}AFX_DATA_INIT
}


void CDiaSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaSelect)
	DDX_Control(pDX, IDC_PREVIEW, m_preview);
	DDX_Control(pDX, IDC_OBJECTS, m_objects);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaSelect, CDialog)
	//{{AFX_MSG_MAP(CDiaSelect)
	ON_LBN_SELCHANGE(IDC_OBJECTS, OnSelchangeObjects)
	ON_LBN_DBLCLK(IDC_OBJECTS, OnDblclkObjects)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaSelect message handlers

static int UTILS_API ep(
	const char * dir, 
	const char * fullPath, 
	const char * relaPath, 
	const void * con
	)
{
	CString sTemp;
	CDiaSelect * dia;
	dia = (CDiaSelect*)con;
	sTemp = relaPath;
	sTemp = sTemp.Left(sTemp.GetLength()-4);
	dia->m_objects.AddString(sTemp);
	return 1;
}

BOOL CDiaSelect::OnInitDialog() 
{
	// WIN32_FIND_DATA FindFileData;
	// HANDLE hFind;	
	CString sTemp;

	CDialog::OnInitDialog();
	
	sTemp.Format("打开(工程路径:%s)", get_working_dir());
	SetWindowText(sTemp);

	if(m_wildcards == "*.emf"){
		m_preview.ShowWindow(SW_SHOWNORMAL);
	}else{
		m_preview.ShowWindow(SW_HIDE);
	}

	// TODO: Add your control notification handler code here
	m_objects.ResetContent();
#if 1
	enumerate_files(
		m_dir,
		m_wildcards, 
		ep, 
		SEARCH_RECURSIVELY | SEARCH_HIDDEN, 
		this
		);
#else
	sTemp = m_dir + "/" + m_wildcards;
	hFind = FindFirstFile((LPCTSTR)sTemp, &FindFileData);
	if(hFind!=INVALID_HANDLE_VALUE){
		do{
			sTemp=FindFileData.cFileName;
			sTemp=sTemp.Left(sTemp.GetLength()-4);
			m_objects.AddString(sTemp);
		}while(FindNextFile(hFind,&FindFileData));
	}
	FindClose(hFind);

	if(m_wildcards == "*.emf"){
		// search in application root directory
		sTemp = get_exe_dir() + m_dir + "/" + m_wildcards;
		hFind = FindFirstFile((LPCTSTR)sTemp, &FindFileData);
		if(hFind!=INVALID_HANDLE_VALUE){
			do{
				sTemp=FindFileData.cFileName;
				sTemp=sTemp.Left(sTemp.GetLength()-4);
				if(m_objects.FindString(-1, sTemp) == CB_ERR){
					m_objects.AddString(sTemp);
				}				
			}while(FindNextFile(hFind,&FindFileData));
		}
		FindClose(hFind);
	}
#endif
	if(!m_select.IsEmpty()){
		int i;
		for(i=0; i<m_objects.GetCount(); i++){
			CString s;
			m_objects.GetText(i, s);
			// utils_debug("%s - %s\n", (LPCSTR)s, (LPCSTR)m_select);
			if(s == m_select){
				// m_objects.EnsureVisible(i);
				m_objects.SetCurSel(i);
				break;
			}
		}
	}
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaSelect::OnSelchangeObjects() 
{
	m_objects.GetText(m_objects.GetCurSel(),m_select);
	if(m_wildcards == "*.emf"){
#if 1
		RECT r;
		CDC * dc = m_preview.GetDC();
		m_preview.GetClientRect(&r);
		dc->FillSolidRect(0, 0, r.right, r.bottom, dc->GetBkColor());
		g_Cache.Play(dc, m_select + ".emf", &r);
		m_preview.ReleaseDC(dc);
#else
		m_preview.SetWindowText("pic/" + m_select + ".emf");
#endif
	}
}

void CDiaSelect::OnOK() 
{
	if(m_objects.GetCurSel()==LB_ERR){
		MessageBox("请选择对象");
		return;
	}
	
	CDialog::OnOK();
}

void CDiaSelect::OnDblclkObjects() 
{
	OnOK();	
}
