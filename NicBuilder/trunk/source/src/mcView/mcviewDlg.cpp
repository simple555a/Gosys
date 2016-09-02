//= UPDATE RECORD
//= Update for    Version     Update by     Date         Update                           
//= (1-1)         1.2.0.5        mox      2013-9-29      1.AlarmCallBack()中新增：if(ac == AC_AckAlarm)分支
//                                                       2.新增类CAckAlarmTransaction和函数SendFakeAlarm()
//  (1-3)         1.2.0.5        mox      2013-9-29      1.新增判断条件if(g_TopAlarmPt>=0 && g_AlarmRecords[g_TopAlarmPt].m_ac < 3 && g_AlarmRecords[g_TopAlarmPt].m_ack == false)
//  (1-4)         1.2.0.5        mox      2013-9-29      1.新增SetDlgItemText(IDC_ALARM,"");
//  (1-4)         1.2.0.5        mox      2013-9-30      1.AlarmCallBack():
//  (1-1)         1.2.0.6        mox      2013-12-6      1.新增message用来刷新报警列表窗口。::PostMessage(hWnd,WM_REDRAWWINDOW_MESSAGE,0,0);
//  (1-2)         1.2.0.6        mox      2013-12-6      1.新增FindTopAlarmNoOperate()
// mcviewDlg.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "mcview.h"
#include "mcviewDlg.h"
#include "fview.h"
#include "diainputd.h"
#include "diainputa.h"
#include "diaalarmlist.h"
#include "diasystem.h"
#include "dlgloop.h"
#include <hisproxy.h>
#include <transaction.h>
#include "rtk_packet.h"
#include <string.h>
#include "DiaAlarmList.h"
#include "DiaAlarmDlg.h"
#include "libhis.h"
#include "event.h"
#include <vector>
#include <MMSystem.h>
#include<direct.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define WM_REFRESH_MESSAGE WM_USER+103 //自定义消息，用来通知refresh任务栏布局

#define ALARMPOOL	1024
#define ALARMLEN	1024
typedef CPMCArchive<CPMCEvent, ALARMLEN> CPMCAlarms;
static CPMCAlarms * alarms_x;

CFView theView;
long glViewWidth,glViewHeight;
long glTimer;
HICON ghNA;
long glRank;
CDiaInputD gDiaInputD;
CDiaInputA gDiaInputA;
CDiaAlarmDlg alarmDlg;
int rank3;
int rank4;
//2016.02.17 GM  添加报警级别
int rank_R1;
int rank_R2;
int rank_Y1;
int rank_Y2;
int rank_W1;
int rank_W2;
int rank_G1;
int rank_G2;
CString fake_TagName;
extern CString m_sCurUser;
CString logName_x;
int flag_refresh;

/////////////////////////////////////////////////////////////////////////////
// CMcviewDlg dialog

