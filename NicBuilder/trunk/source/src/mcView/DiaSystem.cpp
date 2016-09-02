//===Update record==============================================================================
// 
// mcView.exe version            updated for              date                   author
// v1.2.0.9                     (1-1)                     2015-6-5               Zander SUN

// DiaSystem.cpp : implementation file
//
#include "stdafx.h"
#include "mcview.h"
#include "DiaSystem.h"
#include "DiaPassword.h"
#include "fview.h"
#include "dlgabout.h"
#include "keyhook.h"
#include "shellapi.h"
#include "DiaUserManager.h"
#include "DiaInputD.h"
#include "rtkgui.h"
#include <iostream>
#include <fstream>
//#include "Print.h"
#include "ScreenShutter.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiaSystem dialog
extern CFView theView;
extern HHOOK hKeyboardLL;
extern BOOL gbNoSwitch;
CString m_sCurUser;
extern char * userinfoPath;
CDiaInputD czDiaInputD;
extern CString fake_TagName;
CString strCls1;
//HHOOK hKeyboardLL=NULL;

//2015.08.26 GM
UINT ThreadPrint1(LPVOID pParam);

CDiaSystem::CDiaSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaSystem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaSystem)
	m_rank = 0;
	m_noSwitch = FALSE;
	//}}AFX_DATA_INIT
	//for(int i=0;i<10;i++){
	//	m_password[i].Empty();
	//}
	//LoadPassword();

	m_noSwitch=gbNoSwitch;
	//if(NULL!=hKeyboardLL) m_noSwitch = TRUE;
}


void CDiaSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaSystem)
	DDX_Control(pDX, IDC_SW2, m_sw2);
	DDX_Text(pDX, IDC_USER, m_loginUserName);
	DDX_Text(pDX, IDC_PWD, m_loginPswd);
	DDX_Check(pDX, IDC_NOSWITCH, m_noSwitch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaSystem, CDialog)
	//{{AFX_MSG_MAP(CDiaSystem)
	ON_BN_CLICKED(IDC_RETURN_WND, OnReturnWnd)
	ON_BN_CLICKED(IDC_PRINT, OnPrint)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_NOSWITCH, OnNoswitch)
	ON_LBN_DBLCLK(IDC_SW2, OnDblclkSw2)
	ON_WM_KEYDOWN()
	ON_WM_CHARTOITEM()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USERMANAGER, &CDiaSystem::OnBnClickedUsermanager)
	ON_BN_CLICKED(IDC_LOGIN, &CDiaSystem::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_LOGOUT, &CDiaSystem::OnBnClickedLogout)
	ON_BN_CLICKED(IDOK, &CDiaSystem::OnBnClickedOk)
	ON_STN_CLICKED(IDC_CURRENTUSER, &CDiaSystem::OnStnClickedCurrentuser)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaSystem::OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiaSystem message handlers

void CDiaSystem::OnCancel() 
{
	CDialog::OnCancel();
}


/*void CDiaSystem::OnRank0() 
{
	SetDlgItemText(IDC_ERR,"");
	SetDlgItemText(IDC_PWD,"");
	m_rank=0;
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank1() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<1){
		if(0==m_password[1].Compare(sPwd)){
			m_rank=1;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=1;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank2() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<2){
		if(0==m_password[2].Compare(sPwd)){
			m_rank=2;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=2;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank3() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<3){
		if(0==m_password[3].Compare(sPwd)){
			m_rank=3;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=3;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank4() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<4){
		if(0==m_password[4].Compare(sPwd)){
			m_rank=4;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=4;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank5() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<5){
		if(0==m_password[5].Compare(sPwd)){
			m_rank=5;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=5;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank6() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<6){
		if(0==m_password[6].Compare(sPwd)){
			m_rank=6;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=6;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank7() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<7){
		if(0==m_password[7].Compare(sPwd)){
			m_rank=7;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=7;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank8() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<8){
		if(0==m_password[8].Compare(sPwd)){
			m_rank=8;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=8;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}

void CDiaSystem::OnRank9() 
{
	CString sPwd;
	GetDlgItemText(IDC_PWD,sPwd);

	SetDlgItemText(IDC_ERR,"");
	if(m_rank<9){
		if(0==m_password[9].Compare(sPwd)){
			m_rank=9;
		}else{
			SetDlgItemText(IDC_ERR,"�������");
		}
	}else{
		m_rank=9;
	}
	SetDlgItemText(IDC_PWD,"");
	
	UpdateData(FALSE);
	SetButton();
}*/



