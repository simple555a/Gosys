// DiaUserManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mcview.h"
#include "DiaUserManager.h"
#include <commctrl.h>
#include "DiaUserGroupNew.h"
#include "DiaUserNew.h"
#include "DiaUserGroupEdit.h"
#include "DiaUserEdit.h"
//#include "DiaMsgDlg.h"

#define MAX_ALLSECTIONS 500 //�������Ķγ���
#define MAX_SECTION 30 //�ε���󳤶�

using namespace std;

vector<CString> userVec;//��������userinfo�������û����û���������getNodeName����ǰҪ���userVec
CString temppassword;

char * userinfoPath = "config\\userinfo.ini";
// CDiaUserManager �Ի���

IMPLEMENT_DYNAMIC(CDiaUserManager, CDialog)

CDiaUserManager::CDiaUserManager(CWnd* pParent /*=NULL*/)
: CDialog(CDiaUserManager::IDD, pParent)
{
}

CDiaUserManager::~CDiaUserManager()
{
}

void CDiaUserManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERTREE, m_usertree);
	DDX_Control(pDX, IDC_USERRANK, m_userank);
}


BEGIN_MESSAGE_MAP(CDiaUserManager, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_USERTREE, &CDiaUserManager::OnNMRClickUsertree)
	ON_NOTIFY(NM_CLICK, IDC_USERTREE, &CDiaUserManager::OnNMClickUsertree)
	//ON_COMMAND(ID_GROUP_NEW, &CDiaUserManager::OnGroupNew)
	//ON_COMMAND(ID_GROUP_EDIT, &CDiaUserManager::OnGroupEdit)
	//ON_COMMAND(ID_GROUP_DELETE, &CDiaUserManager::OnGroupDelete)
	//ON_COMMAND(ID_GROUP_DELETEALL, &CDiaUserManager::OnGroupDeleteall)
	ON_COMMAND(ID_USER_DELETE, &CDiaUserManager::OnUserDelete)
	ON_COMMAND(ID_USER_EDIT, &CDiaUserManager::OnUserEdit)
	ON_COMMAND(ID_USER_NEW, &CDiaUserManager::OnUserNew)
	ON_BN_CLICKED(IDOK, &CDiaUserManager::OnBnClickedOk)
//	ON_WM_LBUTTONDBLCLK()
//ON_WM_LBUTTONDOWN()
ON_NOTIFY(LVN_ITEMCHANGED, IDC_USERRANK, &CDiaUserManager::OnLvnItemchangedUserrank)
//ON_NOTIFY(LVN_ODSTATECHANGED, IDC_USERRANK, &CDiaUserManager::OnLvnOdstatechangedUserrank)
ON_NOTIFY(NM_CLICK, IDC_USERRANK, &CDiaUserManager::OnNMClickUserrank)
END_MESSAGE_MAP()


// CDiaUserManager ��Ϣ�������

BOOL CDiaUserManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	isNewUserAdded = false;
	isListCtrlChanged = false;
	isUpdateMysql = false;
	
	initTreeInfo();
	initListCtrl();
	//�õ����е��û���
	//getNodeName(userinfoPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDiaUserManager::initListCtrl()
{
	m_userank.SetBkColor(RGB(0,0,0));
	m_userank.SetTextColor(RGB(255,255,255));
	m_userank.SetTextBkColor(RGB(0,0,0));
	m_userank.SetExtendedStyle(LVS_EX_CHECKBOXES);

	m_userank.InsertColumn(0,"Ȩ��",LVCFMT_LEFT,60,-1);
	m_userank.InsertColumn(1,"����",LVCFMT_LEFT,300,-1);

	std::vector<CString> insertText;

	insertText.push_back("�Զ���");
	insertText.push_back("�Զ���");
	insertText.push_back("�Զ���");
	insertText.push_back("�Զ���");
	insertText.push_back("�Զ���");
	insertText.push_back("�Զ���");
	insertText.push_back("�Զ���");
	insertText.push_back("��ֹ�л�����Ӧ��");
	insertText.push_back("�û�����");
	insertText.push_back("�ر�ϵͳ");

	int row = 0;
	for(std::vector<CString>::iterator it = insertText.begin(); it != insertText.end(); it++)
	{
		CString indexText;
		indexText.Format("%2d", row);
		m_userank.InsertItem(row,"");
		m_userank.SetItemText(row, 0, indexText);
		m_userank.SetItemText(row, 1, *it);
		row++;
	}
}