void CDECL AlarmCallBack(int rank, ALARM_CLASS ac, const char *msg, PCRTK_ADDR src)
{

	if(!g_bShowAllAlarms && rank < 500)
	{
		return;             
	}
	//moxb+++20130929
	if(ac == AC_AckAlarm)//receive the fake alarm  
	{   
		CString str3, str4, str6;
		str3.Format("%s",msg); 
		str4 = str3.Right(8);
		str6 = str3.Right(5);
		if (!strcmp(str6, "_EVTD"))//最后五个字符必须是_EVTD
		{
			CAlarmRecord * pAlarm;
			pAlarm = g_AlarmRecords;
			if (!strcmp(str4, "ALL_EVTD"))//ack all the alarms
			{
				//取出操作用户名
				CArgs args, argsx;
				parse_arg_ey(str3, &args,";");//操作员

				CPMCEvent evts;
				CPMCEvent *evt = &evts;
				evt->valid = true;
				evt->eClass = ac;
				strncpy(evt->data, msg, sizeof(evt->data));
				evt->data[sizeof(evt->data) - 1] = 0;
				evt->src = src->host;
				evt->rTime = getCurrentTime();
				evt->eTime = evt->rTime;
				//
				CString tempx, tempx0, tempx1, tempx2, tempx3, tempx4, tempx5, tempx6;
				for(int i = 0; i < RCT_ALARM_COUNT; i++, pAlarm++)
				{
					if (!(pAlarm->m_ack) && (pAlarm->m_rank != 0))
					{
						pAlarm->m_ack = true;
						//把“已确认”三个字加入到报警的记录中去
						PMCTIME tm = getCurrentTime();//取当前的时间，加入是操作员确认报警的时间。
						char str[30] = {0};
						time2str_x(tm, str);
						parse_arg_ey(pAlarm->m_msg, &argsx,";");
						tempx = str;
						tempx0 = argsx.argv[0];
						tempx1 = argsx.argv[1];
						tempx2 = argsx.argv[2];
						tempx3 = argsx.argv[3];
						tempx4 = argsx.argv[4];
						tempx5 = argsx.argv[5];
						tempx6 = tempx + "(" + tempx0 + ")" + ";" + tempx1 + ";" + tempx2 + ";" + tempx3 + _T("/确认") + ";" + tempx4 + ";" + tempx5 + ";";
						strncpy(pAlarm->m_msg, tempx6, sizeof(pAlarm->m_msg));
						strcat(pAlarm->m_msg, args.argv[0]);
						strcat(pAlarm->m_msg, ";");
						strncpy(evt->data, pAlarm->m_msg, sizeof(evt->data));
						alarms_x->addNewRecord(&evts);//确认全部报警记录，存入本地
						alarms_x->flush2file();
					}
				}
				::SetDlgItemText(AfxGetApp()->m_pMainWnd->m_hWnd,IDC_ALARM,"");	
			}
			else//ack just one alarm
			{
				long m = RCT_ALARM_COUNT;
				for(int i = 0; i < m; i++)
				{
					char* m_msg = g_AlarmRecords[i].m_msg;//数组中原有的报警记录
					CString str,str1,str2,temp,temp0,temp1,temp2,temp3,temp4,temp5,str5,str6;
					str.Format("%s",msg);//evtd传来的报警记录
					int s = str.GetLength();
					str1 = str.Left(s - 5);//去除尾缀
					//取出操作用户名
					CArgs args;
					parse_arg_ey(str1,&args,";");
					temp0 = args.argv[0];
					temp1 = args.argv[1];
					temp2 = args.argv[2];
					temp3 = args.argv[3];
					temp4 = args.argv[4];
					temp5 = args.argv[5];
					logName_x = args.argv[6];//操作员
					str2 = temp0 + ";" + temp1 + ";" + temp2 + ";" + temp3 + ";" + temp4 + ";" + temp5 + ";";
					//str5 = str2 + logName_x;
					if (!strcmp(str2, m_msg))//去掉字符串尾的操作用户，先对报警进行比较,
					{
						//pAlarm->m_ack = true;
						g_AlarmRecords[i].m_ack = true;//true为已经确认
						//刷新任务栏报警
						// mox++2013-9-30
						char msg_r[256] = {0}; 
						::GetDlgItemText(AfxGetApp()->m_pMainWnd->m_hWnd,IDC_ALARM,msg_r,256);//the alarm info of IDC_ALARM right now.  
						if(!strcmp(str2,msg_r))
							::SetDlgItemText(AfxGetApp()->m_pMainWnd->m_hWnd,IDC_ALARM,"");
						//写入本地记录
						CPMCEvent evts;
						CPMCEvent *evt = &evts;
						evt->valid = true;
						evt->eClass = ac;
						strncpy(evt->data, msg, sizeof(evt->data));
						evt->data[sizeof(evt->data) - 1] = 0;
						evt->src = src->host;
						evt->rTime = getCurrentTime();
						evt->eTime = evt->rTime;
						PMCTIME tm = getCurrentTime();//取当前的时间，覆盖掉报警发生的时间，因为这里需要的是操作员确认报警的时间。
						char strx[30] = {0};
						time2str_x(tm, strx);
						temp = strx;
						str6 = temp + "(" + temp0 + ")" + ";" + temp1 + ";" + temp2 + ";" + temp3 + _T("/确认") + ";" + temp4 + ";" + temp5 + ";" + logName_x + ";";
						strncpy(g_AlarmRecords[i].m_msg, str6, sizeof(pAlarm->m_msg));
						strncpy(evt->data, str6, sizeof(evt->data));
						alarms_x->addNewRecord(&evts);//确认全部报警记录，存入本地
						alarms_x->flush2file();
						break;
					}
				}
			}
		}
	}
	//moxe+++20130929
	else//正常处理报警,
	{
		if(rank >= rank3 && rank4 >= rank)//判断是否为本机的报警级别，
		{
			CAlarmRecord alarm;
			alarm.m_ac=ac;
			alarm.m_rank=rank;
			strncpy(alarm.m_msg,msg,sizeof(alarm.m_msg));
			alarm.m_msg[sizeof(alarm.m_msg) - 1] = 0;
			alarm.m_valid=true;
			g_AlarmQueueTail++;
			if(g_AlarmQueueTail == RCT_ALARM_COUNT)
			{
				g_AlarmQueueTail = 0;
			}
			g_AlarmRecords[g_AlarmQueueTail] = alarm;
		}
	}

	CMcviewDlg *pCtlDlg=(CMcviewDlg*)(((CMcviewApp*)::AfxGetApp())->GetMainWnd());
	if(!pCtlDlg) return;

	//任务栏取最新报警
	CString sAlarm;
	g_TopAlarmPt = FindTopAlarmNoOperate();//mox+++20131206
	if(g_TopAlarmPt >= 0 && g_AlarmRecords[g_TopAlarmPt].m_ac < 2 && g_AlarmRecords[g_TopAlarmPt].m_ack==false)//mox+++20130929
	{
		sAlarm.Format("%s", g_AlarmRecords[g_TopAlarmPt].m_msg);
		pCtlDlg->SetDlgItemText(IDC_ALARM,sAlarm);
	}
	//
	//刷新列表窗口//mox+++20131206
	HWND hWnd = ::FindWindow(NULL, _T("当前事件列表"));//取到窗口的句柄
	if (hWnd != NULL)
	{
		::PostMessage(hWnd,WM_REDRAWWINDOW_MESSAGE,0,0);
	}
}

