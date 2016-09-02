// DiaAlarmDlg.cpp : ʵ���ļ�
//= UPDATE RECORD
//= Update for    Version     Update by     Date         Update                           
//= (1-1)         1.2.0.8        mox      2014-10-11      1.UpdateAckList()�������ж�������"!r->m_ack"��ֻ��ʾδȷ�ϵı�����


#include "stdafx.h"
#include "mcview.h"
#include "DiaAlarmDlg.h"
#include "libhis.h"
#include "event.h"
#include "direct.h"
#include "rtkproxy.h"
#include <string>
#include "DiaQueryAlarm.h"
#include <vector>
#include "tlhelp32.h"

using namespace std;

CAlarmRecord g_AckAlarmRecords[RCT_ALARM_COUNT];

#ifdef _DEBUG
#define new DEBUG_NEW
#endif  

#define ALARMPOOL	1024
#define ALARMLEN	1024
typedef CPMCArchive<CPMCEvent, ALARMLEN> CPMCAlarms;

#define OPERATEPOOL 1024
#define OPERATELEN	1024
typedef CPMCArchive<CPMCEvent, OPERATELEN> CPMCOperates;

#define SOEPOOL 1024
#define SOELEN	1024
typedef CPMCArchive<CPMCEvent, SOELEN> CPMCSoes;

CPMCEvent evtsList[MAX_GET_RECORD];
static CPMCAlarms * alarms;
static CPMCAlarms * alarms_x;
static CPMCOperates * operates;

extern long glViewWidth;
extern long glViewHeight;

// CDiaAlarmDlg �Ի���

IMPLEMENT_DYNAMIC(CDiaAlarmDlg, CDialog)

CDiaAlarmDlg::CDiaAlarmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaAlarmDlg::IDD, pParent)
{

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));

}

CDiaAlarmDlg::~CDiaAlarmDlg()
{
}

void CDiaAlarmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALARM_QUERY, m_alarmquery);
	DDX_Control(pDX, IDC_PRINT_BTN, m_print);
	DDX_Control(pDX, IDC_OUTFILE_BTN, m_outfile);
	DDX_Control(pDX, IDC_AckAlarmList, m_acklistalarm);
	DDX_Control(pDX, IDC_LIST_HIS, m_HisList);
	DDX_Control(pDX, IDC_LIST_ACKED, m_ackedlistalarm);
	DDX_Control(pDX, IDC_ACK, m_ackline);
	DDX_Control(pDX, IDC_ACKALL, m_ackall);
	DDX_Control(pDX, IDC_STATIC_ALARM_ACK, m_alarmackgroupbox);
	DDX_Control(pDX, IDC_SOUND_ON, m_soundOn);
	DDX_Control(pDX, IDC_SOUND_OFF, m_soundOff);
}


BEGIN_MESSAGE_MAP(CDiaAlarmDlg, CDialog)
	ON_BN_CLICKED(IDC_ALARM, &CDiaAlarmDlg::OnBnClickedAlarm)
	ON_BN_CLICKED(IDC_ALARM_ACKED, &CDiaAlarmDlg::OnBnClickedAlarmAcked)
	ON_BN_CLICKED(IDC_ALARM_HIS, &CDiaAlarmDlg::OnBnClickedAlarmHis)
	ON_BN_CLICKED(IDC_ALARM_QUERY, &CDiaAlarmDlg::OnBnClickedAlarmQuery)
	ON_BN_CLICKED(IDC_PRINT_BTN, &CDiaAlarmDlg::OnBnClickedPrintBtn)
	ON_BN_CLICKED(IDC_OUTFILE_BTN, &CDiaAlarmDlg::OnBnClickedOutfileBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ACK, &CDiaAlarmDlg::OnBnClickedAck)
	ON_BN_CLICKED(IDC_ACKALL, &CDiaAlarmDlg::OnBnClickedAckall)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_AckAlarmList, &CDiaAlarmDlg::OnLvnGetdispinfoAckalarmlist)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_AckAlarmList, &CDiaAlarmDlg::OnNMCustomdrawAckalarmlist)
	ON_NOTIFY(NM_KILLFOCUS, IDC_AckAlarmList, &CDiaAlarmDlg::OnNMKillfocusAckalarmlist)
	ON_NOTIFY(NM_SETFOCUS, IDC_AckAlarmList, &CDiaAlarmDlg::OnNMSetfocusAckalarmlist)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_HIS, &CDiaAlarmDlg::OnLvnGetdispinfoListHis)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_HIS, &CDiaAlarmDlg::OnNMCustomdrawListHis)
	ON_BN_CLICKED(IDC_SOUND_ON, &CDiaAlarmDlg::OnBnClickedSoundOn)
	ON_BN_CLICKED(IDC_SOUND_OFF, &CDiaAlarmDlg::OnBnClickedSoundOff)
END_MESSAGE_MAP()