extern CMcviewApp theApp;
BOOL CDiaSystem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_CURRENTUSER, m_sCurUser);
	UpdateData(TRUE);
	if (m_sCurUser.IsEmpty())
	{
		CWnd *pwnd2;
		pwnd2=GetDlgItem(IDC_LOGOUT);
		pwnd2->EnableWindow(FALSE);	
		pwnd2=GetDlgItem(IDC_LOGIN);
		pwnd2->EnableWindow(TRUE);
	} 
	else
	{
		CWnd *pwnd2;
		pwnd2=GetDlgItem(IDC_LOGOUT);
		pwnd2->EnableWindow(TRUE);	
		pwnd2=GetDlgItem(IDC_LOGIN);
		pwnd2->EnableWindow(FALSE);
		//pwnd2->GetDlgItem(IDC_CURRENTUSER);//mox???
	}

	if (gbNoSwitch)
	{
		CWnd *pwnd2;
		pwnd2=GetDlgItem(IDC_SW2);
		pwnd2->EnableWindow(FALSE);
	}
	else
	{
		CWnd *pwnd2;
		pwnd2=GetDlgItem(IDC_SW2);
		pwnd2->EnableWindow(TRUE);
	}

	// load sw2 app
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	CString sTemp;
	CString sPath;
	sPath = theApp.m_path + "sw2\\*.lnk";
	hFind = FindFirstFile((LPCTSTR)sPath, &FindFileData);
	if(hFind!=INVALID_HANDLE_VALUE){
		sTemp=FindFileData.cFileName;
		sTemp=sTemp.Left(sTemp.GetLength()-4);
		m_sw2.AddString(sTemp);
		while(FindNextFile(hFind,&FindFileData)){
			sTemp=FindFileData.cFileName;
			sTemp=sTemp.Left(sTemp.GetLength()-4);
			m_sw2.AddString(sTemp);
		}
		FindClose(hFind);
	}

	SetButton();
		
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaSystem::SetButton()//Ȩ�����ã����ΰ�ť
{
	/*BOOL en;
	if(m_rank<9) en=FALSE;
	else en=TRUE;

	CWnd *pwnd;
	pwnd=GetDlgItem(IDC_RETURN_WND);
	pwnd->EnableWindow(en);
	pwnd=GetDlgItem(IDC_CHG_PWD);
	pwnd->EnableWindow(en);
	pwnd=GetDlgItem(IDC_PRINT);
	pwnd->EnableWindow(m_rank >=0? TRUE : FALSE);
	pwnd=GetDlgItem(IDC_NOSWITCH);
	pwnd->EnableWindow(en);
	pwnd=GetDlgItem(IDC_SW2);
	pwnd->EnableWindow(en && !m_noSwitch);*/
    
	//Ĭ��m_rank==0
	CWnd *wnd  = AfxGetMainWnd();//��ȡmcviewdlg��ָ��
	if ((m_rank & 0x01) == 0)
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//�л���windows
	}
	else
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(TRUE);//�л���windows
	}

	//m_rank==1
	if ((m_rank & 0x01<<1) == 0)
	{
		//wnd->GetDlgItem(IDC_HJHG)->EnableWindow(FALSE);//�������		
	}
	else
	{
		//wnd->GetDlgItem(IDC_HJHG)->EnableWindow(TRUE);//�������	
	}

    //m_rank==2
	if ((m_rank & 0x01<<2) == 0)
	{
		//wnd->GetDlgItem(IDC_COMMUNICATION_MANAGE)->EnableWindow(FALSE);//��Ϣ����
	}
	else
	{
		//wnd->GetDlgItem(IDC_COMMUNICATION_MANAGE)->EnableWindow(TRUE);//��Ϣ����
	}

	//m_rank==3
	if ((m_rank & 0x01<<3) == 0)
	{
		//wnd->GetDlgItem(IDC_REPORT_CHECK)->EnableWindow(FALSE);//��������
	}
	else
	{
		//wnd->GetDlgItem(IDC_REPORT_CHECK)->EnableWindow(TRUE);//��������
	}

	//m_rank==4
	if ((m_rank & 0x01<<4) == 0)
	{
		//wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(FALSE);//����ȷ�ϣ�������¼
	}
	else
	{
		//wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(TRUE);//����ȷ�ϣ�������¼
	}

	//m_rank==5
	if ((m_rank & 0x01<<5) == 0)
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(FALSE);//�����ж�
	}
	else
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(TRUE);//�����ж�
	}

	
	if ((m_rank & 0x01<<6) == 0)
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(FALSE);//�����ж�
	}
	else
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(TRUE);//�����ж�
	}

	//m_rank==7
	if ((m_rank & 0x01<<7) == 0)
	{
		GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//�л���windows
	}
	else
	{
		GetDlgItem(IDC_NOSWITCH)->EnableWindow(TRUE);//�л���windows
	}

	//m_rank==8
	if ((m_rank & 0x01<<8) == 0)
	{
		GetDlgItem(IDC_USERMANAGER)->EnableWindow(FALSE);//�û� ���� 
	}
	else
	{ 
		GetDlgItem(IDC_USERMANAGER)->EnableWindow(TRUE);//�û� ����
	}

	//m_rank==9
	if ((m_rank & 0x01<<9) == 0)
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//�л���windows
		GetDlgItem(IDC_RETURN_WND)->EnableWindow(FALSE);//�ر�ϵͳ

	}
	else
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(TRUE);//�л���windows
		GetDlgItem(IDC_RETURN_WND)->EnableWindow(TRUE);//�ر�ϵͳ
	}

	return;
}