static void _cdecl _OnProgress(__uint t, __uint c, const char * msg)
{
}

CMcviewDlg::CMcviewDlg(UINT nID, CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent)
{
	
	m_alarmonoff = FALSE;
	//{{AFX_DATA_INIT(CMcviewDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_brushWhite.CreateSolidBrush(RGB(255,255,255));
	glRank=0;
	// alarm_callback=AlarmCallBack;
	g_AlarmQueueTail=-1;
	g_TopAlarmPt=-1;
	m_curView=-1;
	for(int i=0;i<10;i++){
		m_viewList[i].Empty();
	}

	m_loopCount = 0;
	m_loopCurrent = 0;
	m_loopInterval = 3;
	m_bShowLoop = false;

	m_notListUpdate = false;

}

CMcviewDlg::~CMcviewDlg()
{
	POSITION pos;
	for(pos=m_PopViews.GetHeadPosition();pos!=NULL;){
		delete m_PopViews.GetNext(pos);
	}	 
	m_PopViews.RemoveAll();
	uninitHisProxy();
	uninit_proxy();
}

void CMcviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Check(pDX, IDC_ALARM_ON_OFF, m_alarmonoff);
	//{{AFX_DATA_MAP(CMcviewDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ALARMLIST, m_alarmList);
	DDX_Control(pDX, IDC_CLOSEALARM, m_alarmClose);
}

BEGIN_MESSAGE_MAP(CMcviewDlg, CDialog)
	//{{AFX_MSG_MAP(CMcviewDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SYSTEM, OnSystem)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ACKALARM, OnAckalarm)
	ON_BN_CLICKED(IDC_ALARMLIST, OnAlarmlist)
	ON_BN_CLICKED(IDC_INDEX, OnIndex)
	ON_BN_CLICKED(IDC_NOW, OnNow)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_ALARM, OnAlarm)
	ON_BN_CLICKED(IDC_KEY, OnKey)
	ON_BN_CLICKED(IDC_PREVIOUS, OnPrevious)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_LOOP, OnLoop)
	ON_WM_DESTROY()
	ON_WM_COPYDATA()
	ON_MESSAGE(WM_REFRESH_MESSAGE, OnRefreshMessage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CLOSEALARM, &CMcviewDlg::OnBnClickedClosealarm)
	ON_WM_SIZING()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMcviewDlg message handlers