void CDiaUserManager::initTreeInfo()
{
	userTreeFont.CreatePointFont(100, "����" ,NULL);
	m_usertree.SetFont(&userTreeFont);

	m_usertree.DeleteAllItems();
	//�������ڵ�
	hRootItem = m_usertree.InsertItem(_T("�û�����"), TVI_ROOT);

	//��ȡgroupinfo.ini�еĽڵ�����ݣ���ÿ���û������֣������뵽groupVec��
	userVec.clear();
	getNodeName(userinfoPath);

	for (UINT j = 0; j < userVec.size(); j++)
	{
		m_usertree.InsertItem(userVec[j], hRootItem);
	}
}

//�õ���ǰ���µ����е����û������֣����û��������뵽userVec��
/*void CDiaUserManager::getUserName(CString &groupStr, vector<CString> &userVec)
{
	userVec.clear();
    CString id;//�û����id
	//ȡgroupStr���û���id�ļ�ֵ
	char id[];
	GetPrivateProfileString(
		groupStr,
		"id",
		_T(""),
		id,		
		sizeof(id),
		".//groupinfo.ini")
		);

    //�ж�userinfo�����в��ҵ��û�������û��������浽strVec��
	getNodeCompared(".//userinfo.ini",userVec,id);
}*/	
//�õ����е��û���
//void CDiaUserManager::getAllUser(vector<CString> &strVec)
//{
//	strVec.clear();
//	if (mysql_query(&mysqlData, "SELECT userName FROM userinfo") == 0)
//	{
//		MYSQL_RES *mysqlRes = mysql_store_result(&mysqlData);
//		vector<CString> vec;
//		for (int i = 0; i<mysql_num_rows(mysqlRes);i++)
//		{
//			MYSQL_ROW mysqlrow = mysql_fetch_row(mysqlRes);
//			CString str;
//			str.Format(("%s"), mysqlrow[0]);
//			strVec.push_back(str);
//		}
//		mysql_free_result(mysqlRes);
//	}
//}

//ȡ�û�str������pswd
CString CDiaUserManager::getUserPassWord(CString *str)
{
	//sqlStr.Format("SELECT psword from userinfo WHERE userName='%s'", *str);
    char temp[256];
	GetPrivateProfileString(
		*str,
		"psword",
		"error",
		temp,		
		sizeof(temp),
		userinfoPath
		);

	   return temp;
}

//�Ҽ���������Ի���
void CDiaUserManager::OnNMRClickUsertree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	POINT point;
	GetCursorPos(&point);
	::ScreenToClient(pNMTreeView->hdr.hwndFrom, &point);
	UINT uFlags;
	uFlags = TVHT_ONITEMLABEL;
	HTREEITEM hItem = m_usertree.HitTest(point, &uFlags);

	if (hItem != NULL)
	{
		m_usertree.SelectItem(hItem);		
	}
	else 
	{
		return;
	}

	selectItemText =  m_usertree.GetItemText(hItem);
	isUpdateMysql = false;
	updateListCtrl(&selectItemText);
	
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_USERMANAGER));
	CMenu* popmenu;

	::ClientToScreen(pNMTreeView->hdr.hwndFrom, &point);

	if (selectItemText == "�û�����")
	{
		//m_userank.EnableWindow(FALSE);
		popmenu = menu.GetSubMenu(0);
		popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
		return;
	}
	else
	{
		/*for (UINT i = 0; i < groupVec.size(); i++)
		{
			if (selectItemText == groupVec[i])
			{
				m_userank.EnableWindow(FALSE);
				popmenu = menu.GetSubMenu(1);
				popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
				return;
			}
		}*/

		for (UINT j = 0;j < userVec.size();j++)
		{
			if (selectItemText == userVec[j])
			{
				//m_userank.EnableWindow(TRUE);
				popmenu = menu.GetSubMenu(1);
				popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
			}
		}
	}
	return;
}