// CDiaAlarmDlg ��Ϣ�������
BOOL CDiaAlarmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_acklistalarm.SetBkColor(RGB(196,197,206));
	m_ackedlistalarm.SetBkColor(RGB(196,197,206));
	m_ackedlistalarm.SetTextBkColor(RGB(196,197,206));
	m_HisList.SetBkColor(RGB(196,197,206));

	RECT rectDesktop;
	::GetWindowRect (::GetDesktopWindow(), &rectDesktop);
	int wx = rectDesktop.right;
	int wy = rectDesktop.bottom;

	SetWindowPos(NULL, 0, wx/10, wx, wy-wx/10-wx/16,NULL);

	//RECT rect;
	//GetClientRect(&rect);
	//int cx = rect.right;
	//int cy = rect.bottom;
	int cx = glViewWidth - 10;
	int cy = glViewHeight;

	CWnd* pPrint = GetDlgItem(IDC_PRINT_BTN);
	CWnd* pOutFile = GetDlgItem(IDC_OUTFILE_BTN);

	//�б�ؼ�
	CWnd* pAckAlarmList = GetDlgItem(IDC_AckAlarmList);
	CWnd* pHisAlarmList = GetDlgItem(IDC_LIST_HIS);
	CWnd* pAckedAlarmList = GetDlgItem(IDC_LIST_ACKED);

	//������ť
	CWnd* pAlarmBtn = GetDlgItem(IDC_ALARM);
	CWnd* pAlarmAckedBtn = GetDlgItem(IDC_ALARM_ACKED);
	CWnd* pAlarmHisBtn = GetDlgItem(IDC_ALARM_HIS);
	CWnd* pAlarmQueryBtn = GetDlgItem(IDC_ALARM_QUERY);

	CWnd* pGBAck = GetDlgItem(IDC_STATIC_ALARM_ACK);
	CWnd* pAckLine = GetDlgItem(IDC_ACK);
	CWnd* pAckAll = GetDlgItem(IDC_ACKALL);
	CWnd* pAlarmTitle = GetDlgItem(IDC_ALARMTITLE);

	//������ť
    CWnd* pAlarmSoundOnBtn = GetDlgItem(IDC_SOUND_ON);
	CWnd* pAlarmSoundOffBtn = GetDlgItem(IDC_SOUND_OFF);

	//
	pAckAlarmList->SetWindowPos(NULL,1,40,cx,cy-120,SWP_NOZORDER);
	pHisAlarmList->SetWindowPos(NULL,1,40,cx,cy-50,SWP_NOZORDER);
	pAckedAlarmList->SetWindowPos(NULL,1,40,cx,cy-50,SWP_NOZORDER);

	pAlarmBtn->MoveWindow(1,5,100,30);
	pAlarmAckedBtn->MoveWindow(111,5,100,30);
	pAlarmHisBtn->MoveWindow(221,5,100,30);

	pAlarmQueryBtn->MoveWindow(cx/2+400,5,80,30);
	pPrint->MoveWindow(cx/2+490,5,80,30);
	pOutFile->MoveWindow(cx/2+580,5,80,30);

	pAlarmSoundOnBtn->MoveWindow(cx-100,1,70,30);
	pAlarmSoundOffBtn->MoveWindow(cx-100,1,70,30);

	pGBAck->SetWindowPos(NULL,1,cy-70,220,60,SWP_NOZORDER);
	pAckLine->SetWindowPos(NULL,21,cy-50,80,30,SWP_NOZORDER);
	pAckAll->SetWindowPos(NULL,121,cy-50,80,30,SWP_NOZORDER);
	pAlarmTitle->MoveWindow(cx/2-50,1,200,33);

	m_ListUpdate = true;
	ListAlarmInit();

	SetTimer(1,1000,0);
	SetTimer(2,2000,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDiaAlarmDlg::OnBnClickedAlarm()//��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ackedlistalarm.ShowWindow(SW_HIDE);
	m_acklistalarm.ShowWindow(SW_SHOW);
	m_HisList.ShowWindow(SW_HIDE);
	m_alarmquery.ShowWindow(SW_HIDE);
	m_print.ShowWindow(SW_HIDE);
	m_outfile.ShowWindow(SW_HIDE);
	m_alarmackgroupbox.ShowWindow(SW_SHOW);
	m_ackline.ShowWindow(SW_SHOW);
	m_ackall.ShowWindow(SW_SHOW);

	GetDlgItem(IDC_ALARMTITLE)->SetWindowText(_T("ʵʱ��������"));
}

void CDiaAlarmDlg::OnBnClickedAlarmAcked()//��ȷ�ϱ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateAckedList();

	m_ackedlistalarm.ShowWindow(SW_SHOW);
	m_acklistalarm.ShowWindow(SW_HIDE);
	m_HisList.ShowWindow(SW_HIDE);
	m_alarmquery.ShowWindow(SW_HIDE);
	m_print.ShowWindow(SW_HIDE);
	m_outfile.ShowWindow(SW_HIDE);
	m_alarmackgroupbox.ShowWindow(SW_HIDE);
	m_ackline.ShowWindow(SW_HIDE);
	m_ackall.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_ALARMTITLE)->SetWindowText(_T("��ȷ�ϱ���"));
}

