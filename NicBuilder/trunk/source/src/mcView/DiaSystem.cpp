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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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
			SetDlgItemText(IDC_ERR,"口令错误");
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

void CDiaSystem::SetButton()//权限设置，屏蔽按钮
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
    
	//默认m_rank==0
	CWnd *wnd  = AfxGetMainWnd();//获取mcviewdlg的指针
	if ((m_rank & 0x01) == 0)
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//切换到windows
	}
	else
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(TRUE);//切换到windows
	}

	//m_rank==1
	if ((m_rank & 0x01<<1) == 0)
	{
		//wnd->GetDlgItem(IDC_HJHG)->EnableWindow(FALSE);//环境后果		
	}
	else
	{
		//wnd->GetDlgItem(IDC_HJHG)->EnableWindow(TRUE);//环境后果	
	}

    //m_rank==2
	if ((m_rank & 0x01<<2) == 0)
	{
		//wnd->GetDlgItem(IDC_COMMUNICATION_MANAGE)->EnableWindow(FALSE);//信息发布
	}
	else
	{
		//wnd->GetDlgItem(IDC_COMMUNICATION_MANAGE)->EnableWindow(TRUE);//信息发布
	}

	//m_rank==3
	if ((m_rank & 0x01<<3) == 0)
	{
		//wnd->GetDlgItem(IDC_REPORT_CHECK)->EnableWindow(FALSE);//报告审批
	}
	else
	{
		//wnd->GetDlgItem(IDC_REPORT_CHECK)->EnableWindow(TRUE);//报告审批
	}

	//m_rank==4
	if ((m_rank & 0x01<<4) == 0)
	{
		//wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(FALSE);//报警确认？报警记录
	}
	else
	{
		//wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(TRUE);//报警确认？报警记录
	}

	//m_rank==5
	if ((m_rank & 0x01<<5) == 0)
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(FALSE);//辅助判断
	}
	else
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(TRUE);//辅助判断
	}

	
	if ((m_rank & 0x01<<6) == 0)
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(FALSE);//辅助判断
	}
	else
	{
		//wnd->GetDlgItem(IDC_ASSISTANTBTN)->EnableWindow(TRUE);//辅助判断
	}

	//m_rank==7
	if ((m_rank & 0x01<<7) == 0)
	{
		GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//切换到windows
	}
	else
	{
		GetDlgItem(IDC_NOSWITCH)->EnableWindow(TRUE);//切换到windows
	}

	//m_rank==8
	if ((m_rank & 0x01<<8) == 0)
	{
		GetDlgItem(IDC_USERMANAGER)->EnableWindow(FALSE);//用户 管理 
	}
	else
	{ 
		GetDlgItem(IDC_USERMANAGER)->EnableWindow(TRUE);//用户 管理
	}

	//m_rank==9
	if ((m_rank & 0x01<<9) == 0)
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//切换到windows
		GetDlgItem(IDC_RETURN_WND)->EnableWindow(FALSE);//关闭系统

	}
	else
	{
		//GetDlgItem(IDC_NOSWITCH)->EnableWindow(TRUE);//切换到windows
		GetDlgItem(IDC_RETURN_WND)->EnableWindow(TRUE);//关闭系统
	}

	return;
}