void CDiaUserManager::OnNMClickUsertree(NMHDR *pNMHDR, LRESULT *pResult)//�������û���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	
	NM_TREEVIEW*pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	POINT point;
	GetCursorPos(&point);
	::ScreenToClient(pNMTreeView->hdr.hwndFrom, &point);
	UINT uFlags;
	uFlags = TVHT_ONITEMLABEL;
	HTREEITEM hitem = m_usertree.HitTest(point, &uFlags);

	if (hitem != NULL)
	{
		m_usertree.SelectItem(hitem);
	}
	else
	{
		return;
	}

	selectItemText = m_usertree.GetItemText(hitem);
	isUpdateMysql = false;
	updateListCtrl(&selectItemText);
	if (selectItemText == "�û�����")
	{
		//m_userank.EnableWindow(FALSE);
		return;
	}
	else
	{
		for (UINT j = 0;j < userVec.size();j++)
		{
			if (selectItemText == userVec[j])
			{
				m_userank.EnableWindow(TRUE);
			}
		}
	}
}

//����list�ؼ�����
void CDiaUserManager::updateListCtrl(CString *itemText)
{
	char userRank[256];
	
	//��ȡ�û���Ȩ�ޣ�userRank��ֵ
	GetPrivateProfileString(
		*itemText,
		"userRank",
		"error",
		userRank,		
		sizeof(userRank),
		userinfoPath
		);
	///
	for (int a = 0; a < RANKCOUNT; a++)
	{
		m_userank.SetCheck(a,FALSE);
	}

	///
	CString userRankRow = userRank;
	if (userRankRow == _T("error"))
	{ 
	    return;
	}
			
	CArgs args;
	parse_arg_ey(userRank,&args,"*");

	for (int i=0;i<args.argc;i++)
	{
		int j = atoi(args.argv[i]);
		m_userank.SetCheck(j, TRUE);
	}
} 

void CDiaUserManager::updateTreeInfo()
{
	m_usertree.DeleteAllItems();
	//�������ڵ�
	HTREEITEM hRootItem = m_usertree.InsertItem(_T("�û�����"), TVI_ROOT);

	userVec.clear();
	getNodeName(userinfoPath);
	for(UINT i = 0; i < userVec.size();i++)
	{
		//Ϊ���οؼ���ӽڵ�
		CString userStr = userVec[i];
		HTREEITEM hSubitem = m_usertree.InsertItem(userStr, hRootItem);
	}
	m_usertree.Expand(hRootItem, TVE_EXPAND);
}

//void CDiaUserManager::OnGroupNew()
//{
//	// TODO: �ڴ���������������
//	CDiaUserGroupNew dlg;
//	if (dlg.DoModal() == IDOK)
//	{
//		sqlStr.Format("INSERT INTO groupinfo(groupName,userID) VALUES('%s','')", dlg.groupName);
//        
//
//		if (mysql_query(&mysqlData, qroupNameChar) == 0)
//		{
//			//updateTreeInfo();
//			groupVec.push_back(dlg.groupName);
//			m_usertree.InsertItem(dlg.groupName, hRootItem);
//		}
//	}
//}
//
//void CDiaUserManager::OnGroupEdit()
//{
//	// TODO: �ڴ���������������
//	CDiaGroupEdit dlg;
//	dlg.m_newgroupname = selectItemText;
//	if (dlg.DoModal() == IDOK)
//	{
//		CString sqlStr;
//		sqlStr.Format("UPDATE groupinfo SET groupName='%s' WHERE groupName='%s'", dlg.groupName, selectItemText);
//		const char *qroupNameChar = (LPSTR)(LPCTSTR)sqlStr;
//		if (mysql_query(&mysqlData, qroupNameChar) == 0)
//		{
//			//updateTreeInfo();
//			getGroupInfo(groupVec);
//			hSubitem = m_usertree.GetSelectedItem();
//			m_usertree.SetItemText(hSubitem, dlg.m_newgroupname);
//		}
//	}
//}
//
//void CDiaUserManager::OnGroupDelete()
//{
//	// TODO: �ڴ���������������
//	//sqlStr.Format("DELETE FROM groupinfo WHERE groupName='%s'", selectItemText);
//	
//    WritePrivateProfileString(selectItemText,NULL,NULL,".//groupinfo.ini"); 
//	//updateTreeInfo();
//	getNodeName(".//groupinfo.ini", groupVec);
//	hSubitem = m_usertree.GetSelectedItem();
//	m_usertree.DeleteItem(hSubitem);
//}
//
//void CDiaUserManager::OnGroupDeleteall()
//{
//	// TODO: �ڴ���������������
//}

