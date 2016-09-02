#pragma once
#include "afxcmn.h"
#include <vector>

#define RANKCOUNT 10

// CDiaUserManager 对话框

class CDiaUserManager : public CDialog
{
	DECLARE_DYNAMIC(CDiaUserManager)

public:
	CDiaUserManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaUserManager();

// 对话框数据
	enum { IDD = IDD_USERMANAGER };
	CFont userTreeFont; //用户树形控件的字体；

	CStringList groupList;
	CStringList userlist;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_usertree;

private:
	HTREEITEM hRootItem;//根节点
	//HTREEITEM hSubitem;//组节点
	HTREEITEM hSubUseritem;//用户节点
	vector<CString> groupVec;//所有组名
	CString curUerIDStr;//当前组下的用户ID
	vector<CString> userVec;//所有用户名
	vector<CString> curUserVec;//当前组下的用户名
	CString selectItemText;
	CString selectItemId;

	CString m_newUserName;//增加新用户

	//编辑用户
	CString m_userNameEdit;
	CString m_PassWordEdit;

	//---
	void initListCtrl();//初始化listcontrol控件
	void updateListCtrl(CString *itemText);//
	void initTreeInfo();
	void updateTreeInfo();//更新treeview显示
	void getGroupInfo(vector<CString> &strVec);
	void getUserName(CString &groupStr, vector<CString> &userVec);
	void getAllUser(vector<CString> &strVec);
	CString getUserPassWord(CString *str);
	void updateMySqlRank(CString *userStr, CString *rankStr);

	int getNodeName(CString fileName);
	//int getNodeCompared(CString fileName, vector<CString> &strVec, char compare);

	bool isListCtrlChanged; //列表控件是否有改动
	bool isNewUserAdded;//使用新增了用户
	bool isUpdateMysql;//是否更新mysql数据库
	
public:
	CListCtrl m_userank;
	afx_msg void OnNMRClickUsertree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickUsertree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUserDelete();
	afx_msg void OnUserEdit();
	afx_msg void OnUserNew();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedUserrank(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickUserrank(NMHDR *pNMHDR, LRESULT *pResult);
};
