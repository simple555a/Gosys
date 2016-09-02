// DiaCellAttr.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "MCDraw.h"
#include "DiaCellAttr.h"
#include "DiaButtonWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaCellAttr dialog


CDiaCellAttr::CDiaCellAttr(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaCellAttr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaCellAttr)
	//}}AFX_DATA_INIT
	m_iSel=-1;
}


void CDiaCellAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaCellAttr)
	DDX_Control(pDX, IDC_TAG_NAME, m_TagName);
	DDX_Control(pDX, IDC_TAGS, m_TagList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaCellAttr, CDialog)
	//{{AFX_MSG_MAP(CDiaCellAttr)
	ON_LBN_SELCHANGE(IDC_TAGS, OnSelchangeTags)
	ON_BN_CLICKED(IDC_SELTAG, OnButton1)
	ON_LBN_DBLCLK(IDC_TAGS, OnDblclkTags)
	ON_EN_CHANGE(IDC_TAG_NAME, OnChangeTagName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaCellAttr message handlers

void CDiaCellAttr::OnSelchangeTags() 
{	
	CString sTemp1;
	m_iSel=m_TagList.GetCurSel();
	if(m_iSel >= 0){
		m_TagList.GetText(m_iSel,sTemp1);
		sTemp1 = sTemp1.Right(sTemp1.GetLength() - 30);
		m_TagName.SetWindowText(sTemp1);
	}
	m_TagName.SetFocus();
}

BOOL CDiaCellAttr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pCell->GetLableTag(&m_TagList);

	CListBox * pList = (CListBox*)GetDlgItem(IDC_TAGS);
	if(pList->GetCount() > 0){
		m_iSel = 0;
		pList->SetCurSel(m_iSel);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaCellAttr::OnOK() 
{
	m_pCell->SetTag(&m_TagList);
	
	CDialog::OnOK();
}

void CDiaCellAttr::OnButton1() 
{
	CString sTagName;
    CString sTemp,sTemp1;

	if(m_iSel == -1){
		return;
	}
	
    m_TagList.GetText(m_iSel,sTemp1);
    sTemp1=sTemp1.Left(30);
	sTemp = (LPCTSTR)sTemp1 + 30;
	if(m_TagList.GetItemData(m_iSel)){
		CDiaButtonWnd dlg;
		dlg.m_WndFileName=sTemp;
		if(dlg.DoModal()==IDOK){
			sTagName = dlg.m_WndFileName;
			sTemp.Format("%-30s%s",sTemp1,sTagName);
			m_TagList.DeleteString(m_iSel);
			m_TagList.InsertString(m_iSel,sTemp);
			m_TagList.SetCurSel(m_iSel);
		}
		return;
	}
	// TODO: Add your control notification handler code here
	if( !Discover() ){
		return;
	}

	RTK_TAG tag;
	TAG_NAME tName;
	char name[TAGNAME_TEXT_LENGTH + 1];
	if(GetFirstTag(&tag)){
		tName.node = tag.node;
		tName.sname.group = tag.group;
		tName.sname.tag = tag.key;
		tagname_to_text(&tName, name, TAGNAME_TEXT_LENGTH + 1);
		
		sTagName = name;
	}	

	if(m_iSel>-1){
		UpdateData();
        sTemp.Format("%-30s%s",sTemp1,sTagName);
        m_TagList.DeleteString(m_iSel);
        m_TagList.InsertString(m_iSel,sTemp);

		CListBox * pList = (CListBox*)GetDlgItem(IDC_TAGS);
		/*
		m_iSel ++;
		if(m_iSel >= pList->GetCount()){
			m_iSel = 0;
		}
		*/
		pList->SetCurSel(m_iSel);
	}
}

void CDiaCellAttr::OnDblclkTags() 
{
	// TODO: Add your control notification handler code here
	OnButton1();	
}

void CDiaCellAttr::OnChangeTagName() 
{
	// change the selected tag in the list
	CString sTagName;
    CString sTemp,sTemp1;
	char buf[1024];

	if(m_iSel == -1){
		return;
	}
	
    m_TagList.GetText(m_iSel,sTemp1);
    sTemp1=sTemp1.Left(30);
	m_TagName.GetWindowText(sTemp);

	sprintf(buf, "%-30s%s", (LPCTSTR)sTemp1, (LPCTSTR)sTemp);
	m_TagList.DeleteString(m_iSel);
	m_TagList.InsertString(m_iSel, buf);
	m_TagList.SetCurSel(m_iSel);
}
