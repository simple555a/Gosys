// DlgSelectKName.cpp : implementation file
//

#include "stdafx.h"
#include "mcview.h"
#include "DlgSelectKName.h"


// CDlgSelectKName dialog

IMPLEMENT_DYNAMIC(CDlgSelectKName, CDialog)

CDlgSelectKName::CDlgSelectKName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectKName::IDD, pParent)
{

}

CDlgSelectKName::~CDlgSelectKName()
{
}

void CDlgSelectKName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTKEYNAME, m_KeyObjects);
	DDX_Text(pDX, IDC_EDIT1, m_selstr);
}


BEGIN_MESSAGE_MAP(CDlgSelectKName, CDialog)
	ON_BN_CLICKED(IDC_REFRESH, &CDlgSelectKName::OnBnClickedRefresh)
	ON_BN_CLICKED(IDOK, &CDlgSelectKName::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LISTKEYNAME, &CDlgSelectKName::OnLbnDblclkListkeyname)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSelectKName::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgSelectKName message handlers

BOOL CDlgSelectKName::OnInitDialog()
{
	// TODO:  在此添加额外的初始化
	CDialog::OnInitDialog();
	CString NodeName;
	CString GroupName;
	CString TagName;
	lock_rtdb(__false, 500);
	RTK_CURSOR hNode;
	PRTK_NODE pNode;
	PRTK_GROUP pGroup;
	RTK_CURSOR hGroup;
	PRTK_TAG pTag;
	RTK_CURSOR hTag;

	hNode = open_first_node();
	while( !cursor_is_end(hNode) ){
		pNode = (PRTK_NODE)cursor_get_item(hNode);

		hGroup = cursor_open_first_subitem(hNode);
		while(!cursor_is_end(hGroup)){
			pGroup = (PRTK_GROUP)cursor_get_item(hGroup);

			hTag = cursor_open_first_subitem(hGroup);
			while(!cursor_is_end(hTag)){
				pTag = (PRTK_TAG)cursor_get_item(hTag);
				char u1[rtkm_group_key_length] = "U1";
				char u2[rtkm_group_key_length] = "U2";
				char u9[rtkm_group_key_length] = "U9";
				//				if (stricmp(pTag- >group.Data,u1)==0 || stricmp(pTag->group.Data,u2)==0 || stricmp(pTag->group.Data,u9)==0)
				//				if (stricmp(pTag->group.Data,u1)==0 || stricmp(pTag->group.Data,u2)==0 || stricmp(pTag->group.Data,u9)==0)
				//				{
				TagName = TagName+pTag->node.Data + "." +pTag->group.Data +"." +pTag->key.Data;
				m_KeyObjects.AddString(TagName);
				TagName.Empty();
				cursor_move_next(hTag);
			}
			cursor_move_next(hGroup);
		}
		cursor_move_next(hNode);
	}
	UpdateData(false);
	unlock_rtdb();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSelectKName::OnBnClickedRefresh()
{
	// TODO: Add your control notification handler code here
	CString NodeName;
	CString GroupName;
	CString TagName;
	lock_rtdb(__false, 500);
	RTK_CURSOR hNode;
	PRTK_NODE pNode;
	PRTK_GROUP pGroup;
	RTK_CURSOR hGroup;
	PRTK_TAG pTag;
	RTK_CURSOR hTag;

	m_KeyObjects.ResetContent();

	hNode = open_first_node();
	while( !cursor_is_end(hNode) ){
		pNode = (PRTK_NODE)cursor_get_item(hNode);

		hGroup = cursor_open_first_subitem(hNode);
		while(!cursor_is_end(hGroup)){
			pGroup = (PRTK_GROUP)cursor_get_item(hGroup);

			hTag = cursor_open_first_subitem(hGroup);
			while(!cursor_is_end(hTag)){
				pTag = (PRTK_TAG)cursor_get_item(hTag);
				TagName = TagName+pTag->node.Data + "." +pTag->group.Data +"." +pTag->key.Data;
				m_KeyObjects.AddString(TagName);
				TagName.Empty();
				cursor_move_next(hTag);
			}
			cursor_move_next(hGroup);
		}
		cursor_move_next(hNode);
	}
	UpdateData(false);
	unlock_rtdb();
}

void CDlgSelectKName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(m_KeyObjects.GetCurSel() == LB_ERR)
	{
		MessageBox("请选择对象");
		return;
	}
	OnOK();
}

void CDlgSelectKName::OnLbnDblclkListkeyname()
{
	// TODO: Add your control notification handler code here
    OnBnClickedOk();
}

void CDlgSelectKName::OnBnClickedButton1()//搜索
{
	// TODO: Add your control notification handler code here
	//CAtlRegExp<> re;
	//CAtlREMatchContext<> mc;

	//char* szIn = "";
	//char szMatch[128];
	//memset(szMatch,'\0',128);
	//re.Parse(m_selstr);//要匹配的字符
	//for (int i = 0; i < m_KeyObjects.GetCount(); i++)
	//{

        //szIn = m_KeyObjects.GetCurSel(i);
		//if(re.Match(szIn,&mc,&szIn))
		//{
            //szIn
			//cout << szIn << endl;
		//}
	//}
	int sel = m_KeyObjects.SelectString(0, m_selstr);
}