BOOL CMcviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	init_proxy(
		m_hWnd, 
		PROXY_ENABLE_BGLOADER | PROXY_ENABLE_DISPATCHER
		);
	initHisProxy();

	// Set the icon for this dialog.  The framework does this automatically
	//	when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// name self, so that the successively coming instances
	// will find this window
	SetWindowText(MCVIEW_MAIN_DLG_NAME);

	// 设置控制板为全窗口宽，高度为全窗口宽的1/32,任务栏
	RECT rectDesktop;
	::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
	int iW=rectDesktop.right;
	flag_refresh = rectDesktop.bottom;
	SetWindowPos(NULL,0,rectDesktop.bottom-iW/32,iW,iW/32,0);
	
	glTimer=0;
	ghNA=((CMcviewApp*)::AfxGetApp())->LoadIcon(IDI_NA);
	//显示窗口
	glViewWidth=iW;
	glViewHeight=rectDesktop.bottom-iW/32;
	theView.m_pCtrDlg=this;
	theView.Create(IDD_FVIEW_DIALOG);
	theView.MoveWindow(0,0,glViewWidth,glViewHeight);
	theView.ShowWindow(SW_SHOWNORMAL);
 
	//设置控件位置
	GetClientRect(&rectDesktop);
	//ClientToScreen(&rectDesktop);
	iW=rectDesktop.right-rectDesktop.left;
	int iH=rectDesktop.bottom-rectDesktop.top;
	CWnd* pContrl;
	//system button
	pContrl=GetDlgItem(IDC_SYSTEM);
	pContrl->MoveWindow(0,0,iW/24,iH);
	//key
 	pContrl=GetDlgItem(IDC_KEY);
 	pContrl->MoveWindow(iW-5*iW/24-iW/2-iW/6-2*iW/36,0,iW/24,iH);
	//index
	pContrl=GetDlgItem(IDC_INDEX);
	pContrl->MoveWindow(iW-4*iW/24-iW/2-iW/6-2*iW/36,0,iW/24,iH);
	//previous
	pContrl=GetDlgItem(IDC_PREVIOUS);
	pContrl->MoveWindow(iW-3*iW/24-iW/2-iW/6-2*iW/36,0,iW/36,iH);
	//next 
	pContrl=GetDlgItem(IDC_NEXT);
	pContrl->MoveWindow(iW-3*iW/24-iW/2-iW/6-iW/36,0,iW/36,iH);
	//loop
	pContrl=GetDlgItem(IDC_LOOP);
	pContrl->MoveWindow(iW-3*iW/24-iW/2-iW/6,0,iW/24,iH);
	//time lab
	pContrl=GetDlgItem(IDC_TIME);
	pContrl->MoveWindow(iW-2*iW/24-iW/2-iW/6,0,iW/6,iH);
	
	//alarm
	pContrl=GetDlgItem(IDC_ALARM);
	pContrl->MoveWindow(iW-2*iW/24-iW/2,0,iW/2,iH);//Zander

	//ack alarm
	pContrl=GetDlgItem(IDC_ACKALARM);
	pContrl->MoveWindow(iW-2*iW/24,0,iW/24,iH);
	
	//报警监视
	pContrl=GetDlgItem(IDC_ALARMLIST);
	pContrl->MoveWindow(iW-iW/24,0,iW/24,iH);

	//监视关闭
	pContrl=GetDlgItem(IDC_CLOSEALARM);
	pContrl->MoveWindow(iW-iW/24,0,iW/24,iH);

	//显示报警窗口
	alarmDlg.Create(IDD_ALARM_DIALOG);
	alarmDlg.MoveWindow(0,0,glViewWidth,glViewHeight);
	alarmDlg.ShowWindow(SW_HIDE);
	
	//设置定时器

	m_nTimerRefresh=SetTimer(1,250,0);

	m_curView=0;

	m_viewList[m_curView] = g_startFile;
	theView.LoadFile(g_startFile);

	if(g_bEnableNoSwitch){
		SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

	g_ProxyDispatchTable.OnAlarm = AlarmCallBack;
	g_ProxyDispatchTable.OnProgress = _OnProgress;

	//将用户名全局变量置为空
	m_sCurUser = "";

	//初始化本地报警确认记录保存
	string str, s1, s2;
	str = string(get_working_dir()) + string("var");
	if(ENOENT == _mkdir(str.c_str())){
		CString msg;
		msg.Format("path %s not found!", str.c_str());
		MessageBox(msg);
		PostQuitMessage(0);
	}
	str = str + "/evtd";
	_mkdir(str.c_str());
	s1 = str + "/alarm";
	_mkdir(s1.c_str());
	s2 = str + "/ackedalarm";
	_mkdir(s2.c_str());

	alarms_x = new CPMCAlarms;
	if(alarms_x){
		alarms_x->initialize(s2.c_str(), ALARMPOOL);
	}

	//写入系统启动日志,借用"开关量报警写入记录"的通道完成登陆记录
	char fakeTagName[16];
	memset(fakeTagName, ' ', sizeof(fakeTagName));
	char buff[16];
	memset(buff,  ' ', sizeof(buff));

	GetPrivateProfileString(
		"pmc",
		"serverName",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(fakeTagName, buff, sizeof(fakeTagName));
		memset(buff, ' ', sizeof(buff));
	}

	CString fake_TagName1;
	fake_TagName.Format("%s",fakeTagName);
	fake_TagName1 = fake_TagName + ".N/A.N/A";//启动的用户名是N/A
	TAG_NAME tn;
	tagname_by_text(&tn,(LPCTSTR)fake_TagName1);
	TAG_VALUE tv;
	tv.iValue = 4;//iValue的值为2表示登陆，3表示注销，4表示启动，5表示关闭系统
	proxy_write_tags_y(1, &tn, &tv);

	//初始化报警参数，
	char rankShow[16];
	memset(rankShow, ' ', sizeof(rankShow));

	GetPrivateProfileString(
		"alarm",
		"rankShow",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(rankShow, buff, sizeof(rankShow));
		memset(buff, ' ', sizeof(buff));
	}

	CArgs argx2;
	parse_arg_ey(rankShow, &argx2,",");
	rank3 = atoi(argx2.argv[0]);
	rank4 = atoi(argx2.argv[1]);

	//2016.02.17 GM  添加报警级别参数读取
	//读取红色报警级别参数
	char rankShow_R[16];
	memset(rankShow_R, ' ', sizeof(rankShow_R));

	GetPrivateProfileString(
		"alarm",
		"rAlarmRankShow",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(rankShow_R, buff, sizeof(rankShow_R));
		memset(buff, ' ', sizeof(buff));
	}

	CArgs argxR;
	parse_arg_ey(rankShow_R, &argxR,",");
	rank_R1 = atoi(argxR.argv[0]);
	rank_R2 = atoi(argxR.argv[1]);
	//读取黄色报警级别参数
	char rankShow_Y[16];
	memset(rankShow_Y, ' ', sizeof(rankShow_Y));

	GetPrivateProfileString(
		"alarm",
		"yAlarmRankShow",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(rankShow_Y, buff, sizeof(rankShow_Y));
		memset(buff, ' ', sizeof(buff));
	}

	CArgs argxY;
	parse_arg_ey(rankShow_Y, &argxY,",");
	rank_Y1 = atoi(argxY.argv[0]);
	rank_Y2 = atoi(argxY.argv[1]);
	//读取白色报警级别参数
	char rankShow_W[16];
	memset(rankShow_W, ' ', sizeof(rankShow_W));

	GetPrivateProfileString(
		"alarm",
		"wAlarmRankShow",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(rankShow_W, buff, sizeof(rankShow_W));
		memset(buff, ' ', sizeof(buff));
	}

	CArgs argxW;
	parse_arg_ey(rankShow_W, &argxW,",");
	rank_W1 = atoi(argxW.argv[0]);
	rank_W2 = atoi(argxW.argv[1]);
	//读取白色报警级别参数
	char rankShow_G[16];
	memset(rankShow_G, ' ', sizeof(rankShow_G));

	GetPrivateProfileString(
		"alarm",
		"gAlarmRankShow",
		"N/A",
		buff,
		_countof(buff),
		get_config_file());
	if (buff)
	{
		_tcsncpy_s(rankShow_G, buff, sizeof(rankShow_G));
		memset(buff, ' ', sizeof(buff));
	}

	CArgs argxG;
	parse_arg_ey(rankShow_G, &argxG,",");
	rank_G1 = atoi(argxG.argv[0]);
	rank_G2 = atoi(argxG.argv[1]);

	alarmDlg.m_rank_R1=rank_R1;
	alarmDlg.m_rank_R2=rank_R2;
	alarmDlg.m_rank_Y1=rank_Y1;
	alarmDlg.m_rank_Y2=rank_Y2;
	alarmDlg.m_rank_W1=rank_W1;
	alarmDlg.m_rank_W2=rank_W2;
	alarmDlg.m_rank_G1=rank_G1;
	alarmDlg.m_rank_G2=rank_G2;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//	to draw the icon.  For MFC applications using the document/view model,
//	this is automatically done for you by the framework.

void CMcviewDlg::OnPaint() 
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
/*****************************************/
#if 1  //2011-3-22 guooujie 消除全屏时任务栏覆盖的bug
		RECT rectDesktop;
		::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
		int iW=rectDesktop.right;
		SetWindowPos(NULL,0,rectDesktop.bottom-iW/32,iW,iW/32,0);
		UpdateWindow();
#endif

		CDialog::OnPaint();
	}

	static bool bFirst=true;
	if(bFirst) theView.SetFocus();
	bFirst=false;
}