void CDiaUserManager::OnUserDelete()
{
	// TODO: �ڴ���������������
	/*CString deleteUserId, deleteUserGrpId, userIDs;
	CString sqlDeleteUserIdStr;
	sqlDeleteUserIdStr.Format("SELECT id FROM userinfo WHERE userName='%s'", selectItemText);
	const char *sqlDeleteGrpIdChar = (LPSTR)(LPCTSTR)sqlDeleteUserIdStr;
	if (mysql_query(&mysqlData, sqlDeleteGrpIdChar) == 0)
	{
		MYSQL_RES *deUserIdes = mysql_store_result(&mysqlData);
		if (mysql_num_rows(deUserIdes))
		{
			MYSQL_ROW deUserIdRow = mysql_fetch_row(deUserIdes);
			deleteUserId.Format("%s", deUserIdRow[0]);
		}
		mysql_free_result(deUserIdes);
	}

	CString sqlDeleteUserGrpIdStr;
	sqlDeleteUserGrpIdStr.Format("SELECT groupid FROM userinfo WHERE userName='%s'", selectItemText);
	const char *sqlDeleteUserGrpIdChar = (LPSTR)(LPCTSTR)sqlDeleteUserGrpIdStr;
	if (mysql_query(&mysqlData, sqlDeleteUserGrpIdChar) == 0)
	{
		MYSQL_RES *deUserGrpIdes = mysql_store_result(&mysqlData);
		if (mysql_num_rows(deUserGrpIdes))
		{
			MYSQL_ROW deUserGrpIdRow = mysql_fetch_row(deUserGrpIdes);
			deleteUserGrpId.Format("%s", deUserGrpIdRow[0]);
		}
		mysql_free_result(deUserGrpIdes);
	}

	CString sqlUserIdStr1;
	sqlUserIdStr1.Format("SELECT userID FROM groupinfo WHERE id='%s'", deleteUserGrpId);
	const char *sqlUserIDChar = (LPSTR)(LPCTSTR)sqlUserIdStr1;
	if (mysql_query(&mysqlData, sqlUserIDChar) == 0)
	{
		MYSQL_RES *userIDRES = mysql_store_result(&mysqlData);
		if (mysql_num_rows(userIDRES))
		{
			MYSQL_ROW userIDRow = mysql_fetch_row(userIDRES);
			userIDs.Format("%s", userIDRow[0]);
		}
		mysql_free_result(userIDRES);
	}

	CArgs args;
	CString newUserIDs;
	parse_arg_ex(userIDs, &args, "*");
	for (int i=0; i<args.argc; i++)
	{
		if (args.argv[i] != deleteUserId)
		{
			newUserIDs.Format("%s*%s", newUserIDs, args.argv[i]);
		}
	}
	newUserIDs.TrimLeft("*");
	CString sqlupStr;
	sqlupStr.Format("UPDATE groupinfo SET userID='%s' WHERE id='%s'", newUserIDs, deleteUserGrpId);
	const char *sqlupChar = (LPSTR)(LPCTSTR)sqlupStr;
	if (mysql_query(&mysqlData, sqlupChar) != 0)
	{
		MessageBox("����mysql���ݿ�ʧ��");
	}*/


	//CString sqlStr;
	//sqlStr.Format("DELETE FROM userinfo WHERE userName='%s'", selectItemText);
	WritePrivateProfileString(selectItemText,NULL,NULL,userinfoPath); //ɾ��userinfo.ini�еĽ�,��ɾ��һ���û�������
    
	//updateTreeInfo();
	//getAllUser(userVec);
    userVec.clear();
	getNodeName(userinfoPath);
	hSubUseritem = m_usertree.GetSelectedItem();
	m_usertree.DeleteItem(hSubUseritem);
}