void CDiaAlarmDlg::OnBnClickedAlarmHis()//��ʷ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ackedlistalarm.ShowWindow(SW_HIDE);
	m_acklistalarm.ShowWindow(SW_HIDE);
	m_HisList.ShowWindow(SW_SHOW);

	m_alarmquery.ShowWindow(SW_SHOW);
	m_print.ShowWindow(SW_SHOW);
	m_outfile.ShowWindow(SW_SHOW);
	m_alarmackgroupbox.ShowWindow(SW_HIDE);
	m_ackline.ShowWindow(SW_HIDE);
	m_ackall.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_ALARMTITLE)->SetWindowText(_T("��ʷ����"));
}

void CDiaAlarmDlg::OnBnClickedAlarmQuery()//��ѯ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDiaQueryAlarm dlg(this);

	COleDateTime dt;
	COleDateTime tm;
	PMCSYSTIME tmSys;
	PMCTIME stm, etm;
	if(m_qIniTime){
		dlg.m_date_start = m_dtSta;
		dlg.m_time_start = m_dtSta;
		dlg.m_date_end = m_dtEnd;
		dlg.m_time_end = m_dtEnd;
		dlg.m_cur_sel = m_cur_sel;
		dlg.m_filter = m_filter;
	}
	if(dlg.DoModal()==IDOK){
		m_qIniTime=true;
		/*		theView1.SetFocus();*/

		dt = dlg.m_date_start;
		tm = dlg.m_time_start;
		tmSys.year = dt.GetYear();
		tmSys.month = dt.GetMonth();
		tmSys.day = dt.GetDay();
		tmSys.hour = tm.GetHour();
		tmSys.minute = tm.GetMinute();
		tmSys.second = tm.GetSecond();
		tmSys.milliSeconds = 0;
		systm2tm(&tmSys, &stm);
		m_dtSta.SetDateTime(tmSys.year, tmSys.month, tmSys.day,
			tmSys.hour, tmSys.minute, tmSys.second);
		dt=dlg.m_date_end;
		tm=dlg.m_time_end;
		tmSys.year=dt.GetYear();
		tmSys.month=dt.GetMonth();
		tmSys.day=dt.GetDay();
		tmSys.hour=tm.GetHour();
		tmSys.minute=tm.GetMinute();
		tmSys.second=tm.GetSecond();
		tmSys.milliSeconds=0;
		systm2tm(&tmSys, &etm);
		m_dtEnd.SetDateTime(tmSys.year, tmSys.month, tmSys.day,
			tmSys.hour, tmSys.minute, tmSys.second);

		m_cur_sel = dlg.m_cur_sel;
		m_filter = dlg.m_filter;
		m_filter.TrimLeft();
		m_filter.TrimRight();

		int count=0;
		switch(m_cur_sel){
		case 1:
			count = alarms->getRecord_r(stm, etm, MAX_GET_RECORD, evtsList, m_filter);
			break;
		case 2:
            count = alarms_x->getRecord_r(stm, etm, MAX_GET_RECORD, evtsList, m_filter);
			break;
		case 0:
			count = operates->getRecord_r(stm, etm, MAX_GET_RECORD, evtsList, m_filter);
			break;
		default:
			break;
		}	
		GetLastError();

		int i;
		for(i = 0; i < MAX_GET_RECORD; i++){
			evtsList[i].valid = false;
		}
		for(i = 0; i < count; i++){
			evtsList[i].valid = true;
		}

		LV_ITEM listItem;
		ZeroMemory(&listItem, sizeof(listItem));
		m_HisList.SetRedraw(FALSE);
		m_HisList.DeleteAllItems();
		listItem.mask = LVIF_TEXT|LVIF_PARAM;
		for(int i = 0; i < MAX_GET_RECORD; i++){
			if (evtsList[i].valid)
			{
				listItem.iSubItem = 0;
				listItem.iItem = m_HisList.GetItemCount();
				listItem.pszText = LPSTR_TEXTCALLBACK;
				listItem.cchTextMax = 50;
				listItem.lParam = (LPARAM)i;

				m_HisList.InsertItem(&listItem);
				m_HisList.SetItemText(listItem.iItem,1, LPSTR_TEXTCALLBACK);

			}
		}
		m_HisList.SetRedraw(TRUE);
	}
}