// The system calls this to obtain the cursor to display while the user drags
//	the minimized window.
HCURSOR CMcviewDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CMcviewDlg::OnSystem() 
{
	OnPaint();

	CDiaSystem dlg(&theView);
	dlg.m_rank=glRank;
	int iRet=dlg.DoModal();
	glRank=dlg.m_rank;
	if(IDC_RETURN_WND==iRet){
		theView.cleanView();
		CDialog::OnCancel();	
	}

	theView.SetFocus();
}

void CMcviewDlg::OnCancel() 
{
}

HBRUSH CMcviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		
	if(pWnd->GetDlgCtrlID()==IDC_ALARM){		
		// Set the text color to red.
		if(g_TopAlarmPt>=0){
			if(g_AlarmRecords[g_TopAlarmPt].m_ac==AC_Alarm){
				pDC->SetTextColor(RGB(255, 0, 0));
			}else if(g_AlarmRecords[g_TopAlarmPt].m_ac==AC_WriteDevice){
				pDC->SetTextColor(RGB(0, 0, 255));
			}else{
				pDC->SetTextColor(RGB(0,192,0));
			}
		}

		// Set the background mode for text to transparent 
		// so background will show thru.
		pDC->SetBkMode(TRANSPARENT);

		// Return handle to our CBrush object.
		//hbr = m_brush;
		hbr=m_brushWhite;
	}

	return hbr;
}

class CAutoPopup
{
public:

#define popupWhenVariableIsOn 1
#define popupAutoHide 2
#define popupOverlay 4
#define maxPopups 64

	struct popupEntry_t{
		TAG_NAME tn;
		__int flags;
		char associatedPicture[MAX_PATH];
	};

	__uint nEntries;
	popupEntry_t entries[maxPopups];

	void init();
	void run();
};

void CAutoPopup::init()
{
	__uint i;
	popupEntry_t * ent;

	nEntries = (__uint)GetPrivateProfileInt(
		"AutoPopup",
		"count",
		0,
		get_config_file()
		);
	if(nEntries >= maxPopups){
		nEntries = maxPopups;
	}
	ent = entries;
	char lineBuf[1024];
	char key[4];
	memset(&entries, 0, sizeof(entries));
	for(i=0; i<nEntries; i++){
		sprintf(key, "%d", i);
		GetPrivateProfileString(
			"AutoPopup",
			key,
			"",
			lineBuf,
			sizeof(lineBuf),
			get_config_file()
			);
		CArgs a;
		parse_arg_ex(lineBuf, &a, ",");
		if(a.argc != 5){
			continue;
		}
		if(!tagname_by_text(&ent->tn, a.argv[0])){
			continue;
		}
		if(atoi(a.argv[1])){
			ent->flags |= popupWhenVariableIsOn;
		}
		if(atoi(a.argv[2])){
			ent->flags |= popupOverlay;
		}
		if(atoi(a.argv[3])){
			ent->flags |= popupAutoHide;
		}
		strncpy(ent->associatedPicture, a.argv[4], MAX_PATH);
		ent->associatedPicture[MAX_PATH - 1] = 0;
		ent++;
	}
	nEntries = ent - entries;
}