void CDiaUserManager::OnUserEdit()
{
	// TODO: �ڴ���������������
	CDiaUserEdit dlg;
	CString pswStr;
	temppassword = getUserPassWord(&selectItemText);
	dlg.m_userName = selectItemText;

	if (dlg.DoModal() == IDOK)
	{
		m_userNameEdit = dlg.m_UserNameNew;
		m_PassWordEdit = dlg.m_PassWordNew;
		
		WritePrivateProfileString(selectItemText,"psword",m_PassWordEdit,userinfoPath);
		//const char *sqlChar = (LPSTR)(LPCTSTR)sqlStr;
	    /*if (mysql_query(&mysqlData, sqlChar) != 0)
		{
			MessageBox("�û��༭ʧ��", NULL, MB_OK);
			return;
		}*/
		hSubUseritem = m_usertree.GetSelectedItem();
		m_usertree.SetItemText(hSubUseritem, m_userNameEdit);
	}
}

void CDiaUserManager::OnUserNew()
{
	// TODO: �ڴ���������������
	CDiaUserNew dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_newUserName = dlg.m_UserName;
		//CString sqlStr,sqlStr1,sqlStr2,sqlGrpIdStr;
		//CString groupID;
		//CString userIDs;//��Ӧgroup�µ������û�ID
		//CString UserID;// �����ӵ��û�ID
		//CString NewUserIDs;//�������û��󣬶�Ӧgroup�µ������û�ID
		

		/*sqlGrpIdStr.Format("SELECT id FROM groupinfo WHERE groupName = '%s'", selectItemText);
		const char *grpIDChar = (LPSTR)(LPCTSTR)sqlGrpIdStr;
		if (mysql_query(&mysqlData, grpIDChar) == 0)
		{
			MYSQL_RES *grpIdDes = mysql_store_result(&mysqlData);
			if (mysql_num_rows(grpIdDes))
			{
				MYSQL_ROW grpId = mysql_fetch_row(grpIdDes);
				groupID.Format("%s", grpId[0]);
			}
			mysql_free_result(grpIdDes);
		}*/

		//sqlStr.Format("INSERT INTO userinfo(userName,userRank,psword,groupid,userState,userIP) VALUES('%s','','%s','%s','0','0')", m_newUserName, dlg.m_PassWord, groupID); //a��ʾû�и���Ȩ�޵ȼ�
		//sqlStr1.Format("SELECT userID FROM groupinfo WHERE groupName = '%s'", selectItemText);

		//int temp = mysql_query(&mysqlData, userNameChar);
		/*if (temp != 0)
		{
			const char* errorChar = mysql_error(&mysqlData);
			unsigned int errorNum = mysql_errno(&mysqlData);
		}*/

		//if (temp == 0)
		////{
		//	if (mysql_query(&mysqlData, usersIDChar) == 0)
		//	{
		//		MYSQL_RES *idsRES = mysql_store_result(&mysqlData);
		//		if (mysql_num_rows(idsRES))
		//		{
		//			MYSQL_ROW idsChar = mysql_fetch_row(idsRES);
		//			userIDs.Format("%s", idsChar[0]);

		//			sqlStr2.Format("SELECT id FROM userinfo WHERE userName='%s'", m_newUserName);
		//			const char *userIDChar = (LPSTR)(LPCTSTR)sqlStr2;
		//			if (mysql_query(&mysqlData, userIDChar) == 0)
		//			{
		//				MYSQL_RES *idRES= mysql_store_result(&mysqlData);
		//				if (mysql_num_rows(idRES))
		//				{
		//					MYSQL_ROW idChar = mysql_fetch_row(idRES);
		//					UserID.Format("%s", idChar[0]);
		//				}
		//				//mysql_free_result(idRES);
		//			}
		//		}
		//		//mysql_free_result(idsRES);
		//	//}

			//NewUserIDs.Format("%s%s%s", userIDs, "*", UserID);
			//NewUserIDs.TrimLeft("*");
			//CString newUserIDsStr;
			//newUserIDsStr.Format("UPDATE groupinfo SET userID='%s' WHERE groupName='%s'", NewUserIDs, selectItemText);
			WritePrivateProfileString(m_newUserName,"psword",dlg.m_PassWord,userinfoPath);
			WritePrivateProfileString(m_newUserName,"userRank","",userinfoPath);

			userVec.clear();
			getNodeName(userinfoPath);
			hRootItem = m_usertree.GetSelectedItem();
			m_usertree.InsertItem(m_newUserName, hRootItem);
		//}
		isNewUserAdded = true;
	}
}