void CDiaAlarmDlg::OnBnClickedPrintBtn()//��ӡ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPrintDialog dlg(FALSE);
	if (IDOK!=dlg.DoModal())
	{
		return;
	}

	HDC hdcPrinter = dlg.GetPrinterDC();
	if (hdcPrinter == NULL){
		MessageBox("�޷��ҵ���ӡ��",NULL,MB_OK|MB_ICONSTOP);
	}else{
		CDC dcPrinter;
		dcPrinter.Attach(hdcPrinter);

		// call StartDoc() to begin printing
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = "�¼���¼����";

		// if it fails, complain and exit gracefully
		if (dcPrinter.StartDoc(&docinfo) < 0){
			MessageBox("�޷���ʼ����ӡ��",NULL,MB_OK|MB_ICONSTOP);
		}else{
			dcPrinter.SetMapMode(MM_TWIPS);
			CFont *pOldFont;
			CFont fnt;
			if(fnt.CreatePointFont(70,"����",&dcPrinter)){
				pOldFont=(CFont*)dcPrinter.SelectObject(&fnt);                  
			}else{
				pOldFont=(CFont*)dcPrinter.SelectStockObject(DEVICE_DEFAULT_FONT);
			}
			CPoint pt(dcPrinter.GetDeviceCaps(HORZRES),dcPrinter.GetDeviceCaps(VERTRES));
			dcPrinter.DPtoLP (&pt);
			pt.y=-pt.y;
			CSize sz=dcPrinter.GetTextExtent("���   ");
			CSize sz1=dcPrinter.GetTextExtent("ʱ��                        ");
			CSize sz2=dcPrinter.GetTextExtent("����                  ");
			CSize sz3=dcPrinter.GetTextExtent("����                      ");
			CSize sz4=dcPrinter.GetTextExtent("����״̬   ");
			CSize sz5=dcPrinter.GetTextExtent("��Ϣ       ");
			CSize sz6=dcPrinter.GetTextExtent("��ǰֵ/����վ   ");
			CSize sz7=dcPrinter.GetTextExtent("����Ա");
			int lineHeight=sz.cy*1.5;
			int lineCount=(pt.y-sz.cy*10)/lineHeight;
			if(lineCount<0) return;
			long id=0;
			CString str;
			int i,iPage=0;
			int cLeftGap=800;
			while(evtsList[id].valid){
				// start a page
				if (dcPrinter.StartPage() < 0){
					MessageBox("�޷���ʼ��ҳ",NULL,MB_OK|MB_ICONSTOP);
					dcPrinter.AbortDoc();
				}else{
					// actually do some printing
					//print title
					iPage++;
					str.Format("�¼���¼���� ��%2dҳ",iPage);
					dcPrinter.TextOut(cLeftGap,-lineHeight*3,str);
					str.Format("���");
					dcPrinter.TextOut(cLeftGap,-lineHeight*5,str);
					str.Format("ʱ��");
					dcPrinter.TextOut(cLeftGap+sz.cx,-lineHeight*5,str);
					str.Format("����");
					dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx,-lineHeight*5,str);
					str.Format("����");
					dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx,-lineHeight*5,str);//�������������
					str.Format("����״̬");
					dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx,-lineHeight*5,str);
					str.Format("��Ϣ");
					dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx+sz4.cx,-lineHeight*5,str);
					str.Format("��ǰֵ/����վ");
					dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx+sz4.cx+sz5.cx,-lineHeight*5,str);
					str.Format("����Ա");
					dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx+sz4.cx+sz5.cx+sz6.cx,-lineHeight*5,str);
					for(i=0;i<lineCount;i++){
						if(evtsList[id].valid){
							CArgs args;
							parse_arg_ey(evtsList[id].data, &args, ";");
							str.Format("%3d",id);
							dcPrinter.TextOut(cLeftGap,-lineHeight*(6+i),str);
							str.Format("%s",args.argv[0]);
							dcPrinter.TextOut(cLeftGap+sz.cx,-lineHeight*(6+i),str);
							str.Format("%s",args.argv[1]);
							dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx,-lineHeight*(6+i),str);
							str.Format("%s",args.argv[2]);
							dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx,-lineHeight*(6+i),str);
							str.Format("%s",args.argv[3]);
							dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx,-lineHeight*(6+i),str);
							str.Format("%s",args.argv[4]);
							dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx+sz4.cx,-lineHeight*(6+i),str);
							str.Format("%s",args.argv[5]);
							dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx+sz4.cx+sz5.cx,-lineHeight*(6+i),str);
							str.Format("%s",args.argv[6]);
							dcPrinter.TextOut(cLeftGap+sz.cx+sz1.cx+sz2.cx+sz3.cx+sz4.cx+sz5.cx+sz6.cx,-lineHeight*(6+i),str);
							id++;
						}else{
							break;
						}
					}
				}
				dcPrinter.EndPage();
			}
			dcPrinter.SelectObject(pOldFont);
			dcPrinter.EndDoc();
		}
	}
}

void CDiaAlarmDlg::OnBnClickedOutfileBtn()//������ļ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(FALSE, "csv", "", OFN_OVERWRITEPROMPT, "csv Files(*.csv)|*.csv||");
	if (dlg.DoModal() == IDOK)
	{
		CString strFile = dlg.GetPathName();
		FILE *pFile = fopen(strFile, "w+");
		if(pFile){
			CString str;
			str.Format("���,ʱ��,����,����,����״̬,��Ϣ,��ǰֵ/����վ,����Ա\n");
			fprintf(pFile, "%s", str);
			long id=0;
			while(evtsList[id].valid){
				CArgs args;
				parse_arg_ey(evtsList[id].data, &args, ";");
				str.Format("%3d,%s,%s,%s,%s,%s,%s\n", id, args.argv[0], args.argv[1], args.argv[2], args.argv[3], args.argv[4], args.argv[5], args.argv[6]);
				fprintf(pFile, "%s", str);
				id++;
			}
			fclose(pFile);
		}
	}
}