CAutoPopup autoPopups;

void CMcviewDlg::doAutoPopup()
{
	// 2004/5/30, auto-popups
	do{
		__uint i;
		static bool bPopupInited;
		CAutoPopup::popupEntry_t * ent;
		
		if(!bPopupInited){
			autoPopups.init();
			bPopupInited = true;
		}
		ent = autoPopups.entries;
		if(!lock_rtdb(__false, 1000)){
			break;
		}
		for(i=0; i<autoPopups.nEntries; i++, ent++){
			//
			PCRTK_TAG tag;
			bool bPopup;
			
			tag = query_tag_f(&ent->tn);
			if(!tag){
				continue;
			}
			if(get_value_type(tag->s.Flags) != dt_bool){
				continue;
			}
			bPopup = tag->d.Value.Value.b? true : false;
			if(!(ent->flags & popupWhenVariableIsOn)){
				bPopup = !bPopup;
			}
			if(bPopup){
				if(ent->flags & popupOverlay){
					theView.LoadFile(ent->associatedPicture);
				}else{
					PopupView(ent->associatedPicture);
				}
			}else{
				if((ent->flags & popupAutoHide) && !(ent->flags & popupOverlay)){
					CFView * view;
					view = FindView(ent->associatedPicture);
					if(view){
						view->CloseWindow();
					}
				}
			}
		}
		unlock_rtdb();
	}while(0);
}


void CMcviewDlg::OnTimer(UINT nIDEvent) 
{
	static int iCount=0;

	if(nIDEvent == 1){
		glTimer++;

		//refresh HMI every 100 ms
		theView.Show();
		CFView* pPopView;
		POSITION pos;

		for(pos=m_PopViews.GetHeadPosition();pos!=NULL;){
			pPopView=(CFView*)(m_PopViews.GetNext(pos));
			pPopView->Show();
		}

		//refresh digital input dialog if exist
		if(gDiaInputD.m_bVisible) gDiaInputD.RefreshShow();

		//refresh analog input dialog if exist
		if(gDiaInputA.m_bVisible) gDiaInputA.RefreshShow();

		//refresh time every 250 ms
		if(iCount%4==0){
			GetLocalTime(&m_current);
			CString sCurrent;
			sCurrent.Format("%4d年%02d月%02d日 %02d:%02d:%02d",m_current.wYear,m_current.wMonth,
				m_current.wDay,m_current.wHour,m_current.wMinute,m_current.wSecond);
			SetDlgItemText(IDC_NOW,sCurrent);
		}

		iCount++;

		doAutoPopup();
	}else if(nIDEvent == 2){
		theView.LoadFile(m_viewLoop[m_loopCurrent]);
		m_loopCurrent++;
		if(m_loopCurrent >= m_loopCount){
			m_loopCurrent = 0;
		}
	}

	///
	if (iCount%30 == 0)
	{
		//取桌面工作区的大小，判断是否需要重绘工具条，
		RECT rt;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);//获取桌面工作区大小
		int y1 = ::GetSystemMetrics(SM_CYFULLSCREEN)-rt.bottom;//任务栏在下端时的高度，
		int y2 = rt.top;//任务栏在上端时的高度，
		int y3 = rt.left;//任务栏在左端时的高度，
		int y4 = ::GetSystemMetrics(SM_CXFULLSCREEN)-rt.right;//任务栏在右端时的高度，
		int x = 1;
		//if (flag_refresh != rectDesktop.bottom)
		//{
		//	OnPaint();
		//}
	}

	
	CDialog::OnTimer(nIDEvent);
}

void CMcviewDlg::OnAckalarm()
{	
	if(g_TopAlarmPt<0){
		return;
	}
	g_AlarmRecords[g_TopAlarmPt].m_ack=true;
    
	//moxb+++20130929
	SetDlgItemText(IDC_ALARM,"");//确认该报警后，清空报警任务条
	char* m_msg;
	m_msg = g_AlarmRecords[g_TopAlarmPt].m_msg;
    this->SendFakeAlarm(m_msg);
    //moxe+++20130929
	
	// ok, advance g_TopAlarmPt to next un-confirmed
	// alarm entry
	g_TopAlarmPt = FindTopAlarmNoOperate();//mox+++
	//if(g_TopAlarmPt>=0){
		// update the list-box
	if(g_TopAlarmPt>=0 && g_AlarmRecords[g_TopAlarmPt].m_ac < 2 && g_AlarmRecords[g_TopAlarmPt].m_ack == false){//mox+++20130929
		CString sAlarm;
		sAlarm.Format("%s",g_AlarmRecords[g_TopAlarmPt].m_msg);
		SetDlgItemText(IDC_ALARM,sAlarm);
	}else{
		SetDlgItemText(IDC_ALARM,"");
	}
}