void CDiaUserManager::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDiaUserManager::OnLvnItemchangedUserrank(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;   
    //*pResult = 0;   
       
    if (pNMListView->uOldState == 0 && pNMListView->uNewState == 0)   
        return; // No change   
       
    BOOL bPrevState = (BOOL)(((pNMListView->uOldState &    
        LVIS_STATEIMAGEMASK)>>12)-1);   // Old check box state   
    if (bPrevState < 0) // On startup there's no previous state    
        bPrevState = 0; // so assign as false (unchecked)   
       
    // New check box state   
    BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);      
    if (bChecked < 0) // On non-checkbox notifications assume false   
        bChecked = 0;    
       
    if (bPrevState == bChecked) // No change in check box   
        return; 

	if (isUpdateMysql == false)
		return;

	CString rankStr;
	for (int a = 0; a < RANKCOUNT; a++)
	{
		int b = m_userank.GetCheck(a);
		if (m_userank.GetCheck(a))
		{
			//int itemNum = pNMListView->iItem;
			CString itemNumChar;
			itemNumChar.Format("%s%d", "*",a);
			rankStr.Append(itemNumChar);
		}
	}
	rankStr.TrimLeft("*");
	updateMySqlRank(&selectItemText, &rankStr);
    // bChecked��ʾ��ǰitem�Ƿ�checked       
	*pResult = 0;
}

void CDiaUserManager::updateMySqlRank(CString *userStr, CString *rankStr)//�����û���Ȩ��
{
	//CString sqlStr;
	//bool flag = false;
	//int i = 0;
	//int j = 0;
	//�ȱ�������groupinfo���Ƿ���userStr���������ȡ����Ӧ��uersID,ִ����������Ȩ;���û�о�ִ��else��ӵ����û�
	//getGroupInfo(groupVec);//ȡ�������������ֵ����groupVec
	//while(i < groupVec.size())
 //   {
	//	if (!strcmp(groupVec.at(i),userStr->GetString()))
	//	{
	//		getUserName(groupVec.at(i), curUserVec);//ȡ����ѡ����groupVec[i]�������ĵ������û������浽curUserVec��
	//		flag = true;
	//		break;
	//	}
	//	i++;
 //   }

	//flagΪtureʱ��������
	//if (flag)//������Ȩ��
	////{
	//	while(j < curUserVec.size())
	//	{
	//	    sqlStr.Format("UPDATE userinfo SET userRank='%s' WHERE userName='%s'", *rankStr, curUserVec.at(j));
	//		const char *sqlchar = (LPSTR)(LPCTSTR)sqlStr;
	//		if (mysql_query(&mysqlData, sqlchar) != 0)
	//		{
	//			return;
	//		}
	//		j++;
	//	}
	//}
	//else//�����û����Ȩ��
	//{
		//sqlStr.Format("UPDATE userinfo SET userRank='%s' WHERE userName='%s'", *rankStr, *userStr);
	WritePrivateProfileString(*userStr,"userRank",*rankStr,userinfoPath);
	//}

}
void CDiaUserManager::OnNMClickUserrank(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	isUpdateMysql = true;
	*pResult = 0;
}