void CDiaAlarmDlg::ListAlarmInit()
{
	RECT listRect;
	GetClientRect(&listRect);
	int aiW=listRect.right/9;

	m_acklistalarm.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	LV_COLUMN listColumn;
	TCHAR* rgszColumns[] = { _T("���"), _T("ʱ��"),_T("����"),_T("����"),_T("����״̬"),_T("��Ϣ"),_T("��ǰֵ/����վ"),_T("����Ա")};
	listColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt = LVCFMT_LEFT;
	listColumn.cx = 1.25*aiW/4;
	listColumn.iSubItem = 0;
	listColumn.pszText = rgszColumns[0];
	m_acklistalarm.InsertColumn(0, &listColumn);
	listColumn.cx = 5*aiW/2;
	listColumn.iSubItem = 1;
	listColumn.pszText = rgszColumns[1];
	m_acklistalarm.InsertColumn(1, &listColumn);
	listColumn.iSubItem = 2;
	listColumn.pszText = rgszColumns[2];
	listColumn.cx = 1.4*aiW;
	m_acklistalarm.InsertColumn(2, &listColumn);
	listColumn.iSubItem = 3;
	listColumn.pszText = rgszColumns[3];
	listColumn.cx = 1.3*aiW - 20;
	m_acklistalarm.InsertColumn(3, &listColumn);
	listColumn.iSubItem = 4;
	listColumn.pszText = rgszColumns[4];
	listColumn.cx = 2*aiW/2;
	m_acklistalarm.InsertColumn(4, &listColumn);
	listColumn.iSubItem = 5;
	listColumn.pszText = rgszColumns[5];
	listColumn.cx = 2*aiW/4;
	m_acklistalarm.InsertColumn(5, &listColumn);
	listColumn.iSubItem = 6;
	listColumn.pszText = rgszColumns[6];
	listColumn.cx = 3*aiW/4;
	m_acklistalarm.InsertColumn(6, &listColumn);
	listColumn.iSubItem = 7;
	listColumn.pszText = rgszColumns[7];
	listColumn.cx = 2*aiW/4;
	m_acklistalarm.InsertColumn(7, &listColumn);


	LV_COLUMN listColumn3;
	TCHAR* rgszColumns2[] = { _T("���"), _T("ʱ��"),_T("����"),_T("����"),_T("����״̬"),_T("��Ϣ"),_T("��ǰֵ/����վ"),_T("����Ա")};
	listColumn3.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn3.fmt = LVCFMT_LEFT;
	listColumn3.cx = 1.25*aiW/4;
	listColumn3.iSubItem = 0;
	listColumn3.pszText = rgszColumns2[0];
	m_ackedlistalarm.InsertColumn(0, &listColumn3);
	listColumn3.iSubItem = 1;
	listColumn3.pszText = rgszColumns2[1];
	listColumn3.cx = 5*aiW/2;
	m_ackedlistalarm.InsertColumn(1, &listColumn3);
	listColumn3.iSubItem = 2;
	listColumn3.pszText = rgszColumns2[2];
	listColumn3.cx = 1.4*aiW;
	m_ackedlistalarm.InsertColumn(2, &listColumn3);
	listColumn3.iSubItem = 3;
	listColumn3.pszText = rgszColumns2[3];
	listColumn3.cx = 1.3*aiW - 20;
	m_ackedlistalarm.InsertColumn(3, &listColumn3);
	listColumn3.iSubItem = 4;
	listColumn3.pszText = rgszColumns2[4];
	listColumn3.cx = 2*aiW/2;
	m_ackedlistalarm.InsertColumn(4, &listColumn3);
	listColumn3.iSubItem = 5;
	listColumn3.pszText = rgszColumns2[5];
	listColumn3.cx = 2*aiW/4;
	m_ackedlistalarm.InsertColumn(5, &listColumn3);
	listColumn3.iSubItem = 6;
	listColumn3.pszText = rgszColumns2[6];
	listColumn3.cx = 3*aiW/4;
	m_ackedlistalarm.InsertColumn(6, &listColumn3);
	listColumn3.iSubItem = 7;
	listColumn3.pszText = rgszColumns2[7];
	listColumn3.cx = 2*aiW/4;
	m_ackedlistalarm.InsertColumn(7, &listColumn3);

	//��ʷ����
	LV_COLUMN listColumn4;
	TCHAR* rgszColumns3[] = { _T("���"), _T("ʱ��"),_T("����"),_T("����"),_T("����״̬"),_T("��Ϣ"),_T("��ǰֵ/����վ"),_T("����Ա")};
	listColumn4.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn4.fmt = LVCFMT_LEFT;
	listColumn4.cx = 1.25*aiW/4;
	listColumn4.iSubItem = 0;
	listColumn4.pszText = rgszColumns3[0];
	m_HisList.InsertColumn(0, &listColumn4);
	listColumn4.iSubItem = 1;
	listColumn4.pszText = rgszColumns3[1];
	listColumn4.cx = 5*aiW/2;
	m_HisList.InsertColumn(1, &listColumn4);
	listColumn4.iSubItem = 2;
	listColumn4.pszText = rgszColumns3[2];
	listColumn4.cx = 1.4*aiW;
	m_HisList.InsertColumn(2, &listColumn4);
	listColumn4.iSubItem = 3;
	listColumn4.pszText = rgszColumns3[3];
	listColumn4.cx = 1.3*aiW - 20;
	m_HisList.InsertColumn(3, &listColumn4);
	listColumn4.iSubItem = 4;
	listColumn4.pszText = rgszColumns3[4];
	listColumn4.cx = 2*aiW/2;
	m_HisList.InsertColumn(4, &listColumn4);
	listColumn4.iSubItem = 5;
	listColumn4.pszText = rgszColumns3[5];
	listColumn4.cx = 2*aiW/4;
	m_HisList.InsertColumn(5, &listColumn4);
	listColumn4.iSubItem = 6;
	listColumn4.pszText = rgszColumns[6];
	listColumn4.cx = 3*aiW/4;
	m_HisList.InsertColumn(6, &listColumn4);
	listColumn4.iSubItem = 7;
	listColumn4.pszText = rgszColumns[7];
	listColumn4.cx = 2*aiW/4;
	m_HisList.InsertColumn(7, &listColumn4);

	string str, s1, s2, s3;

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
	s3 = str + "/operate";
	_mkdir(s3.c_str());

	alarms = new CPMCAlarms;
	if(alarms){
		alarms->initialize(s1.c_str(), ALARMPOOL);
	}
	alarms_x = new CPMCAlarms;
	if(alarms_x){
		alarms_x->initialize(s2.c_str(), ALARMPOOL);
	}
	operates = new CPMCAlarms;
	if(operates){
		operates->initialize(s3.c_str(), ALARMPOOL);
	}
}

