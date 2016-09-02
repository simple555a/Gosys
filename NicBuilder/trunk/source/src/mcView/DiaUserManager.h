#pragma once
#include "afxcmn.h"
#include <vector>

#define RANKCOUNT 10

// CDiaUserManager �Ի���

class CDiaUserManager : public CDialog
{
	DECLARE_DYNAMIC(CDiaUserManager)

public:
	CDiaUserManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaUserManager();

// �Ի�������
	enum { IDD = IDD_USERMANAGER };
	CFont userTreeFont; //�û����οؼ������壻

	CStringList groupList;
	CStringList userlist;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_usertree;

private:
	HTREEITEM hRootItem;//���ڵ�
	//HTREEITEM hSubitem;//��ڵ�
	HTREEITEM hSubUseritem;//�û��ڵ�
	vector<CString> groupVec;//��������
	CString curUerIDStr;//��ǰ���µ��û�ID
	vector<CString> userVec;//�����û���
	vector<CString> curUserVec;//��ǰ���µ��û���
	CString selectItemText;
	CString selectItemId;

	CString m_newUserName;//�������û�

	//�༭�û�
	CString m_userNameEdit;
	CString m_PassWordEdit;

	//---
	void initListCtrl();//��ʼ��listcontrol�ؼ�
	void updateListCtrl(CString *itemText);//
	void initTreeInfo();
	void updateTreeInfo();//����treeview��ʾ
	void getGroupInfo(vector<CString> &strVec);
	void getUserName(CString &groupStr, vector<CString> &userVec);
	void getAllUser(vector<CString> &strVec);
	CString getUserPassWord(CString *str);
	void updateMySqlRank(CString *userStr, CString *rankStr);

	int getNodeName(CString fileName);
	//int getNodeCompared(CString fileName, vector<CString> &strVec, char compare);

	bool isListCtrlChanged; //�б�ؼ��Ƿ��иĶ�
	bool isNewUserAdded;//ʹ���������û�
	bool isUpdateMysql;//�Ƿ����mysql���ݿ�
	
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