void CDiaSystem::OnReturnWnd()//�ر�ϵͳ 
{
	//д��ϵͳ�ر���־,����"����������д���¼"��ͨ����ɵ�½��¼
	//�ӱ���ini�ļ�ȡ�û�����ֵ
	char strCls[16];//�û���
	memset(strCls, ' ', sizeof(strCls));
	char buff[16];
	memset(buff,  ' ', sizeof(buff));

	GetPrivateProfileString(
		"userState",
		"userName",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(strCls, buff, sizeof(strCls));
		memset(buff, ' ', sizeof(buff));
	}

    CString fake_TagName1;
	strCls1.Format("%s",strCls);
	fake_TagName1 = fake_TagName + ".N/A." + strCls1;//�û�������Ϊ��ĵ����δ��ݽ�ȥ
	TAG_NAME tn;
	tagname_by_text(&tn,(LPCTSTR)fake_TagName1);
	TAG_VALUE tv;
	tv.iValue = 5;//iValue��ֵΪ2��ʾ��½��3��ʾע����4��ʾ������5��ʾ�ر�ϵͳ
	proxy_write_tags_y(1, &tn, &tv);

	//ע��ini�е��û��ĵ�¼״̬��userName�ǵ�¼���û�����userState�ǵ�¼״̬,1Ϊ��¼״̬
	WritePrivateProfileString("userState", "userState", NULL, get_config_file());
	WritePrivateProfileString("userState", "userState", "0", get_config_file());
	WritePrivateProfileString("userState", "userName", NULL, get_config_file());
	
	//�����ر�ϵͳʱ����ini��exception��ֵsetΪ0,ϵͳ��ʼ��ʱ��Ϊ1��ֻҪ���������ر�ϵͳ����Ϊ�쳣�ر�Ϊ1
	WritePrivateProfileString("exception", "state", NULL, get_config_file());
	WritePrivateProfileString("exception", "state", "0", get_config_file());

	EndDialog(IDC_RETURN_WND);
}

void CDiaSystem::OnChgPwd() 
{
	/*CDiaPassword dlg;
	if(dlg.DoModal()==IDOK){
		m_password[dlg.m_rank]=dlg.m_pwd;
		SavePassword();
	}*/
}

void CDiaSystem::LoadPassword()
{
	/*CString sKeyFile=((CMcviewApp*)::AfxGetApp())->m_path+"keys";
	FILE *pKeys=fopen((LPCTSTR)sKeyFile,"rb");
	char cKey[17];
	if(NULL!=pKeys){
		for(int i=0;i<10;i++){
			fread(cKey,17,1,pKeys);
			m_password[i].Format("%s",cKey);
		}
		fclose(pKeys);
	}*/
}