void CDiaAlarmDlg::UpdateAckList()
{
	LV_ITEM listItem;
	int pt, guardian;
	CString psztemp;

	ZeroMemory(&listItem, sizeof(listItem));
	listItem.mask = LVIF_TEXT|LVIF_PARAM;

	m_acklistalarm.SetRedraw(FALSE);
	m_acklistalarm.DeleteAllItems();
	if(g_AlarmQueueTail < 0){
		goto __done;
	}

	guardian = g_AlarmQueueTail;

	CAlarmRecord * r;
	
	pt = (g_AlarmQueueTail) % RCT_ALARM_COUNT;
	for(;;)
	{
		r = &g_AlarmRecords[pt];
		if(r->m_valid && !r->m_ack)//add "!r->m_ack"��ֻ��ʾδȷ�ϵı�����
		{
			listItem.iSubItem = 0;
			listItem.iItem = m_acklistalarm.GetItemCount();
			listItem.pszText = LPSTR_TEXTCALLBACK;
			listItem.cchTextMax = 50;
			listItem.lParam = (LPARAM)pt;

			m_acklistalarm.InsertItem(&listItem);
			m_acklistalarm.SetItemText(listItem.iItem, 1, LPSTR_TEXTCALLBACK);
			
		}
		/*if(pt == guardian){
		break;
		}
		pt++;
		if(pt == RCT_ALARM_COUNT){
		pt = 0;
		}*/
		pt--;
		if(pt == -1){
			pt = RCT_ALARM_COUNT-1;
		}
		if(pt == guardian){
			break;
		}
	}
	int nCount = m_acklistalarm.GetItemCount();
	if (nCount > 0)
		m_acklistalarm.EnsureVisible(nCount-1, FALSE);

#if 0
	topItem = g_TopAlarmPt;
	if(topItem >= 0){
		m_acklistalarm.SetSelectionMark(topItem);
		m_acklistalarm.SetItem(
			topItem, 
			0, 
			LVIF_STATE, 
			0, 
			0, 
			LVIS_SELECTED, 
			LVIS_SELECTED,
			0
			);
		m_acklistalarm.EnsureVisible(topItem, FALSE);
	}
#endif
__done:
	m_acklistalarm.SetRedraw(TRUE);
}

void CDiaAlarmDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent){
		case 1:
			//UpdateList();
			break;
		case 2:
			if (m_ListUpdate){
				UpdateAckList();
			}
			break;
		default:
			ASSERT(0);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDiaAlarmDlg::OnBnClickedAck()//ȷ�ϵ�������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	selected = m_acklistalarm.GetSelectionMark();
	if(selected < 0) {
		return;
	}
	long pt;
	pt = m_acklistalarm.GetItemData(selected);
	if(pt < 0){
		return;
	}
	CAlarmRecord *pAlarm = g_AlarmRecords + pt;
	//pAlarm->m_ack=true;
	pAlarm->m_valid=false;

	//ȡ�û���
	char strCls[16];
	memset(strCls, ' ', sizeof(strCls));
	char buff[16];
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

	//CString operater = strCls;

	//����վ�䱨��ȷ��ͬ��

	char msg_x[256] = {0};
	memcpy(msg_x, pAlarm->m_msg, sizeof(pAlarm->m_msg));
	strcat(msg_x, strCls);
    sfo->SendFakeAlarm(msg_x);
	///

	int i, topItem, topRank;
	topItem = -1;
	topRank = -1;
	for(i=selected - 1; i >= 0; i--){
		pt = m_acklistalarm.GetItemData(i);
		assert(pt >= 0);
		if(!g_AlarmRecords[pt].m_ack && g_AlarmRecords[pt].m_rank > topRank){
			topRank = g_AlarmRecords[pt].m_rank;
			topItem = i;
		}
	}
	if(topItem >= 0){
		m_acklistalarm.SetSelectionMark(topItem);
		m_acklistalarm.SetItem(
			topItem, 
			0, 
			LVIF_STATE, 
			0, 
			0, 
			LVIS_SELECTED, 
			LVIS_SELECTED,
			0
			);
		m_acklistalarm.EnsureVisible(topItem, FALSE);
	}
	m_acklistalarm.RedrawWindow();
}

void CDiaAlarmDlg::OnBnClickedAckall()//ȷ��ȫ������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAlarmRecord *pAlarm;
	pAlarm = g_AlarmRecords;

	//ȡ�û���
	char strCls[16];
	memset(strCls, ' ', sizeof(strCls));
	char buff[16];
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

	//
	for(int i=0;i<RCT_ALARM_COUNT;i++, pAlarm++)
	{
		//pAlarm->m_ack = true;
		pAlarm->m_valid = false;
		//strcat(pAlarm->m_msg, strCls);
	}


	char m_msg[256] = ";ALL";
	char* msg = m_msg;
	strcat(strCls, msg);
	sfo->SendFakeAlarm(strCls);//����վ��ͨ��
	
	g_TopAlarmPt = -1;

	m_acklistalarm.RedrawWindow();
}

void CDiaAlarmDlg::OnLvnGetdispinfoAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	long pt=pDispInfo->item.lParam;
	bool bSel;
	CArgs args;
	LVITEM * it;
	it = &pDispInfo->item;

	if(it->state & LVIS_SELECTED){
		bSel = true;
	}else{
		bSel = false;
	}
	char pc[6];
	if(bSel){
		sprintf(pc,"%3d", it->iItem);
	}else{
		sprintf(pc,"%5d", it->iItem);
	}

	//���,����ʱ��,������,��������,����ʱ����ֵ,��λ,��������,��ֵ*********�˴���Ҫ��ioss.dll�ڷ��͵ı�����Ϣ�������޸ġ�
	CAlarmRecord * pAlarm = g_AlarmRecords + pt;

	parse_arg_ey(pAlarm->m_msg,&args,";");

	CString temp = args.argv[2];
	if(it->mask & LVIF_TEXT){
		LPTSTR& pszText = pDispInfo->item.pszText;

		switch(pDispInfo->item.iSubItem)
		{
			//���
		case 0: 
			pszText = pc;
			break;
			//ʱ��
		case 1:
			pszText = args.argv [0];
			break;
			//����
		case 2:
			pszText = args.argv [1];
			break;
			//����
		case 3:
			pszText = args.argv [2];
			break;
			//����״̬
		case 4:
			pszText = args.argv [3];
			break;
			//������Ϣ(��/����Ϣ������ģ���������ȼ�)
		case 5:
			pszText = args.argv [4];
			break;
			//����վ����ǰֵ
		case 6:
			pszText = args.argv [5];
			break;
			//����Ա
		case 7:
			pszText = args.argv [6];
			break;
		default:
			ASSERT(0);
		}
	}
	*pResult = 0;
}

void CDiaAlarmDlg::OnNMCustomdrawAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW pDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	if(CDDS_PREPAINT == pDraw->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}else if (CDDS_ITEMPREPAINT == pDraw->nmcd.dwDrawStage)
	{
		long pt=pDraw->nmcd.lItemlParam;
		CAlarmRecord * pAlarm = g_AlarmRecords + pt;
		if(pAlarm->m_ack)
		{
			pDraw->clrText = RGB(198,198,198);//��ɫ
		}
		else if (pAlarm->m_ac==AC_CancelAlarm)
		{
			pDraw->clrText = RGB(70,191,101);//��ɫ��ȡ��״̬�ı���
		}
		/*else
		{*/
		//	if(pAlarm->m_rank==501){
		//		pDraw->clrText = RGB(255,0,255);//���ɫ
		//	}else if(pAlarm->m_rank==502){
		//		pDraw->clrText = RGB(255,0,0);//��ɫ
		//	}else if(pAlarm->m_rank==503){
		//		pDraw->clrText = RGB(255,255,0);//��ɫ
		//	}else if(pAlarm->m_rank==504){
		//		pDraw->clrText = RGB(0,255,0);//��ɫ
		//	}else if(pAlarm->m_rank==505){
		//		pDraw->clrText = RGB(255,255,255);//��
		//	}
		//}
		pDraw->clrTextBk = RGB(196,197,206);
	}
}

