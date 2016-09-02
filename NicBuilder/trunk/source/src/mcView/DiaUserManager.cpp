// DiaUserManager.cpp : 实现文件
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

#define MAX_ALLSECTIONS 500 //定义最大的段长度
#define MAX_SECTION 30 //段的最大长度

using namespace std;

vector<CString> userVec;//用来保存userinfo中所有用户的用户名，调用getNodeName函数前要清空userVec
CString temppassword;

char * userinfoPath = "config\\userinfo.ini";
// CDiaUserManager 对话框

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


// CDiaUserManager 消息处理程序

BOOL CDiaUserManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	isNewUserAdded = false;
	isListCtrlChanged = false;
	isUpdateMysql = false;
	
	initTreeInfo();
	initListCtrl();
	//得到所有的用户名
	//getNodeName(userinfoPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDiaUserManager::initListCtrl()
{
	m_userank.SetBkColor(RGB(0,0,0));
	m_userank.SetTextColor(RGB(255,255,255));
	m_userank.SetTextBkColor(RGB(0,0,0));
	m_userank.SetExtendedStyle(LVS_EX_CHECKBOXES);

	m_userank.InsertColumn(0,"权限",LVCFMT_LEFT,60,-1);
	m_userank.InsertColumn(1,"描述",LVCFMT_LEFT,300,-1);

	std::vector<CString> insertText;

	insertText.push_back("自定义");
	insertText.push_back("自定义");
	insertText.push_back("自定义");
	insertText.push_back("自定义");
	insertText.push_back("自定义");
	insertText.push_back("自定义");
	insertText.push_back("自定义");
	insertText.push_back("禁止切换其他应用");
	insertText.push_back("用户管理");
	insertText.push_back("关闭系统");

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
	userTreeFont.CreatePointFont(100, "宋体" ,NULL);
	m_usertree.SetFont(&userTreeFont);

	m_usertree.DeleteAllItems();
	//创建根节点
	hRootItem = m_usertree.InsertItem(_T("用户管理"), TVI_ROOT);

	//读取groupinfo.ini中的节点的内容（即每个用户的名字），存入到groupVec中
	userVec.clear();
	getNodeName(userinfoPath);

	for (UINT j = 0; j < userVec.size(); j++)
	{
		m_usertree.InsertItem(userVec[j], hRootItem);
	}
}

//得到当前组下的所有的子用户的名字，子用户名都存入到userVec中
/*void CDiaUserManager::getUserName(CString &groupStr, vector<CString> &userVec)
{
	userVec.clear();
    CString id;//用户组的id
	//取groupStr组用户的id的键值
	char id[];
	GetPrivateProfileString(
		groupStr,
		"id",
		_T(""),
		id,		
		sizeof(id),
		".//groupinfo.ini")
		);

    //判断userinfo中所有查找的用户组的子用户名，并存到strVec中
	getNodeCompared(".//userinfo.ini",userVec,id);
}*/	
//得到所有的用户名
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

//取用户str的密码pswd
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

//右键点击弹出对话框
void CDiaUserManager::OnNMRClickUsertree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
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

	if (selectItemText == "用户管理")
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

void CDiaUserManager::OnNMClickUsertree(NMHDR *pNMHDR, LRESULT *pResult)//左键点击用户树
{
	// TODO: 在此添加控件通知处理程序代码
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
	if (selectItemText == "用户管理")
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

//更新list控件内容
void CDiaUserManager::updateListCtrl(CString *itemText)
{
	char userRank[256];
	
	//读取用户的权限，userRank键值
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
	//创建根节点
	HTREEITEM hRootItem = m_usertree.InsertItem(_T("用户管理"), TVI_ROOT);

	userVec.clear();
	getNodeName(userinfoPath);
	for(UINT i = 0; i < userVec.size();i++)
	{
		//为树形控件添加节点
		CString userStr = userVec[i];
		HTREEITEM hSubitem = m_usertree.InsertItem(userStr, hRootItem);
	}
	m_usertree.Expand(hRootItem, TVE_EXPAND);
}

//void CDiaUserManager::OnGroupNew()
//{
//	// TODO: 在此添加命令处理程序代码
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
//	// TODO: 在此添加命令处理程序代码
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
//	// TODO: 在此添加命令处理程序代码
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
//	// TODO: 在此添加命令处理程序代码
//}

void CDiaUserManager::OnUserDelete()
{
	// TODO: 在此添加命令处理程序代码
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
		MessageBox("操作mysql数据库失败");
	}*/


	//CString sqlStr;
	//sqlStr.Format("DELETE FROM userinfo WHERE userName='%s'", selectItemText);
	WritePrivateProfileString(selectItemText,NULL,NULL,userinfoPath); //删除userinfo.ini中的节,既删除一个用户的数据
    
	//updateTreeInfo();
	//getAllUser(userVec);
    userVec.clear();
	getNodeName(userinfoPath);
	hSubUseritem = m_usertree.GetSelectedItem();
	m_usertree.DeleteItem(hSubUseritem);
}

void CDiaUserManager::OnUserEdit()
{
	// TODO: 在此添加命令处理程序代码
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
			MessageBox("用户编辑失败", NULL, MB_OK);
			return;
		}*/
		hSubUseritem = m_usertree.GetSelectedItem();
		m_usertree.SetItemText(hSubUseritem, m_userNameEdit);
	}
}