//moxb+++20130929
/*将已经报警伪装为报警数据包，内含已确认的报警信息的packet
把当前刚确认的alarm的信息从数组g_AlarmRecords[]中反解析出来传递给ap中对应的成员变量,
其中报警信息在ackmsg中，并不关心group.Data和tag.Data
*/
class CAckAlarmTransaction : public CWaitableTransaction
{
public:
	CAckAlarmTransaction()
	{
		add_transaction(vbus_alarm, this);
	}
	__bool QueryAckAlarm(char* ackmsg,__uint timeout)
	{
		char buf[2048] ={0};
		RTK_ALARM_PACKET &ap = *((RTK_ALARM_PACKET *)buf);
		TAG_VALUE va;
		memset((void*)&va, 0, sizeof(TAG_VALUE));

		ap.Tag.node.Data[0] = 0;	
		strcpy(ap.Tag.sname.group.Data, "groupname");  //为保证ap结构完整性，此处无实际意义
		strcpy(ap.Tag.sname.tag.Data, "tagname");      //为保证ap结构完整性，此处无实际意义
		ap.Class = AC_AckAlarm;                        //报警处理状态为伪装
		ap.Rank = 600;                                 //报警级别，在AlarmCallBack中rank大于500才接收
		memcpy((void*)&ap.Value, (void*)&va,sizeof(TAG_VALUE));//为保证ap结构完整性，将value置为0，此处无实际意义
		sprintf(ap.Msg, "%s", ackmsg); 

		//广播
		int res;
		res = broadcast_rtk_data(
			vbus_alarm, 
			PORT_ALL_CLIENT, 
			PT_Alarm,
			&ap,
			sizeof(RTK_ALARM_PACKET)+strlen(ap.Msg) + 1
			);
		return Wait(timeout);
	}
private:
	virtual void OnEvent(PCRTK_PACKET p)
	{
		SetEvent();
	}
};
bool CMcviewDlg::SendFakeAlarm(char* ackmsg)
{	

	CAckAlarmTransaction *tr = new CAckAlarmTransaction();
	if(tr){
		tr->QueryAckAlarm(ackmsg, 500);
	}
	return true;
}
//moxe+++20130929


void CMcviewDlg::OnAlarmlist()//报警监视
{
	alarmDlg.ShowWindow(SW_SHOWNORMAL);
	theView.ShowWindow(SW_HIDE);
	m_alarmList.ShowWindow(SW_HIDE);
	m_alarmClose.ShowWindow(SW_SHOWNORMAL);
}

void CMcviewDlg::OnBnClickedClosealarm()//监视关闭
{
	// TODO: 在此添加控件通知处理程序代码
	alarmDlg.ShowWindow(SW_HIDE);
	theView.ShowWindow(SW_SHOWNORMAL);
	m_alarmList.ShowWindow(SW_SHOWNORMAL);
	m_alarmClose.ShowWindow(SW_HIDE);
}


void CMcviewDlg::OnIndex()//目录 
{
    OnPaint();

	alarmDlg.ShowWindow(SW_HIDE);
	theView.ShowWindow(SW_SHOWNORMAL);
	m_alarmList.ShowWindow(SW_SHOWNORMAL);
	m_alarmClose.ShowWindow(SW_HIDE);

	m_curView++;
	if(m_curView>9) m_curView=0;
	m_viewList[m_curView]="index.gdf";
	theView.LoadFile("index.gdf");
	theView.LoadFile("index.gdf");
	theView.SetFocus();
}

void CMcviewDlg::OnOK() 
{
	//do nothing	
	//CDialog::OnOK();
}

void CMcviewDlg::OnNow() 
{
	OnPaint();
	theView.SetFocus();	
}

void CMcviewDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	theView.SetFocus();	
	CDialog::OnLButtonUp(nFlags, point);
}

void CMcviewDlg::OnAlarm() 
{
	OnPaint();
	theView.SetFocus();	
}

void CMcviewDlg::OnKey() 
{
	theView.SetFocus();	
}

void CMcviewDlg::OnPrevious() 
{
	OnPaint();

	for(int i=0;i<10;i++){
		m_curView--;
		if(m_curView<0) m_curView=9;
		if(!m_viewList[m_curView].IsEmpty()){
			theView.LoadFile(m_viewList[m_curView]);
			break;
		}
	}
	theView.SetFocus();		
}

void CMcviewDlg::OnNext() 
{
	OnPaint();

	for(int i=0;i<10;i++){
		m_curView++;
		if(m_curView>9) m_curView=0;
		if(!m_viewList[m_curView].IsEmpty()){
			theView.LoadFile(m_viewList[m_curView]);
			break;
		}
	}
	theView.SetFocus();		
}


CFView * CMcviewDlg::FindView(CString sFileName)
{
	CFView* pPopView;
	POSITION pos;
	for(pos=m_PopViews.GetHeadPosition();pos!=NULL;){
		pPopView=(CFView*)(m_PopViews.GetNext(pos));
		//pPopView->Show();
		if(pPopView->m_sFileName==sFileName){
			//pPopView->ShowWindow(SW_SHOW);
			return pPopView;
		}
	}
	return NULL;
}