void CDiaSystem::SavePassword()
{
	CString sKeyFile=((CMcviewApp*)::AfxGetApp())->m_path+"keys";
	FILE *pKeys=fopen((LPCTSTR)sKeyFile,"wb");
	char cKey[27];
	if(NULL!=pKeys){
		for(int i=0;i<10;i++){
			strncpy(cKey,(LPCTSTR)m_password[i],16);
			fwrite(cKey,17,1,pKeys);
		}
		fclose(pKeys);
	}
}
//2015.08.26 GM  ��ǰ�����������
static CString GetModuleDir()//
{
	HMODULE module = GetModuleHandle(0);
	char pFileName[MAX_PATH];
	GetModuleFileName(module, pFileName, MAX_PATH);

	CString csFullPath(pFileName);
	int nPos =csFullPath.ReverseFind(_T('\\'));
	if (nPos<0)
	{
		return CString("");
	}

	return csFullPath.Left(nPos);	
}
//2015.08.26 GM  ��ǰ�����������
UINT ThreadPrint1(LPVOID pParam)//�߳�Ҫ���õĺ���
//void ThreadPrint1()
{ 
	//MessageBox(NULL,_T("i am called by a thread."), _T("thread func"),MB_OK);
	Sleep(500);
	CString str;
	CString strDateTime;
	CScreenShutter ScrShut;
	//CPrint PrtBitmap;
	//��ȡϵͳʱ��
	SYSTEMTIME st;
	GetLocalTime(&st);
	strDateTime.Format("%0.4d%0.2d%0.2d%0.2d%0.2d%0.2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	//����ͼƬ�����·�����ļ���

	CString path = GetModuleDir();
	path += "\\CopyPic\\";
	CreateDirectory(path, NULL);
	str.Format("%sCopy.bmp", strDateTime);
	str = path + str;
	ScrShut.getScreenBitmap(true);//����
	if (ScrShut.SaveBitmapToFile(str) == TRUE)//����
	{
		
		//theView.PrintView();
		theView.PrintView(str);
	}
	return 0;
}
void CDiaSystem::OnPrint() 
{
	
	CDialog::OnCancel();
	//2015.08.26 GM  �����µĴ�ӡ��ǰ���溯��
	//::AfxBeginThread(ThreadPrint1, NULL);
	//ThreadPrint1();
	theView.SetFocus();
	::AfxBeginThread(ThreadPrint1, NULL);


	//theView.PrintView(str);
}




void CDiaSystem::OnAbout() 
{
#if 0
	CDlgAbout dlg;
	dlg.DoModal();
#else
	pmcAbout(m_hWnd, "���л���");
#endif
}

void CDiaSystem::OnNoswitch() 
{
	if(!g_bEnableNoSwitch){
		return;
	}
	UpdateData(TRUE);
	CFView* pTheView=(CFView*)GetParent();

	char sysdir[_MAX_PATH];
	::GetSystemDirectory(sysdir,_MAX_PATH);
	CString sGina;
	sGina.Format("%s\\pmcgina.cf",sysdir);
	FILE *fGina=fopen((LPCTSTR)sGina,"wb");

	CWnd *pSw2 = GetDlgItem(IDC_SW2);

	gbNoSwitch=m_noSwitch;
	if(m_noSwitch){
		hKeyboardLL=SetWindowsHookEx(WH_KEYBOARD_LL,
			  (HOOKPROC)LowLevelKeyboardProc,::AfxGetApp()->m_hInstance,0);
		if(NULL!=pTheView) pTheView->m_pCtrDlg->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		if(NULL!=fGina){
			int b=1;
			fwrite(&b,sizeof(int),1,fGina);
			fclose(fGina);
		}
		pSw2->EnableWindow(FALSE);
	}else{
		if(NULL!=hKeyboardLL) UnhookWindowsHookEx(hKeyboardLL);
		hKeyboardLL=NULL;
		if(NULL!=pTheView) pTheView->m_pCtrDlg->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		if(NULL!=fGina){
			int b=0;
			fwrite(&b,sizeof(int),1,fGina);
			fclose(fGina);
		}
		pSw2->EnableWindow();
	}
}

void CDiaSystem::OnDblclkSw2()//ִ���µĳ��� 
{
	CString strExe;
	int nSel = m_sw2.GetCurSel();
	if(LB_ERR != nSel){
		m_sw2.GetText(nSel, strExe);
		strExe = theApp.m_path + "sw2\\" + strExe;
		int hRtv = (int)ShellExecute(theApp.m_pMainWnd->m_hWnd, NULL, strExe, NULL, NULL, SW_SHOWNORMAL);
		if(hRtv > 32){
			EndDialog(IDOK);
		}else{
			CString sErr;
			sErr.Format("Can't launch %s, err code:%d.", strExe, hRtv);
			MessageBox(sErr,NULL,MB_OK | MB_ICONSTOP);
		}
	}
}

void CDiaSystem::OnBnClickedUsermanager()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDiaUserManager dlg;
	dlg.DoModal();
}