//mox+++
//ȡini�����еĽڵ��������������У����ؽڵ�ĸ���
//int CDiaUserManager::getNodeName(CString fileName, vector<CString> &strVec)
//{
//	TCHAR chSectionNames[2048] = {0}; //���н�����ɵ��ַ�����
//	char * pSectionName; //�����ҵ���ĳ�������ַ������׵�ַ
//	//GetPrivateProfileSectionNames(chSectionNames, 2048, fileName);//??
//	GetPrivateProfileSectionNames(chSectionNames, 2048, "..//userinfo.ini");
//
//	CString str;
//	int i, j = 0;
//	for (i = 0; i < 2048; i++, j++)
//	{
//		if(chSectionNames[0] == '/0')
//			break; //�����һ���ַ�����0����˵��ini��һ����Ҳû��
//		if(chSectionNames[i] == '/0')
//		{
//			pSectionName=&chSectionNames[i-j]; //�ҵ�һ��0����˵��������ַ���ǰ������j��ƫ������
//			//����һ���������׵�ַ
//			j = -1;
//			str.Format(_T("%s"), pSectionName);
//			strVec.push_back(str);
//
//			if(chSectionNames[i+1] == 0)
//			{
//				break; //���������ڵ��ַ�����0ʱ�������еĽ��������ҵ���ѭ����ֹ
//			}
//		}
//	}
//	return strVec.size();
//}

//ȡini�е������compare��ͬ�ļ�ֵ�ýڵ������浽strVec�У������ط��ϵĽڵ�ĸ���
/*int CDiaUserManager::getNodeCompared(CString fileName, vector<CString> &strVec, char compare)
{
	TCHAR chSectionNames[2048] = {0}; //���н�����ɵ��ַ�����
	char * pSectionName; //�����ҵ���ĳ�������ַ������׵�ַ
	GetPrivateProfileSectionNames(chSectionNames, 2048, fileName);

	CString str;
	int i, j = 0;
	for (i = 0; i < 2048; i++, j++)
	{
		if(chSectionNames[0] == '/0')
			break; //�����һ���ַ�����0����˵��ini��һ����Ҳû��
		if(chSectionNames[i] == '/0')
		{
			pSectionName=&chSectionNames[i-j]; //�ҵ�һ��0����˵��������ַ���ǰ������j��ƫ������
			//����һ���������׵�ַ
			j = -1;
			if(chSectionNames[i+1] == 0)
			{
				break; //���������ڵ��ַ�����0ʱ�������еĽ��������ҵ���ѭ����ֹ
			}
			else
			{ 
				GetPrivateProfileString(pSectionName,"groupid","Error",id,20,fileName);
				if (!strcmp(compare, id) == 0)
				{
					str.Format(_T("%s"), chSectionNames[i]);
					strVec.push_back(str);
				}
			}
		}
	}
	return i;
}*/

//ȡini�����еĽڵ��������������У����ؽڵ�ĸ���
int CDiaUserManager::getNodeName(CString fileName)
{
	int i;
	int pos=0;
	CString str;
	char chSectionNames[MAX_ALLSECTIONS]={0};//�ܵ���������ַ���
	char chSection[MAX_SECTION]={0};//���һ��С����
	char chKey[MAX_SECTION]={0};
	//LPCTSTER lpFileName="";
	GetPrivateProfileSectionNames(chSectionNames,MAX_ALLSECTIONS,userinfoPath);

	for(i=0;i<MAX_ALLSECTIONS-1;i++)
	{
		if(chSectionNames[i]==0)//�õ�ǰ�����
		{
			for(int m=pos;m<=i;m++)
			{
				chSection[m-pos]=chSectionNames[m];//��ȡС����
			}
			str.Format("%s",chSection);
			userVec.push_back(str);

			if(chSectionNames[i+1]!=0)
			{
				pos=i+1;
			}
			else
			{
				break;
			}
		}
	}
	return userVec.size();
}