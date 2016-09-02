// MainFrm.cpp : implementation of the CMainFrame class
//
/*
改动记录
Date       Version    Author       Modify
2012-7-9   1.0.0.4    Zander Sun   1、增加查询模式的设置--CMainFrame::OnOptQuery()、CMainFrame::OnOptScrprv()、CMainFrame::OnOptScrltr()
*/


#include "stdafx.h"
#include "hisq.h"
#include "MainFrm.h"
#include "config.h"
#include "dlgquery.h"
#include "dlgserverip.h"
#include "dlgformat.h"
#include "dlgfmtcurve.h"
#include "dlgcount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4800 4244)

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_CURVE, OnViewCurve)
	ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CURVE, OnUpdateViewCurve)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIST, OnUpdateViewList)
	ON_COMMAND(ID_OPT_QUERY, OnOptQuery)
	ON_COMMAND(ID_OPT_PRINT, OnOptPrint)
	ON_COMMAND(ID_VIEW_FORMAT, OnViewFormat)
	ON_COMMAND(ID_FORMAT_CURVE, OnFormatCurve)
	ON_COMMAND(ID_OPT_OUTFILE, OnOptOutfile)
	ON_COMMAND(ID_OPT_SCRPRV, OnOptScrprv)
	ON_COMMAND(ID_OPT_SCRLTR, OnOptSrcltr)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

extern CMcHisQueryApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pView=NULL;

	archiveCache = createArchiveCache();
	memset(&cacheInfo, 0, sizeof(cacheInfo));
	cacheSetInfo(archiveCache, &cacheInfo);

	m_format="%g";

	m_bShowGrid=true;
	m_iHorGrid=10;
	m_iVerGrid=10;
	m_bAutoZoom=true;
	for(int i=0;i<HISLOG_MAX_QUERY_TAG_COUNT;i++){
		m_fMin[i]=0.;
		m_fMax[i]=100.;
		m_width[i] = 0;
	}
	m_color[0]=RGB(255,0,0);
	m_color[1]=RGB(0,192,0);
	m_color[2]=RGB(0,0,192);
	m_color[3]=RGB(128,0,128);
	m_color[4]=RGB(128,128,0);
	m_color[5]=RGB(0,128,128);
	m_color[6]=RGB(128,0,64);
	m_color[7]=RGB(64,128,0);
	m_color[8]=RGB(0,64,128);
	m_color[9]=RGB(128,64,64);
}