void CDiaSystem::OnBnClickedLogin()//��¼
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	//CString sqlStr;
	char pswdStr[256];
	char stateStr[256];

	CString temp;
	temp = m_loginUserName;
	temp.TrimLeft(" ");
	temp.TrimRight(" ");
	 if (temp.IsEmpty())
	{
		MessageBox("�û��������벻��ȷ!", NULL, MB_OK);
		return;
	}

	//��ȡ����¼�û�����
	GetPrivateProfileString(
		m_loginUserName,
		"psword",
		"error",
		pswdStr,		
		sizeof(pswdStr),
		userinfoPath
		);
	
	//�ж������Ƿ���ȷ
	if (strcmp(pswdStr,m_loginPswd))
	{
		MessageBox("�û��������벻��ȷ!", NULL, MB_OK);
		return;
	}
	else
	{
		//�鿴�����Ƿ��Ѿ����û���¼��
		GetPrivateProfileString(
			"userState",
			"userState",
			_T(""),
			stateStr,		
			sizeof(stateStr),
			get_config_file()
			);
       
		if (stateStr == "1")
		{
			MessageBox("���������û����ڵ�¼״̬��");
			return;
		}

		////��ѯ�û�����
		char rankStr[256];
		GetPrivateProfileString(
			m_loginUserName,
			"userRank",
			"error",
			rankStr,		
			sizeof(rankStr),
			userinfoPath
			);

		if (!strcmp(rankStr,"")) 
		{
			MessageBox("���û�û���κ�Ȩ�ޣ���������Ȩ��",NULL,MB_OK);
			return;
		}

		CArgs args;
		parse_arg_ey(rankStr,&args,"*");

		for (int i=0;i<args.argc;i++)
		{
			int j = atoi(args.argv[i]);
			m_rank = m_rank | (0x01<<j);
		}

		//////////////
		SetDlgItemText(IDC_USER, _T(""));
		SetDlgItemText(IDC_PWD, _T(""));
		SetDlgItemText(IDC_CURRENTUSER, m_loginUserName);

		//��ini��userState��״̬��Ϊ1����ʾ�ѵ�¼
		WritePrivateProfileString("userState", "userState", NULL, get_config_file());
        WritePrivateProfileString("userState", "userState", "1", get_config_file());
	
		m_sCurUser = _T("");//���һ��
		m_sCurUser = m_loginUserName;
		
		//�ѵ�¼���û���д�뵽ini�ļ���
        WritePrivateProfileString("userState", "userName", m_sCurUser, get_config_file());

		CWnd *pwndBtn;
		pwndBtn = GetDlgItem(IDC_LOGIN);
		pwndBtn->EnableWindow(FALSE);
		pwndBtn = GetDlgItem(IDC_LOGOUT);
		pwndBtn->EnableWindow(TRUE);

		CWnd *wnd  = AfxGetMainWnd();//��ȡmcviewdlg��ָ��

		wnd->GetDlgItem(IDC_INDEX)->EnableWindow(TRUE);//Ŀ¼
		wnd->GetDlgItem(IDC_PREVIOUS)->EnableWindow(TRUE);//���ͷ
		wnd->GetDlgItem(IDC_NEXT)->EnableWindow(TRUE);//�Ҽ�ͷ
		wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(TRUE);//��������
		wnd->GetDlgItem(IDC_CLOSEALARM)->EnableWindow(TRUE);//�����ر�
		wnd->GetDlgItem(IDC_LOOP)->EnableWindow(TRUE);//ѭ��

		SetButton();

		//д���¼��־,����"����������д���¼"��ͨ����ɵ�½��¼
		//�ӱ���ini�ļ�ȡ�û�����ֵ
		char strCls[16];//�û���
		memset(strCls, ' ', sizeof(strCls));
		char buff[16];
		memset(buff,  ' ', sizeof(buff));

		GetPrivateProfileString(
			"userState",
			"userName",
			"N/A",
			buff,
			_countof(buff),
			get_config_file());
		if (buff)
		{
			_tcsncpy_s(strCls, buff, sizeof(strCls));
			memset(buff, ' ', sizeof(buff));
		}
 
		CString fake_TagName1;
		strCls1.Format("%s",strCls);
		fake_TagName1 = fake_TagName + ".N/A." + strCls1;//�û�������Ϊ��ĵ����δ��ݽ�ȥ
		TAG_NAME tn;
		tagname_by_text(&tn,(LPCTSTR)fake_TagName1);

		TAG_VALUE tv;
		tv.iValue = 2;//iValue��ֵΪ2��ʾ��½��3��ʾע��
		proxy_write_tags_y(1, &tn, &tv);
	}
}