void CDiaAlarmDlg::OnNMKillfocusAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ListUpdate = true;
	*pResult = 0;
}

void CDiaAlarmDlg::OnNMSetfocusAckalarmlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_ListUpdate = false; //��ֹˢ��
	*pResult = 0;
}

void CDiaAlarmDlg::OnLvnGetdispinfoListHis(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CArgs args;

	long pt=pDispInfo->item.lParam;
	CPMCEvent *pEvt = &(evtsList[pt]);

	char pc[6];
	sprintf(pc, "%5d", pt + 1);

	parse_arg_ey(pEvt->data, &args, ";");

	CString temp = args.argv[2];
	if(pDispInfo->item.mask & LVIF_TEXT)
	{
		LPTSTR& pszText = pDispInfo->item.pszText;
		switch(pDispInfo->item.iSubItem)
		{
			//���
		case 0: 
			pszText = pc;
			break;
			//ʱ��
		case 1:
			pszText = args.argv [0];
			break;
			//����
		case 2:
			pszText = args.argv [1];
			break;
			//����
		case 3:
			pszText = args.argv [2];
			break;
			//��������/����״̬
		case 4:
			pszText = args.argv [3];
			break;
			//������Ϣ
		case 5:
			pszText = args.argv [4];
			break;
			//��ǰֵ
		case 6:
			pszText = args.argv [5];
			break;
			//����Ա
		case 7:
			pszText = args.argv [6];
			break;
		default:
			ASSERT(0);
		}
	}
	*pResult = 0;
}

void CDiaAlarmDlg::OnNMCustomdrawListHis(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	if(CDDS_PREPAINT == pDraw->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}else if (CDDS_ITEMPREPAINT == pDraw->nmcd.dwDrawStage)
	{
		long pt=pDraw->nmcd.lItemlParam;
		CAlarmRecord * pAlarm = g_AlarmRecords + pt;
		pDraw->clrTextBk = RGB(196,197,206);
	}
}

BOOL CDiaAlarmDlg::PreTranslateMessage(MSG* pMsg)
{
	//TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)    //���λس���ESC  
			return TRUE;     
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4 )  //����ALT+F4
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDiaAlarmDlg::UpdateAckedList()//��ȷ�ϵı����б�
{
	CArgs args;
	int j = 0;
	m_ackedlistalarm.DeleteAllItems();
	for (int i = 0;i < RCT_ALARM_COUNT;i++)
	{
		if (g_AlarmRecords[i].m_ack == true)
		{
			if (g_AlarmRecords[i].m_msg[0] != NULL)
			{
				parse_arg_ey(g_AlarmRecords[i].m_msg, &args, ";");
				m_ackedlistalarm.InsertItem(j, "");
				CString index;
				index.Format("%d", j+1);
				m_ackedlistalarm.SetItemText(j, 0, index);
				m_ackedlistalarm.SetItemText(j, 1, args.argv [0]);//ʱ��
				m_ackedlistalarm.SetItemText(j, 2, args.argv [1]);//����
				m_ackedlistalarm.SetItemText(j, 3, args.argv [2]);//����
				m_ackedlistalarm.SetItemText(j, 4, args.argv [3]);//��������/����״̬
                m_ackedlistalarm.SetItemText(j, 5, args.argv [4]);//������Ϣ
				m_ackedlistalarm.SetItemText(j, 6, args.argv [5]);//����վ����ǰֵ
				m_ackedlistalarm.SetItemText(j, 7, args.argv [6]);//����Ա
				j++;
			}
		}
	}
}
void CDiaAlarmDlg::OnBnClickedSoundOn()//����������ť
{
	// TODO: Add your control notification handler code here
	//�رձ�������������������
	CString str, str1, str2;
	str = CString(get_working_dir()) + "BZAlarm\\BZAlarm";

	//int hRtv = (int)ShellExecute(theApp.m_pMainWnd->m_hWnd, NULL, str, NULL, NULL, SW_SHOWNORMAL);
	/*if(hRtv > 32){
		EndDialog(IDOK);
	}else{
		CString sErr;
		sErr.Format("Can't launch %s, err code:%d.", str, hRtv);
		MessageBox(sErr,NULL,MB_OK | MB_ICONSTOP);
	}*/

	if (pi.dwProcessId == 0)
	{
		CreateProcess(NULL, str.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		m_soundOn.ShowWindow(SW_HIDE);
		m_soundOff.ShowWindow(SW_SHOWNORMAL);
	}

}

void CDiaAlarmDlg::OnBnClickedSoundOff()//�ر�������ť
{
	// TODO: Add your control notification handler code here
	//�رձ�������

	if (pi.dwProcessId != 0)
	{
		m_soundOn.ShowWindow(SW_SHOWNORMAL);
		m_soundOff.ShowWindow(SW_HIDE);

		::TerminateProcess(pi.hProcess,1);  

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		memset(&si, 0, sizeof(si));
		memset(&pi, 0, sizeof(pi));
	}
}