void CDiaUserManager::OnUserNew()
{
	// TODO: 在此添加命令处理程序代码
	CDiaUserNew dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_newUserName = dlg.m_UserName;
		//CString sqlStr,sqlStr1,sqlStr2,sqlGrpIdStr;
		//CString groupID;
		//CString userIDs;//对应group下的所有用户ID
		//CString UserID;// 新增加的用户ID
		//CString NewUserIDs;//增加新用户后，对应group下的所有用户ID
		

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

		//sqlStr.Format("INSERT INTO userinfo(userName,userRank,psword,groupid,userState,userIP) VALUES('%s','','%s','%s','0','0')", m_newUserName, dlg.m_PassWord, groupID); //a表示没有赋予权限等级
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
	// TODO: 在此添加控件通知处理程序代码
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
    // bChecked表示当前item是否checked       
	*pResult = 0;
}

void CDiaUserManager::updateMySqlRank(CString *userStr, CString *rankStr)//更新用户的权限
{
	//CString sqlStr;
	//bool flag = false;
	//int i = 0;
	//int j = 0;
	//先遍历表中groupinfo中是否有userStr，如果有则取出对应的uersID,执行组批量授权;如果没有就执行else添加单个用户
	//getGroupInfo(groupVec);//取出所有组的名字值赋给groupVec
	//while(i < groupVec.size())
 //   {
	//	if (!strcmp(groupVec.at(i),userStr->GetString()))
	//	{
	//		getUserName(groupVec.at(i), curUserVec);//取出所选中组groupVec[i]所包含的单个的用户名并存到curUserVec中
	//		flag = true;
	//		break;
	//	}
	//	i++;
 //   }

	//flag为ture时是组批量
	//if (flag)//组批量权限
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
	//else//单个用户添加权限
	//{
		//sqlStr.Format("UPDATE userinfo SET userRank='%s' WHERE userName='%s'", *rankStr, *userStr);
	WritePrivateProfileString(*userStr,"userRank",*rankStr,userinfoPath);
	//}

}
void CDiaUserManager::OnNMClickUserrank(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	isUpdateMysql = true;
	*pResult = 0;
}



//mox+++
//取ini中所有的节点名并存在数组中，返回节点的个数
//int CDiaUserManager::getNodeName(CString fileName, vector<CString> &strVec)
//{
//	TCHAR chSectionNames[2048] = {0}; //所有节名组成的字符数组
//	char * pSectionName; //保存找到的某个节名字符串的首地址
//	//GetPrivateProfileSectionNames(chSectionNames, 2048, fileName);//??
//	GetPrivateProfileSectionNames(chSectionNames, 2048, "..//userinfo.ini");
//
//	CString str;
//	int i, j = 0;
//	for (i = 0; i < 2048; i++, j++)
//	{
//		if(chSectionNames[0] == '/0')
//			break; //如果第一个字符就是0，则说明ini中一个节也没有
//		if(chSectionNames[i] == '/0')
//		{
//			pSectionName=&chSectionNames[i-j]; //找到一个0，则说明从这个字符往前，减掉j个偏移量，
//			//就是一个节名的首地址
//			j = -1;
//			str.Format(_T("%s"), pSectionName);
//			strVec.push_back(str);
//
//			if(chSectionNames[i+1] == 0)
//			{
//				break; //当两个相邻的字符都是0时，则所有的节名都已找到，循环终止
//			}
//		}
//	}
//	return strVec.size();
//}

//取ini中的与参数compare相同的键值得节点名，存到strVec中，并返回符合的节点的个数
/*int CDiaUserManager::getNodeCompared(CString fileName, vector<CString> &strVec, char compare)
{
	TCHAR chSectionNames[2048] = {0}; //所有节名组成的字符数组
	char * pSectionName; //保存找到的某个节名字符串的首地址
	GetPrivateProfileSectionNames(chSectionNames, 2048, fileName);

	CString str;
	int i, j = 0;
	for (i = 0; i < 2048; i++, j++)
	{
		if(chSectionNames[0] == '/0')
			break; //如果第一个字符就是0，则说明ini中一个节也没有
		if(chSectionNames[i] == '/0')
		{
			pSectionName=&chSectionNames[i-j]; //找到一个0，则说明从这个字符往前，减掉j个偏移量，
			//就是一个节名的首地址
			j = -1;
			if(chSectionNames[i+1] == 0)
			{
				break; //当两个相邻的字符都是0时，则所有的节名都已找到，循环终止
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

//取ini中所有的节点名并存在数组中，返回节点的个数
int CDiaUserManager::getNodeName(CString fileName)
{
	int i;
	int pos=0;
	CString str;
	char chSectionNames[MAX_ALLSECTIONS]={0};//总的提出来的字符串
	char chSection[MAX_SECTION]={0};//存放一个小节名
	char chKey[MAX_SECTION]={0};
	//LPCTSTER lpFileName="";
	GetPrivateProfileSectionNames(chSectionNames,MAX_ALLSECTIONS,userinfoPath);

	for(i=0;i<MAX_ALLSECTIONS-1;i++)
	{
		if(chSectionNames[i]==0)//得到前面段明
		{
			for(int m=pos;m<=i;m++)
			{
				chSection[m-pos]=chSectionNames[m];//获取小节名
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