void CDiaSystem::OnBnClickedLogout()//ע���û�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//д��ע����־,����"����������д���¼"��ͨ�����ע����¼
	//�ӱ���ini�ļ�ȡ�û�����ֵ
	char strCls[16];//�û���
	memset(strCls, ' ', sizeof(strCls));
	char buff[16];
	memset(buff,  ' ', sizeof(buff));

	GetPrivateProfileString(
		"userState",
		"userName",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(strCls, buff, sizeof(strCls));
		memset(buff, ' ', sizeof(buff));
	}

	CString fake_TagName1;
	strCls1.Format("%s",strCls);
	fake_TagName1 = fake_TagName + ".N/A." + strCls1;//�û�������Ϊ��ĵ����δ��ݽ�ȥ
	TAG_NAME tn;
	tagname_by_text(&tn,(LPCTSTR)fake_TagName1);

	TAG_VALUE tv;
	tv.iValue = 3;//iValue��ֵΪ2��ʾ��½��3��ʾע��

	proxy_write_tags_y(1, &tn, &tv);

	//��ini��userState��״̬��Ϊ0����ʾ�û��Ѿ�ע��
	WritePrivateProfileString("userState", "userState", NULL, get_config_file());
	WritePrivateProfileString("userState", "userState", "0", get_config_file());
	WritePrivateProfileString("userState", "userName", NULL, get_config_file());
	WritePrivateProfileString("userState", "userName", "", get_config_file());

	m_rank = 0;
    m_sCurUser = "";
	SetDlgItemText(IDC_CURRENTUSER, _T(""));

	CWnd *wnd  = AfxGetMainWnd();//��ȡmcviewdlg��ָ��

	wnd->GetDlgItem(IDC_INDEX)->EnableWindow(FALSE);//Ŀ¼
	wnd->GetDlgItem(IDC_PREVIOUS)->EnableWindow(FALSE);//���ͷ
	wnd->GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);//�Ҽ�ͷ
	wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(FALSE);//��������
	wnd->GetDlgItem(IDC_CLOSEALARM)->EnableWindow(FALSE);//�����ر�
	wnd->GetDlgItem(IDC_LOOP)->EnableWindow(FALSE);//ѭ��

	GetDlgItem(IDC_RETURN_WND)->EnableWindow(FALSE);//�ر�ϵͳ
	GetDlgItem(IDC_USERMANAGER)->EnableWindow(FALSE);//�û�����
	GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);//��¼��ť
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);//ע����ť
	GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//�л���windows

	//SetButton();
}


void CDiaSystem::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CDiaSystem::OnStnClickedCurrentuser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDiaSystem::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// Zander 2015-6-5
	ShellExecute(GetSafeHwnd(), NULL, _T("osk.exe"), NULL, NULL, SW_NORMAL);
	WinExec("osk.exe", SW_NORMAL);
}