void CDiaSystem::OnReturnWnd()//关闭系统 
{
	//写入系统关闭日志,借用"开关量报警写入记录"的通道完成登陆记录
	//从本地ini文件取用户名的值
	char strCls[16];//用户名
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
	fake_TagName1 = fake_TagName + ".N/A." + strCls1;//用户名是作为点的第三段传递进去
	TAG_NAME tn;
	tagname_by_text(&tn,(LPCTSTR)fake_TagName1);
	TAG_VALUE tv;
	tv.iValue = 5;//iValue的值为2表示登陆，3表示注销，4表示启动，5表示关闭系统
	proxy_write_tags_y(1, &tn, &tv);

	//注销ini中的用户的登录状态，userName是登录的用户名，userState是登录状态,1为登录状态
	WritePrivateProfileString("userState", "userState", NULL, get_config_file());
	WritePrivateProfileString("userState", "userState", "0", get_config_file());
	WritePrivateProfileString("userState", "userName", NULL, get_config_file());
	
	//正常关闭系统时，将ini中exception的值set为0,系统初始化时置为1，只要不是正常关闭系统都视为异常关闭为1
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
//2015.08.26 GM  当前画面截屏函数
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
//2015.08.26 GM  当前画面截屏函数
UINT ThreadPrint1(LPVOID pParam)//线程要调用的函数
//void ThreadPrint1()
{ 
	//MessageBox(NULL,_T("i am called by a thread."), _T("thread func"),MB_OK);
	Sleep(500);
	CString str;
	CString strDateTime;
	CScreenShutter ScrShut;
	//CPrint PrtBitmap;
	//获取系统时间
	SYSTEMTIME st;
	GetLocalTime(&st);
	strDateTime.Format("%0.4d%0.2d%0.2d%0.2d%0.2d%0.2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	//截屏图片保存的路径和文件名

	CString path = GetModuleDir();
	path += "\\CopyPic\\";
	CreateDirectory(path, NULL);
	str.Format("%sCopy.bmp", strDateTime);
	str = path + str;
	ScrShut.getScreenBitmap(true);//截屏
	if (ScrShut.SaveBitmapToFile(str) == TRUE)//保存
	{
		
		//theView.PrintView();
		theView.PrintView(str);
	}
	return 0;
}
void CDiaSystem::OnPrint() 
{
	
	CDialog::OnCancel();
	//2015.08.26 GM  增加新的打印当前画面函数
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
	pmcAbout(m_hWnd, "运行画面");
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

void CDiaSystem::OnDblclkSw2()//执行新的程序 
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
	// TODO: 在此添加控件通知处理程序代码
	CDiaUserManager dlg;
	dlg.DoModal();
}

void CDiaSystem::OnBnClickedLogin()//登录
{
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox("用户名或密码不正确!", NULL, MB_OK);
		return;
	}

	//读取待登录用户密码
	GetPrivateProfileString(
		m_loginUserName,
		"psword",
		"error",
		pswdStr,		
		sizeof(pswdStr),
		userinfoPath
		);
	
	//判断密码是否正确
	if (strcmp(pswdStr,m_loginPswd))
	{
		MessageBox("用户名或密码不正确!", NULL, MB_OK);
		return;
	}
	else
	{
		//查看本机是否已经有用户登录，
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
			MessageBox("本机已有用户处于登录状态！");
			return;
		}

		////查询用户级别
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
			MessageBox("此用户没有任何权限，请先设置权限",NULL,MB_OK);
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

		//把ini中userState的状态改为1，表示已登录
		WritePrivateProfileString("userState", "userState", NULL, get_config_file());
        WritePrivateProfileString("userState", "userState", "1", get_config_file());
	
		m_sCurUser = _T("");//清空一下
		m_sCurUser = m_loginUserName;
		
		//把登录的用户名写入到ini文件中
        WritePrivateProfileString("userState", "userName", m_sCurUser, get_config_file());

		CWnd *pwndBtn;
		pwndBtn = GetDlgItem(IDC_LOGIN);
		pwndBtn->EnableWindow(FALSE);
		pwndBtn = GetDlgItem(IDC_LOGOUT);
		pwndBtn->EnableWindow(TRUE);

		CWnd *wnd  = AfxGetMainWnd();//获取mcviewdlg的指针

		wnd->GetDlgItem(IDC_INDEX)->EnableWindow(TRUE);//目录
		wnd->GetDlgItem(IDC_PREVIOUS)->EnableWindow(TRUE);//左箭头
		wnd->GetDlgItem(IDC_NEXT)->EnableWindow(TRUE);//右箭头
		wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(TRUE);//报警监视
		wnd->GetDlgItem(IDC_CLOSEALARM)->EnableWindow(TRUE);//报警关闭
		wnd->GetDlgItem(IDC_LOOP)->EnableWindow(TRUE);//循环

		SetButton();

		//写入登录日志,借用"开关量报警写入记录"的通道完成登陆记录
		//从本地ini文件取用户名的值
		char strCls[16];//用户名
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
		fake_TagName1 = fake_TagName + ".N/A." + strCls1;//用户名是作为点的第三段传递进去
		TAG_NAME tn;
		tagname_by_text(&tn,(LPCTSTR)fake_TagName1);

		TAG_VALUE tv;
		tv.iValue = 2;//iValue的值为2表示登陆，3表示注销
		proxy_write_tags_y(1, &tn, &tv);
	}
}

void CDiaSystem::OnBnClickedLogout()//注销用户
{
	// TODO: 在此添加控件通知处理程序代码

	//写入注销日志,借用"开关量报警写入记录"的通道完成注销记录
	//从本地ini文件取用户名的值
	char strCls[16];//用户名
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
	fake_TagName1 = fake_TagName + ".N/A." + strCls1;//用户名是作为点的第三段传递进去
	TAG_NAME tn;
	tagname_by_text(&tn,(LPCTSTR)fake_TagName1);

	TAG_VALUE tv;
	tv.iValue = 3;//iValue的值为2表示登陆，3表示注销

	proxy_write_tags_y(1, &tn, &tv);

	//把ini中userState的状态改为0，表示用户已经注销
	WritePrivateProfileString("userState", "userState", NULL, get_config_file());
	WritePrivateProfileString("userState", "userState", "0", get_config_file());
	WritePrivateProfileString("userState", "userName", NULL, get_config_file());
	WritePrivateProfileString("userState", "userName", "", get_config_file());

	m_rank = 0;
    m_sCurUser = "";
	SetDlgItemText(IDC_CURRENTUSER, _T(""));

	CWnd *wnd  = AfxGetMainWnd();//获取mcviewdlg的指针

	wnd->GetDlgItem(IDC_INDEX)->EnableWindow(FALSE);//目录
	wnd->GetDlgItem(IDC_PREVIOUS)->EnableWindow(FALSE);//左箭头
	wnd->GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);//右箭头
	wnd->GetDlgItem(IDC_ALARMLIST)->EnableWindow(FALSE);//报警监视
	wnd->GetDlgItem(IDC_CLOSEALARM)->EnableWindow(FALSE);//报警关闭
	wnd->GetDlgItem(IDC_LOOP)->EnableWindow(FALSE);//循环

	GetDlgItem(IDC_RETURN_WND)->EnableWindow(FALSE);//关闭系统
	GetDlgItem(IDC_USERMANAGER)->EnableWindow(FALSE);//用户管理
	GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);//登录按钮
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);//注销按钮
	GetDlgItem(IDC_NOSWITCH)->EnableWindow(FALSE);//切换到windows

	//SetButton();
}


void CDiaSystem::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CDiaSystem::OnStnClickedCurrentuser()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDiaSystem::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// Zander 2015-6-5
	ShellExecute(GetSafeHwnd(), NULL, _T("osk.exe"), NULL, NULL, SW_NORMAL);
	WinExec("osk.exe", SW_NORMAL);
}