CMainFrame::~CMainFrame()
{
	if(NULL!=m_pView){
		m_pView->DestroyWindow();
		delete m_pView;
		m_pView=NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetIcon(::AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);

	OnViewCurve();
	m_bViewCurve=true;
	
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;		// fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//	be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//	the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnViewCurve() 
{
	if(NULL!=m_pView){
		m_pView->DestroyWindow();
		delete m_pView;
		m_pView=NULL;
	}

	m_pView=new CChildView;
	m_pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
	ASSERT(m_pView);
	RecalcLayout();
	m_bViewCurve=true;
}

void CMainFrame::OnViewList()
{
	if(NULL!=m_pView){
		m_pView->DestroyWindow();
		delete m_pView;
		m_pView=NULL;
	}
	
	m_pView=new CHisList;
	ASSERT(m_pView);
	CHisList *pView=(CHisList*)m_pView;
	pView->Create(WS_CHILD|WS_VISIBLE|LVS_REPORT,
		CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST);

	int i,j;

	LV_COLUMN listColumn;
	TCHAR*	  rgszColumn=_T("时间");
	listColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt = LVCFMT_LEFT;
	listColumn.cx = 200;
	listColumn.iSubItem = 0;
	listColumn.pszText = rgszColumn;
	pView->InsertColumn(0, &listColumn);
	char name[256];
	for(i=1;i<=10;i++){
		listColumn.iSubItem = i;
		sprintf(name, "%s", m_discription[i-1]);
		listColumn.pszText = name;
		listColumn.cx = 80;
		pView->InsertColumn(i, &listColumn);
	}

	LV_ITEM listItem;
	listItem.mask = LVIF_TEXT|LVIF_PARAM;
	for(i=0;i<600;i++){
		listItem.iSubItem = 0;
		listItem.iItem = pView->GetItemCount();
		listItem.pszText = LPSTR_TEXTCALLBACK;
		listItem.cchTextMax = 32;
		listItem.lParam = (LPARAM)i;
		pView->InsertItem(&listItem);
		for(j=1;j<=HISLOG_MAX_QUERY_TAG_COUNT;j++){    
			pView->SetItemText(listItem.iItem,j,LPSTR_TEXTCALLBACK);
		}
	}

	RecalcLayout();
	m_bViewCurve=false;
}

void CMainFrame::OnUpdateViewCurve(CCmdUI* pCmdUI) 
{
	if(m_bViewCurve) 
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateViewList(CCmdUI* pCmdUI) 
{
	if(!m_bViewCurve) 
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}


void CMainFrame::OnOptQuery() 
{
	CDlgQuery dlg;
	SYSTEMTIME stm;
	int i;

#define gg(i)\
	dlg.m_tag##i = m_names[i-1];\
	dlg.m_dis##i = m_discription[i-1];\

	gg(1)
	gg(2)
	gg(3)
	gg(4)
	gg(5)
	gg(6)
	gg(7)
	gg(8)
	gg(9)
	gg(10)

	if(!cacheInfo.startTime){
		cacheInfo.startTime = hislogGetCurrentTime();
		cacheInfo.step = 1;
		cacheInfo.valueCount=600;
	}
	FILETIME ftm;
	ftm = hislogTimeByKey(cacheInfo.startTime);
	FileTimeToSystemTime(&ftm, &stm);
	dlg.m_date.SetDate(stm.wYear,stm.wMonth,stm.wDay);
	dlg.m_time.SetTime(stm.wHour,stm.wMinute,stm.wSecond);
	CTime tm(ftm);
	CTimeSpan ts(cacheInfo.step*cacheInfo.valueCount);
	tm += ts;
	struct tm real_osTime;
	struct tm* osTime = &real_osTime;
	// 2013-5-22
	/*osTime = tm.GetGmtTm( NULL );*/
	osTime = tm.GetGmtTm( osTime );
	dlg.m_dateEnd.SetDate(osTime->tm_year+1900, osTime->tm_mon+1, osTime->tm_mday);
	dlg.m_timeEnd.SetTime(osTime->tm_hour, osTime->tm_min, osTime->tm_sec);

	if(dlg.DoModal()==IDOK){
		FILETIME ftm1,ftm2;
		stm.wYear=dlg.m_date.GetYear();
		stm.wMonth=dlg.m_date.GetMonth();
		stm.wDay=dlg.m_date.GetDay();
		stm.wHour=dlg.m_time.GetHour();
		stm.wMinute=dlg.m_time.GetMinute();
		stm.wSecond=dlg.m_time.GetSecond();
		stm.wMilliseconds=0;
		SystemTimeToFileTime(&stm, &ftm1);
		cacheInfo.startTime = hislogKeyByTime(&ftm1);

		stm.wYear=dlg.m_dateEnd.GetYear();
		stm.wMonth=dlg.m_dateEnd.GetMonth();
		stm.wDay=dlg.m_dateEnd.GetDay();
		stm.wHour=dlg.m_timeEnd.GetHour();
		stm.wMinute=dlg.m_timeEnd.GetMinute();
		stm.wSecond=dlg.m_timeEnd.GetSecond();
		stm.wMilliseconds=0;
		SystemTimeToFileTime(&stm, &ftm2);
		
		CTime tm1(ftm1), tm2(ftm2);
		ts = tm2-tm1;
		int sec = ts.GetTotalSeconds();
		if( sec< 600){
			cacheInfo.step = 1;
		}else{
			cacheInfo.step = sec / 600;			
		}

#define hh(i)\
		m_discription[i-1]=dlg.m_dis##i;\
		if( !dlg.m_tag##i.IsEmpty() ){\
			tagname_by_text(names + i - 1, (LPCTSTR)dlg.m_tag##i);\
			m_names[i-1]=dlg.m_tag##i;\
		}else{\
			m_names[i-1]="";\
		}

		hh(1)
		hh(2)
		hh(3)
		hh(4)
		hh(5)
		hh(6)
		hh(7)
		hh(8)
		hh(9)
		hh(10)

		cacheInfo.names = names;
		cacheInfo.tagCount = 10;
		cacheSetMode(archiveCache, PT_QueryMode_BYCOUNT);	//2012-7-9
		cacheSetNames(archiveCache, cacheInfo.names, cacheInfo.tagCount);
		cacheSetTimeSpan(archiveCache, cacheInfo.startTime, cacheInfo.step, 600);
		cacheQuery(archiveCache, 1000, 0);

		if(!m_bViewCurve){
			LV_COLUMN listColumn;
			TCHAR*	  rgszColumn=_T("时间");
			listColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
			listColumn.fmt = LVCFMT_LEFT;
			listColumn.cx = 200;
			listColumn.iSubItem = 0;
			listColumn.pszText = rgszColumn;
			((CHisList*)m_pView)->InsertColumn(0, &listColumn);
			char name[256];
			for(i=1;i<=HISLOG_MAX_QUERY_TAG_COUNT;i++){
				listColumn.iSubItem = i;
				if((i-1)<cacheInfo.tagCount){
					sprintf(name, "%s", m_discription[i-1]);
					listColumn.pszText = name;
				}else{
					listColumn.pszText = "";
				}
				listColumn.cx = 80;
				((CHisList*)m_pView)->SetColumn(i, &listColumn);
			}
		}

		m_pView->RedrawWindow();
	}
}

inline void GetItem(CString &str,int line,int col)
{
	CMainFrame* pmf=(CMainFrame*)::AfxGetMainWnd();
	if(pmf->m_names[col].IsEmpty()){
		str.Format("");
		return;
	}

	CString fmt=((CMainFrame*)::AfxGetMainWnd())->m_format;
	
	PCpmc_value_t value;
	value = cacheGetValue(archiveCache, col, line);
	if(!(value->Flags & TF_Valid)){
		str.Format(".");		
	}else{
		switch(get_value_type(value->Flags)){
		case dt_real4:
			str.Format(fmt,value->Value.fltValue);		
			break;
		case dt_bool:
			if(0==value->Value.b){
				str.Format("0");		
			}else{
				str.Format("1");
			}
			break;
		case dt_int32:
			str.Format("%d",value->Value.iValue);		
			break;
		case dt_real8:
			str.Format(fmt,value->Value.dblValue);		
			break;
		default:
			str.Format(".");		
		}
	}
}

void CMainFrame::OnOptOutfile() 
{
	int iPrtCount;

	CDlgCount dlgCount;
	dlgCount.m_count=600;
	if(dlgCount.DoModal() == IDOK){
		iPrtCount = dlgCount.m_count;
	}else{
		return;
	}

	CFileDialog dlg(false, "csv", "", OFN_OVERWRITEPROMPT, "csv Files(*.csv)|*.csv||");
	if(dlg.DoModal()==IDOK){
		CString strFile = dlg.GetPathName();
		FILE *pFile = fopen(strFile, "w+");
		if(pFile){
			int i, j;
			fprintf(pFile, "时间,");
			for(j = 0; j < cacheInfo.tagCount; j++){
				fprintf(pFile, "%s,", m_names[j]);
			}
			fprintf(pFile, "\n");
			fprintf(pFile, ",");
			for(j = 0; j < cacheInfo.tagCount; j++){
				fprintf(pFile, "%s,", m_discription[j]);
			}
			fprintf(pFile, "\n");

			for(i = 0; i < iPrtCount; i++){
				// get a line and output it
				CString str;
				SYSTEMTIME stm;
				FILETIME ftm;
				ftm=hislogTimeByKey(cacheInfo.startTime + cacheInfo.step * i);
				FileTimeToSystemTime(&ftm, &stm);
				str.Format("%4d年%2d月%2d日 %2d:%2d:%2d",
					stm.wYear, stm.wMonth, stm.wDay, stm.wHour, stm.wMinute, stm.wSecond);
				fprintf(pFile, "%s,", str);

				for(j=0;j<cacheInfo.tagCount;j++){
					GetItem(str, i, j);
					fprintf(pFile, "%s,", str);
				}
				fprintf(pFile, "\n");
			}
			fclose(pFile);
		}
	}
}

void CMainFrame::OnOptPrint() 
{
	CPrintDialog dlg(FALSE);
	if(IDOK!=dlg.DoModal()) return;;
	//dlg.GetDefaults();
	
	// is a default printer set up?
	HDC hdcPrinter = dlg.GetPrinterDC();
	if (hdcPrinter == NULL){
		MessageBox("无法找到打印机",NULL,MB_OK|MB_ICONSTOP);
		return;
	}
	
	// create a CDC and attach it to the default printer
	CDC dcPrinter;
	dcPrinter.Attach(hdcPrinter);
	
	// call StartDoc() to begin printing
	DOCINFO docinfo;
	memset(&docinfo, 0, sizeof(docinfo));
	docinfo.cbSize = sizeof(docinfo);
	docinfo.lpszDocName = "历史数据";
	//begin print
	if (dcPrinter.StartDoc(&docinfo) < 0){
		MessageBox("无法初始化打印机",NULL,MB_OK|MB_ICONSTOP);
		return;
	}

	if(m_bViewCurve){
		//print curve
		if (dcPrinter.StartPage() < 0){
			MessageBox("无法初始化页",NULL,MB_OK|MB_ICONSTOP);
			dcPrinter.AbortDoc();
		}else{
			((CChildView*)m_pView)->DrawCurve(&dcPrinter,true);
			dcPrinter.EndPage();
			dcPrinter.EndDoc();
		}
	}else{
		CDlgCount dlgCount;
		dlgCount.m_count=600;
		if(dlgCount.DoModal()!=IDOK) return;
		int iPrtCount=dlgCount.m_count;
		//print table
		dcPrinter.SetMapMode(MM_TWIPS);
		CFont *pOldFont;
		CFont fnt;
		if(fnt.CreatePointFont(100,"宋体",&dcPrinter)){
			pOldFont=(CFont*)dcPrinter.SelectObject(&fnt);					
		}else{
			pOldFont=(CFont*)dcPrinter.SelectStockObject(DEVICE_DEFAULT_FONT);
		}
		CPoint pt(dcPrinter.GetDeviceCaps(HORZRES),dcPrinter.GetDeviceCaps(VERTRES));
		dcPrinter.DPtoLP (&pt);
		pt.y=-pt.y;
		CSize sz=dcPrinter.GetTextExtent("0123456789  ");
		CSize szt=dcPrinter.GetTextExtent("yyyy年mm月dd日 hh:nn:ss	  ");
		int lineHeight=sz.cy*1.5;
		int lineCount=(pt.y-sz.cy*13)/lineHeight;
		if(lineCount<0) return;
		CString str;
		int cLeftGap=800;
		long id=0;
		int i,j,line=0,iPage=0;
		while(line<iPrtCount){
			// start a page
			if (dcPrinter.StartPage() < 0){
				MessageBox("无法初始化页",NULL,MB_OK|MB_ICONSTOP);
				dcPrinter.AbortDoc();
			}else{
				// actually do some printing
				//print title
				iPage++;
				str.Format("历史数据报表 第%2d页",iPage);
				dcPrinter.TextOut(cLeftGap,-lineHeight*3,str);
				str.Format("时间");
				dcPrinter.TextOut(cLeftGap,-lineHeight*5,str);
				for(j=0;j<cacheInfo.tagCount;j++){
					dcPrinter.TextOut(cLeftGap+szt.cx+sz.cx*j,-lineHeight*5,m_discription[j]);
				}
				for(i=0;i<lineCount;i++){
					if(line<iPrtCount){
						//get a line and print it
						SYSTEMTIME stm;
						FILETIME ftm;

						ftm=hislogTimeByKey(cacheInfo.startTime + cacheInfo.step * line);
						FileTimeToSystemTime(&ftm,&stm);
						str.Format("%4d年%2d月%2d日 %2d:%2d:%2d",
							stm.wYear,stm.wMonth,stm.wDay,stm.wHour,stm.wMinute,stm.wSecond);
						dcPrinter.TextOut(cLeftGap,-lineHeight*(6+i),str);
						for(j=0;j<cacheInfo.tagCount;j++){
							GetItem(str,line,j);
							dcPrinter.TextOut(cLeftGap+szt.cx+sz.cx*j,-lineHeight*(6+i),str);
						}
						line++;
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

void CMainFrame::OnViewFormat() 
{
	CDlgFormat dlg;
	dlg.m_format=m_format;
	if(dlg.DoModal()==IDOK){
		m_format=dlg.m_format;
		m_pView->RedrawWindow();
	}
}

void CMainFrame::OnFormatCurve() 
{
	CDlgFmtCurve dlg;
	dlg.m_bShowGrid=m_bShowGrid;
	dlg.m_iHorGrid=m_iHorGrid;
	dlg.m_iVerGrid=m_iVerGrid;
#define ff(i)\
	dlg.m_min##i=m_fMin[i-1];\
	dlg.m_max##i=m_fMax[i-1];\
	dlg.m_w##i=m_width[i-1];\

	ff(1)
	ff(2)
	ff(3)
	ff(4)
	ff(5)
	ff(6)
	ff(7)
	ff(8)
	ff(9)
	ff(10)

	int i;
	for(i=0;i<HISLOG_MAX_QUERY_TAG_COUNT;i++){
		dlg.m_color[i]=m_color[i];
	}
	if(dlg.DoModal()==IDOK){
		m_bShowGrid=dlg.m_bShowGrid;
		m_iHorGrid=dlg.m_iHorGrid;
		m_iVerGrid=dlg.m_iVerGrid;
#define ee(i)\
	m_fMin[i-1]=dlg.m_min##i;\
	m_fMax[i-1]=dlg.m_max##i;\
	m_width[i-1]=dlg.m_w##i;\

		ee(1);
		ee(2);
		ee(3);
		ee(4);
		ee(5);
		ee(6);
		ee(7);
		ee(8);
		ee(9);
		ee(10);
		for(i=0;i<HISLOG_MAX_QUERY_TAG_COUNT;i++){
			m_color[i]=dlg.m_color[i];
		}
		if(m_bViewCurve){
			if(NULL!=m_pView) m_pView->RedrawWindow();
		}
	}
}

void CMainFrame::OnOptScrprv() 
{
	cacheInfo.startTime -= cacheInfo.step * 600;
	cacheSetMode(archiveCache, PT_QueryMode_BYCOUNT);	//2012-7-9
	cacheSetNames(archiveCache, cacheInfo.names, cacheInfo.tagCount);
	cacheSetTimeSpan(archiveCache, cacheInfo.startTime, cacheInfo.step, 600);
	cacheQuery(archiveCache, 1000, 0);
	m_pView->RedrawWindow();
}

void CMainFrame::OnOptSrcltr() 
{
	cacheInfo.startTime += cacheInfo.step * 600;
	cacheSetMode(archiveCache, PT_QueryMode_BYCOUNT);	//2012-7-9
	cacheSetNames(archiveCache, cacheInfo.names, cacheInfo.tagCount);
	cacheSetTimeSpan(archiveCache, cacheInfo.startTime, cacheInfo.step, 600);
	cacheQuery(archiveCache, 1000, 0);
	m_pView->RedrawWindow();
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_bViewCurve){
		bool  b_redraw = true;
		CChildView *pv = (CChildView*)m_pView;
		if(pv->pos_x >= 0 && pv->pos_x <= 600){
			switch(nChar){
			case 37:
				pv->pos_x -= 1;
				break;
			case 39:
				pv->pos_x += 1;
				break;
			case 38:
				pv->pos_x += 10;
				break;
			case 40:
				pv->pos_x -= 10;
				break;
			default:
				b_redraw = false;
				break;
			}
			if(pv->pos_x > 600){
				pv->pos_x = 600;
			}
			if(pv->pos_x < 0){
				pv->pos_x = 0;
			}
			if(b_redraw){
				pv->RedrawWindow();
			}
		}
	}

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

//2015.06.18  GM
BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBrush backBrush(RGB(255,128,128));
	CBrush	*pOldBrush=pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//
	pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
	//pDC->SelectObject(pOldBrush);	
	return TRUE;
	//return CFrameWnd::OnEraseBkgnd(pDC);
}