void CMcviewDlg::PopupView(CString sFileName,int x,int y)
{
	CFView* pPopView;

	pPopView = FindView(sFileName);

	if(pPopView){
		pPopView->SetFocus();
		return;
	}

	pPopView = new (CFView);
	//CFPopView *pPopView=new(CFPopView);
	pPopView->m_pCtrDlg = this;
	pPopView->m_bPopup = true;
	pPopView->Create(IDD_POPUP, &theView);
	pPopView->LoadFile(sFileName, x, y);
	
	// 2003/7/9 Jackie
	theView.PlaceWindowAt(pPopView, x, y);

	pPopView->ShowWindow(SW_SHOW);
	pPopView->m_pos = m_PopViews.AddTail(pPopView);
	pPopView = NULL;

	CloseOldPopView();
}

void CMcviewDlg::RemovePopView(POSITION pos)
{
	m_PopViews.RemoveAt(pos);
}

void CMcviewDlg::OnLoop() 
{
	// test code for bug-2003-7-19
	// TerminateProcess(GetCurrentProcess(), 0);
	// no use, not caused by this
	
	OnPaint();

	theView.SetFocus();		
	CWnd *pWndLoop = GetDlgItem(IDC_LOOP);

	if(m_bShowLoop){
		m_bShowLoop = false;
		KillTimer(2);
		pWndLoop->SetWindowText("循环");
		return;
	}

	CDlgLoop dlgLoop;
	dlgLoop.m_interval = m_loopInterval;

	if(dlgLoop.DoModal() != IDOK){
		return;
	}
	
	m_loopCurrent = 0;
	m_loopCount = 0;

	CString sLoopFile = ((CMcviewApp*)::AfxGetApp())->m_path+"loops";
	FILE *pLoops = fopen((LPCTSTR)sLoopFile, "r");
	ASSERT(pLoops != NULL);
	CString sItem;
	char buf[1024];
	while(fgets(buf, 1024, pLoops)){
		sItem = buf;
		sItem.Remove(10);
		sItem.Remove(13);
		m_viewLoop[m_loopCount] = sItem + ".gdf";
		m_loopCount++;
		if(m_loopCount >= 32){
			break;
		}
	}
	fclose(pLoops);

	if(m_loopCount < 2){
		return;
	}
	m_loopInterval = dlgLoop.m_interval;

	m_bShowLoop = true;
	SetTimer(2, m_loopInterval * 1000, NULL);
	pWndLoop->SetWindowText("停止");

	theView.LoadFile(m_viewLoop[m_loopCurrent]);
	m_loopCurrent++;
	if(m_loopCurrent >= m_loopCount){
		m_loopCurrent = 0;
	}
}

void CMcviewDlg::OnDestroy() 
{
	g_ProxyDispatchTable.OnAlarm = 0;
	// TODO: Add your message handler code here	
}

BOOL CMcviewDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	mcview_ext_cmd_t * cmd;
	cmd = (mcview_ext_cmd_t * )pCopyDataStruct->lpData;
	if(pCopyDataStruct->cbData == sizeof(*cmd)){
		set_working_dir(cmd->projDir);
		theApp.m_path = cmd->projDir;
		theView.LoadFile(cmd->fileName);
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}


void CMcviewDlg::PopupTrend(CString tag1, CString tag2, CString tag3, CString tag4, CString tag5)
{
	CFView* pPopView;

	pPopView = new (CFView);
	pPopView->m_pCtrDlg = this;
	pPopView->m_bPopup = true;
	pPopView->Create(IDD_POPUP,&theView);
	pPopView->LoadFile("DEF_TREND.gdf",0,0);
	pPopView->setTrendTags(tag1, tag2, tag3, tag4, tag5);

	RECT rectDesktop;
	::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
	theView.PlaceWindowAt(pPopView, rectDesktop.right / 2, 0);

	pPopView->ShowWindow(SW_SHOW);
	pPopView->m_pos=m_PopViews.AddTail(pPopView);
	pPopView=NULL;

	CloseOldPopView();
}

void CMcviewDlg::CloseOldPopView()
{
	while(m_PopViews.GetCount() > g_maxPopup){
		CFView* pPopView;
		pPopView = (CFView*)m_PopViews.GetHead();
		m_PopViews.RemoveHead();
		pPopView->DestroyWindow();
		delete pPopView;
	}
}

LRESULT CMcviewDlg::OnRefreshMessage(WPARAM wParam, LPARAM lParam)
{
	OnPaint();
	return 0;
}


void CMcviewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
    OnPaint();
	CDialog::OnLButtonDown(nFlags, point);
}


//mox20150213确保任务控制栏在最下端，不受到拖动windows系统任务栏的影响而移动位置
void CMcviewDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	// TODO: Add your message handler code here
	RECT rectDesktop;
	::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
	int iW=rectDesktop.right;
	flag_refresh = rectDesktop.bottom;
	SetWindowPos(NULL,0,rectDesktop.bottom-iW/32,iW,iW/32,0);
}
