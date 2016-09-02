// FView.cpp : implementation file
/*
Date       Version  Modified by  Modify
2014-8-22  1.0.0.3      mox      1.添加类class CDelayWriter_x，用来走标签点只置0，不记录的接口
                                 2.在类class CDelayWriter_x中添加timerProc_x()
*/
#include "stdafx.h"
#include "mcview.h"
#include "FView.h"

#include "common.h"
#include "Element.h"
#include "Line.h"
#include "Arc.h"
#include "PolyLine.h"
#include "Rectangle.h"
#include "RoundRect.h"
#include "Ellipse.h"
#include "Polygon.h"
#include "Text.h"
#include "Button.h"
#include "Meta.h"
#include "Cell.h"
#include "Trend.h"

#include "Mcview.h"
#include <winuser.h>
#include <windows.h>

#include "dlgdnytip.h"
#include "diainputd.h"
#include "diainputa.h"
#include "dlgtrendtime.h"
#include "DiaUserManager.h"

#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#pragma warning(disable:4800)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern long glViewWidth,glViewHeight;
extern CMcviewApp theApp;
extern CFView theView;
extern CDiaInputD gDiaInputD;
extern CDiaInputA gDiaInputA;
bool gbIsPrint=false;
extern char * userinfoPath;
extern CString m_sCurUser;


/////////////////////////////////////////////////////////////////////////////
// CFView dialog

#define WM_REFRESH_MESSAGE WM_USER+105 //自定义消息，用来通知refresh任务栏布局

CFView::CFView(CWnd* pParent /*=NULL*/)
	: CDialog(CFView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sCaption="";
	m_HaveElements=false;
	m_pSelElement=NULL;
	m_lTagCount=0;
	m_bPopup=false;
	m_pTagNames=NULL;
	m_pTagValues=NULL;
	m_brushWhite.CreateSolidBrush(RGB(255,255,255));
	m_lTimeOut=0;
	m_bPulse = false;
}

CFView::~CFView()
{
	cleanView();
}

void CFView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFView, CDialog)
	//{{AFX_MSG_MAP(CFView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCLBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFView message handlers

void CFView::cleanView()
{
	//清除原有内容
	POSITION pos;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		delete m_Elements.GetNext(pos);
	}
	m_Elements.RemoveAll();
	for(pos=m_TdElements.GetHeadPosition();pos!=NULL;){
		delete m_TdElements.GetNext(pos);
	}	 
	m_TdElements.RemoveAll();
	for(pos=m_BkElements.GetHeadPosition();pos!=NULL;){
		delete m_BkElements.GetNext(pos);
	}	 
	m_BkElements.RemoveAll();
	if(m_lTagCount>0){
		delete[] m_pTagNames;
		delete[] m_pTagValues;
	}
	m_pTagNames=NULL;
	m_pTagValues=NULL;
	m_lTagCount=0;
	
	m_HaveElements=false;
	m_pSelElement=NULL;
	m_sFileName = "";
}

static __uint _reload_tags(__uint count, PCTAG_NAME names, __uint timeout)
{
	PTAG_NAME tn;
	__uint i;
	__uint count1;
	if(!lock_rtdb(__false, 500)){
		return 0;
	}
	tn = new TAG_NAME[count];
	count1=0;
	for(i=0; i<count /* &&  count1< 100 */; i++,names++){
		if(!query_tag_f(names)){
			tn[count1++] = *names;
		}
	}
	count1 = reload_tags(count1, tn, timeout);
	unlock_rtdb();
	delete tn;
	return count1;
}

COLORREF g_WindowColor;
CDC * currentStoredDc;

void CFView::LoadFile(CString sFileName,int x,int y)
{
	cleanView();

	m_sFileName=sFileName;

	CDC *pDC=GetDC();

	MSXML::IXMLDOMDocumentPtr pDoc;	
	//::CoInitialize(NULL);
	HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr)){
		CPen *pOldPen=(CPen*)pDC->SelectStockObject(NULL_PEN);
		CBrush *pOldBrush=(CBrush*)pDC->SelectStockObject(LTGRAY_BRUSH);
		pDC->Rectangle(0,0,glViewWidth,glViewHeight);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		
		if(m_bPopup){
			SetWindowText("错误");
			SetDlgItemText(IDC_ERROR,"can't create DOMDocument object!");
			GetDlgItem(IDC_ERROR)->ShowWindow(SW_SHOW);
		}else{
			CGdiObject *pFont=pDC->SelectStockObject(SYSTEM_FONT);
			pDC->SetTextColor(0x000000ff);
			pDC->TextOut(0,0,L"can't create DOMDocument object!");
			pDC->TextOut(0,0,sFileName);
			pDC->SelectObject(pFont);
		}

		ReleaseDC(pDC);
		return;
	}
	
	sFileName = ((CMcviewApp*)::AfxGetApp())->m_path+"file\\"+sFileName;
	if(pDoc->load(sFileName.AllocSysString())==FALSE){
		CPen *pOldPen=(CPen*)pDC->SelectStockObject(NULL_PEN);
		CBrush *pOldBrush=(CBrush*)pDC->SelectStockObject(LTGRAY_BRUSH);
		pDC->Rectangle(0,0,glViewWidth,glViewHeight);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		if(m_bPopup){
			sFileName="文件"+sFileName+"不存在 或 文件格式无法识别";
			SetWindowText("错误");
			SetDlgItemText(IDC_ERROR,sFileName); 
			GetDlgItem(IDC_ERROR)->ShowWindow(SW_SHOW);
		}else{
			sFileName="错误：文件"+sFileName+"不存在 或 文件格式无法识别";
			CGdiObject *pFont=pDC->SelectStockObject(SYSTEM_FONT);
			pDC->SetTextColor(0x000000ff);
			pDC->TextOut(0,0,sFileName);
			pDC->SelectObject(pFont);
		}

		ReleaseDC(pDC);
		return;
	}

	_bstr_t strFile=L"hmif";
	MSXML::IXMLDOMElementPtr pRoot=pDoc->documentElement;
	if(pRoot->baseName!=strFile){
		CPen *pOldPen=(CPen*)pDC->SelectStockObject(NULL_PEN);
		CBrush *pOldBrush=(CBrush*)pDC->SelectStockObject(LTGRAY_BRUSH);
		pDC->Rectangle(0,0,glViewWidth,glViewHeight);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

		if(m_bPopup){
			sFileName=sFileName+"文件格式无法识别";
			SetWindowText("错误");
			SetDlgItemText(IDC_ERROR,sFileName); 
			GetDlgItem(IDC_ERROR)->ShowWindow(SW_SHOW);
		}else{
			sFileName="错误："+sFileName+"文件格式无法识别";
			CGdiObject *pFont=pDC->SelectStockObject(SYSTEM_FONT);
			pDC->SetTextColor(0x000000ff);			
			pDC->TextOut(0,0,sFileName);
			pDC->SelectObject(pFont);
		}

		ReleaseDC(pDC);
		return;
	}
	
	//装载数据
	_bstr_t strGO=L"go";
	_variant_t strArc=L"arc";
	_variant_t strCell=L"cell";
	_variant_t strEllipse=L"ellipse";
	_variant_t strLine=L"line";
	_variant_t strMCButton=L"mcbutton";
	_variant_t strMeta=L"meta";
	_variant_t strPolygon=L"polygon";
	_variant_t strPolyLine="polyline";
	_variant_t strRectangle=L"rectangle";
	_variant_t strRoundRect=L"roundrect";
	_variant_t strText=L"text";
	_variant_t strTrend=L"trend";

	_variant_t val;
	//load window attribute
	val=pRoot->getAttribute(L"c");
	val.ChangeType(VT_UI4);
	m_WindowColor=val.ulVal;
	g_WindowColor=m_WindowColor;
	val=pRoot->getAttribute(L"w");
	val.ChangeType(VT_UI4);
	m_lWndWidth=val.ulVal;
	val=pRoot->getAttribute(L"h");
	val.ChangeType(VT_UI4);
	m_lWndHeight=val.ulVal;
	val=pRoot->getAttribute(L"cap");
	m_sCaption=val.bstrVal;
	CElement::UPtoLP(m_lWndWidth,m_lWndHeight,&m_lWndWidth,&m_lWndHeight);

	if(m_bPopup){
		SetWindowText(m_sCaption);
		if(!dcMem.CreateCompatibleDC(pDC)){
			char err[128];
			sprintf(err, "CreateCompatibleDC() 1 Faild, error %d!", GetLastError());
			SetWindowText("错误");
			SetDlgItemText(IDC_ERROR, err); 
			GetDlgItem(IDC_ERROR)->ShowWindow(SW_SHOW);
			ReleaseDC(pDC);
			return;
		}
		if(!dcBkMem.CreateCompatibleDC(pDC)){
			SetWindowText("错误");
			SetDlgItemText(IDC_ERROR,"CreateCompatibleDC() 2 Faild!"); 
			GetDlgItem(IDC_ERROR)->ShowWindow(SW_SHOW);
			ReleaseDC(pDC);
			return;
		}
		POINT ptSize;
		ptSize.x=m_lWndWidth;
		ptSize.y=m_lWndHeight;
		pDC->LPtoDP(&ptSize);
		if(!memBitmap.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y)){
			SetWindowText("错误");
			SetDlgItemText(IDC_ERROR,"CreateCompatibleBitmap() 1 Faild!"); 
			GetDlgItem(IDC_ERROR)->ShowWindow(SW_SHOW);
			ReleaseDC(pDC);
			return;
		}
		if(!memBkBitmap.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y)){
			SetWindowText("错误");
			SetDlgItemText(IDC_ERROR,"CreateCompatibleBitmap() 2 Faild!"); 
			GetDlgItem(IDC_ERROR)->ShowWindow(SW_SHOW);
			ReleaseDC(pDC);
			return;
		}
		dcMem.SelectObject(&memBitmap);
		dcMem.SetBkMode(TRANSPARENT);
		dcBkMem.SelectObject(&memBkBitmap);
		dcBkMem.SetBkMode(TRANSPARENT);
		
		CRect rw, rc ,rd;
		GetWindowRect(rw);
		GetClientRect(rc);
		rd = rw;
		rd -= rc;
		
		SIZE sz;
		sz.cx = rd.Width();
		sz.cy = rd.Height();
		
		MoveWindow(x, y, ptSize.x + rd.Width(), ptSize.y + rd.Height());
	}

	MSXML::IXMLDOMElementPtr pItem;
	CElement * pElement;
	for(pItem=pRoot->firstChild;pItem!=NULL;pItem=pItem->nextSibling){
		if(pItem->baseName==strGO){
			val=pItem->getAttribute(L"type");
			if(val==strRectangle){
				pElement=new CRectangle();
				((CRectangle*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strArc){
				pElement=new CArc();
				((CArc*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strEllipse){
				pElement=new CEllipse();
				((CEllipse*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strLine){///line+++
				pElement=new CLine();
				((CLine*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strMCButton){
				pElement=new CMCButton();
				((CMCButton*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strMeta){
				pElement=new CMeta();
				((CMeta*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strRoundRect){
				pElement=new CRoundRect();
				((CRoundRect*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strText){
				pElement=new CText();
				((CText*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strPolygon){
				pElement=new CPolygon();
				((CPolygon*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strPolyLine){////polyline+++
				pElement=new CPolyLine();
				((CPolyLine*)pElement)->Load(pItem);
				if(pElement->m_DnyAttr.m_Linked)
					m_Elements.AddTail(pElement);
				else
					m_BkElements.AddTail(pElement);
			}else if(val==strCell){
				pElement=new CCell();
				((CCell*)pElement)->Load(pItem);
				m_Elements.AddTail(pElement);
			}else if(val==strTrend){
				pElement=new CTrend();
				((CTrend*)pElement)->Load(pItem);
				m_TdElements.AddTail(pElement);
			}
		}
	}

	//mox+++查找存储用户信息的ini文件是否存在，如果不存在则创建
    CFileFind finder;
    BOOL ifFind = finder.FindFile(userinfoPath);
	/*if(!ifFind)
	{

	}*/

	///

	//将静态元素画在背景图里
	//画背景色
	CPen *pOldPen;
	pOldPen=(CPen *)dcBkMem.SelectStockObject(NULL_PEN);
	CBrush brushBack(m_WindowColor);
	CBrush *pOldBrush;
	POSITION pos;
	
	pOldBrush=(CBrush *)dcBkMem.SelectObject(&brushBack); 
	dcBkMem.Rectangle(0,0,glViewWidth,glViewHeight);
	dcBkMem.SelectStockObject(WHITE_BRUSH);
	if(m_lWndWidth<glViewWidth){
		dcBkMem.Rectangle(m_lWndWidth,0,glViewWidth,glViewHeight);
	}
	if(m_lWndHeight<glViewHeight){
		dcBkMem.Rectangle(0,m_lWndHeight,glViewWidth,glViewHeight);
	}
	dcBkMem.SelectObject(pOldPen);
	dcBkMem.SelectObject(pOldBrush);
	dcBkMem.SetROP2(R2_COPYPEN);
	for(pos=m_BkElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_BkElements.GetNext(pos)));
		pElement->Draw(&dcBkMem);
	}
	for(pos=m_TdElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_TdElements.GetNext(pos)));
		pElement->Draw(&dcBkMem);
	}

	ReleaseDC(pDC);

	//get dynamic tags
	m_lTagCount=GetDynTags(NULL,true);
	utils_debug("%d tags attached.\n", m_lTagCount);
	if(m_lTagCount>0){
		m_pTagNames=new TAG_NAME[m_lTagCount];
		m_pTagValues=new pmc_value_t[m_lTagCount];
		GetDynTags(m_pTagNames,false);
		// 2003/5/14, tags are dynamicly loaded by rtkproxy
		utils_debug(
			"%d of %d tags reloaded.\n", 
			_reload_tags(m_lTagCount, m_pTagNames, 0),
			m_lTagCount
			);
		// proxy_read_tags(m_lTagCount, m_pTagNames, m_pTagValues, 0);
	}

	m_HaveElements=true;
	m_prvShow=PRV_SHOW;

	utils_debug(
		"file %s loaded, %d tags, %d elements(%d+%d+%d)\n",
		(LPCSTR)sFileName,
		m_lTagCount,
		m_BkElements.GetCount()+m_TdElements.GetCount()+m_Elements.GetCount(),
		m_Elements.GetCount(),
		m_BkElements.GetCount(),
		m_TdElements.GetCount()
		);
}

void CFView::Show()
{
	if(!m_HaveElements) return;
	
	if(m_lTagCount>0){
		proxy_read_tags(m_lTagCount, m_pTagNames, m_pTagValues, 0);
#if 0
		{
			for(int i=0; i<m_lTagCount; i++){
				utils_debug(
					"%s.%s.%s=", 
					(char*)CNodeName(m_pTagNames[i].node),
					(char*)CGroupName(m_pTagNames[i].sname.group),
					(char*)CTagName(m_pTagNames[i].sname.tag)
					);
				PRTK_TAG t = query_tag_f(&m_pTagNames[i]);
				if(!t){
					utils_debug("N.B.\n");
				}else if(!(m_pTagValues[i].Flags & TF_Valid)){
					utils_debug("N.A.\n");
				}else{
					char * v = pmc_value_string(&m_pTagValues[i]);
					utils_debug("%s\n", v);
					free_buffer(v);
				}
			}
		}
#endif
#if 0
		{
			for(int i=0; i<m_lTagCount; i++){
				PRTK_TAG t = query_tag_f(&m_pTagNames[i]);
				if(!t){
					continue;
				}
				if((m_pTagValues[i].Flags & TF_Expired) && (m_pTagValues[i].Flags & TF_Valid)){
				}else{
					continue;
				}
				// valid but expired tags found
				utils_debug(
					"BUG030624001 detected, %s.%s.%s,", 
					(char*)CNodeName(m_pTagNames[i].node),
					(char*)CGroupName(m_pTagNames[i].sname.group),
					(char*)CTagName(m_pTagNames[i].sname.tag)
					);
				if(!t){
					continue;
				}
				SYSTEMTIME st;
				FileTimeToSystemTime((LPFILETIME)&t->d.CachedTime, &st);
				utils_debug(
					"Cached : %d-%d-%d %d:%d:%d:%d,",
					st.wYear,
					st.wMonth,
					st.wDay,
					st.wHour,
					st.wMinute,
					st.wSecond,
					st.wMilliseconds
					);
				GetSystemTime(&st);
				utils_debug(
					"Cached : %d-%d-%d %d:%d:%d:%d,",
					st.wYear,
					st.wMonth,
					st.wDay,
					st.wHour,
					st.wMinute,
					st.wSecond,
					st.wMilliseconds
					);
				utils_debug("Flags=%08x\n", t->d.Flags);
			}
		}
#endif
		SetDynValues(m_pTagValues);
	}

	// 考虑到元素的动态连接只有调用窗口的情况
	// 需要调用这个函数决定是否有权限
	UpdateElements();

	CDC * pDC=GetDC();
	currentStoredDc=pDC;

	//拷贝背景
	dcMem.BitBlt(0,0,glViewWidth,glViewHeight,&dcBkMem,0,0,SRCCOPY);

	POSITION pos;
	CElement *pElement;
	if(m_prvShow>0){
		m_prvShow--;
		//画有效的元件
		for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(m_Elements.GetNext(pos)));
			if(!pElement->m_DnyAttr.m_na) pElement->Draw(&dcMem);
		}
	}else{
		//画所有元件
		for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
			pElement=((CElement*)(m_Elements.GetNext(pos)));
			pElement->Draw(&dcMem);
		}
	}
	for(pos=m_TdElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_TdElements.GetNext(pos)));
		((CTrend*)pElement)->DrawCurve(&dcMem);
	}

	pDC->BitBlt(0,0,glViewWidth,glViewHeight,&dcMem,0,0,SRCCOPY);
	ReleaseDC(pDC);

	m_lTimeOut=(m_lTimeOut+1)%300;
	if(!m_lTimeOut){
		//
	}
}

int CFView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if(m_bPopup) return 0;

	CDC *pDC=GetDC();

	if(!dcMem.CreateCompatibleDC(pDC)){
		ReleaseDC(pDC);
		return -1;
	}
	if(!dcBkMem.CreateCompatibleDC(pDC)){
		ReleaseDC(pDC);
		return -1;
	}

	POINT ptSize;
	ptSize.x=glViewWidth;
	ptSize.y=glViewHeight;
	pDC->LPtoDP(&ptSize);
	if(!memBitmap.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y)){
		ReleaseDC(pDC);
		return -1;
	}
	if(!memBkBitmap.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y)){
		ReleaseDC(pDC);
		return -1;
	}

	dcMem.SelectObject(&memBitmap);
	dcMem.SetBkMode(TRANSPARENT);
	dcBkMem.SelectObject(&memBkBitmap);
	dcBkMem.SetBkMode(TRANSPARENT);

	ReleaseDC(pDC);

	return 0;
}



void CFView::OnOK() 
{
	//do nothing	
	//CDialog::OnOK();
}

void CFView::OnCancel() 
{
	if(m_bPopup){
		m_pCtrDlg->RemovePopView(m_pos);
		DestroyWindow();
		delete this;
	}else{
		//do nothing
	}
}

/*
	gather tag usage data from tree-structure representation
	of the configured file.

	bCountOnly indicates what? dont' know, seems weird

	jht, 2005/3/15
*/
long CFView::GetDynTags(TAG_NAME *pFatherTags,bool bCountOnly,long lFatherCount)
{
	TAG_NAME *pTags;
	long lCount=lFatherCount;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount=((CCell*)pElement)->GetDynTags(pFatherTags,bCountOnly,lCount);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//button wnd
			//pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonWnd));
			//if(pDny->m_Used){
			//	if(!bCountOnly){
			//		tagname_by_text(pTags,pDny->m_TagName.GetBuffer(TAGNAME_TEXT_LENGTH));
			//		pDny->m_TagName.ReleaseBuffer();
			//		pTags++;
			//	}
			//	lCount++;
			//}
			//color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used){
				if( ((LPCTSTR)pDny->m_TagName)[0] == '@' ){
					
					// 20050315, we needs some way of scripting
					// this is a crude implementation
					pDny->action_proc = (void *)GetProcAddress(
						g_hScriptDlll, 
						(LPCTSTR)pDny->m_TagName + 1
						);
					if(!pDny->action_proc){
						// a flag for quick detection of improperly configured item
						pDny->action_proc = (void *)-1;
						pDny->m_TagValue.Flags = 0;
					}else{
						pDny->m_TagValue.Flags |= TF_Valid;
					}
				}else {
					if(!bCountOnly){
						pTags=pFatherTags+lCount;
						tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
						pDny->action_proc = 0;
					}
					lCount++;
				}
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used){
				if( ((LPCTSTR)pDny->m_TagName)[0] == '@' ){
					// 20050315, we needs some way of scripting
					// this is a crude implementation
					pDny->action_proc = (void *)GetProcAddress(
						g_hScriptDlll, 
						(LPCTSTR)pDny->m_TagName + 1
						);
					if(!pDny->action_proc){
						// a flag for quick detection of improperly configured item
						pDny->action_proc = (void *)-1;
						pDny->m_TagValue.Flags = 0;
					}else{
						pDny->m_TagValue.Flags |= TF_Valid;
					}
				}else {
					if(!bCountOnly){
						pTags=pFatherTags+lCount;
						tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
						pDny->action_proc = 0;
					}
					lCount++;
				}
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
				if(!bCountOnly){
					pTags=pFatherTags+lCount;
					tagname_by_text(pTags,(LPCTSTR)pDny->m_TagName);
				}
				lCount++;
			}
		}
	}

	return lCount;
}

BOOL CFView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CElement *pSelElement=m_pSelElement;

	if(pSelElement!=NULL){
		if(pSelElement!=NULL){
			if(pSelElement->m_DnyAttr.m_push!=0 || pSelElement->m_DnyAttr.m_ClsPop){
				if(pSelElement->m_DnyAttr.m_en){
					SetCursor(AfxGetApp()->LoadCursor(IDC_PUSH));
				}else{
					SetCursor(AfxGetApp()->LoadCursor(IDC_UPUSH));
				}
				return true;
			}else{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HELP));
				return true;
			}
		}
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CFView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint pot=point;

	if(nFlags & MK_LBUTTON){
		/*
		pot-=m_potPrv;
		m_potPrv=point;
		CElement::LPtoUP(pot.x,pot.y,&(pot.x),&(pot.y));
		if(m_pSelElement){
			CElement *pSelElement=m_pSelElement;
			if(pSelElement->IsKindOf(RUNTIME_CLASS(CCell))){
				pSelElement=((CCell*)pSelElement)->GetSelElement();
			}
			pSelElement->Slide(pot);
		}
		*/
	}else{
		CElement *pElement,*pSelElement=NULL;
		POSITION pos;
		CElement::LPtoUP(pot.x,pot.y,&(pot.x),&(pot.y));
		for(pos=m_Elements.GetTailPosition();pos!=NULL;){
			pElement=((CElement*)(m_Elements.GetPrev(pos)));
			if(pElement->HitTest(pot.x,pot.y)==TK_PUSH){
				pSelElement=pElement;
				if(pSelElement->IsKindOf(RUNTIME_CLASS(CCell))){
					pSelElement=((CCell*)pSelElement)->GetSelElement();
				}
				break;
			}
		}
		m_pSelElement=pSelElement;
		
		if(NULL==pSelElement){
			for(pos=m_TdElements.GetTailPosition();pos!=NULL;){
				pElement=((CElement*)(m_Elements.GetPrev(pos)));
				if(pElement->HitTest(pot.x,pot.y)==TK_PUSH){
					pSelElement=pElement;
					break;
				}
			}
		}
		m_pSelElement=pSelElement;
	}

	CDialog::OnMouseMove(nFlags, point);
}

#define DWORD_PTR DWORD

class CDelayWriter
{
public:
	TAG_NAME tn;
	TAG_VALUE tv;
	MMRESULT tid;
	CDelayWriter(const TAG_NAME & tn, const TAG_VALUE & tv, DWORD delay);
	static void CALLBACK timerProc(
		UINT uTimerID, 
		UINT uMsg, 
		DWORD_PTR dwUser, 
		DWORD_PTR dw1, 
		DWORD_PTR dw2
		);
	virtual ~CDelayWriter()
	{
		if(tid){
			timeKillEvent(tid);
			tid = 0;
		}
	}
};

class CDelayWriter_x
{
public:
	TAG_NAME tn;
	TAG_VALUE tv;
	MMRESULT tid;
	CDelayWriter_x(const TAG_NAME & tn, const TAG_VALUE & tv, DWORD delay);
	static void CALLBACK timerProc_x(
		UINT uTimerID, 
		UINT uMsg, 
		DWORD_PTR dwUser, 
		DWORD_PTR dw1, 
		DWORD_PTR dw2
		);
	virtual ~CDelayWriter_x()
	{
		if(tid){
			timeKillEvent(tid);
			tid = 0;
		}
	}
};

void CALLBACK CDelayWriter::timerProc(
	UINT uTimerID, 
	UINT uMsg, 
	DWORD_PTR dwUser, 
	DWORD_PTR dw1, 
	DWORD_PTR dw2
	)
{

	CDelayWriter * pThis = (CDelayWriter*)dwUser;
    proxy_write_tags(1, &pThis->tn, &pThis->tv); 
	delete pThis;
}

void CALLBACK CDelayWriter_x::timerProc_x(
	UINT uTimerID, 
	UINT uMsg, 
	DWORD_PTR dwUser, 
	DWORD_PTR dw1, 
	DWORD_PTR dw2
	)
{

	CDelayWriter_x * pThis = (CDelayWriter_x*)dwUser;
	proxy_write_tags_x(1, &pThis->tn, &pThis->tv); 
	delete pThis;
}

CDelayWriter::CDelayWriter(const TAG_NAME & tn, const TAG_VALUE & tv, DWORD delay)
{
	this->tn = tn;
	this->tv = tv;
	tid = timeSetEvent(
		delay,
		20,
		timerProc,
		(DWORD)this,
		TIME_ONESHOT
		);
}

CDelayWriter_x::CDelayWriter_x(const TAG_NAME & tn, const TAG_VALUE & tv, DWORD delay)
{
	this->tn = tn;
	this->tv = tv;
	tid = timeSetEvent(
		delay,
		20,
		timerProc_x,
		(DWORD)this,
		TIME_ONESHOT
		);
}

#define WRITE_ENABLE 1
void CFView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE("lbdown\n");

	m_potPrv=point;
	CElement *pSelElement=m_pSelElement;

	if(pSelElement!=NULL){
		if(pSelElement->m_DnyAttr.m_en){
			if(pSelElement->m_DnyAttr.m_push==BUTTON_WND){
				CString sFileName=pSelElement->m_DnyAttr.m_ButtonWnd.m_TagName+".gdf";
				if(pSelElement->m_DnyAttr.m_ButtonWnd.m_Mode==0){
					if(m_bPopup){
						CRect rt;
						GetWindowRect(&rt);
						m_pCtrDlg->PopupView(sFileName,rt.right,rt.top);
					}else{
						m_pCtrDlg->PopupView(sFileName,point.x,point.y);
					}
				}else{
					m_pCtrDlg->m_curView++;
					if(m_pCtrDlg->m_curView>9) m_pCtrDlg->m_curView=0;
					m_pCtrDlg->m_viewList[m_pCtrDlg->m_curView]=sFileName;
					theView.LoadFile(sFileName);
				}
			}else if(pSelElement->m_DnyAttr.m_push==INPUT_D){

#if WRITE_ENABLE
				if(!gDiaInputD.m_bVisible){
					gDiaInputD.m_bVisible=true;
					gDiaInputD.m_pInputD=&(pSelElement->m_DnyAttr.m_InputD);
					gDiaInputD.DoModal();
					gDiaInputD.m_bVisible=false;
				}
#endif

			}else if(pSelElement->m_DnyAttr.m_push==INPUT_A){

#if WRITE_ENABLE
				if(!gDiaInputA.m_bVisible){
					gDiaInputA.m_bVisible=true;
					gDiaInputA.m_pInputA=&(pSelElement->m_DnyAttr.m_InputA);
					gDiaInputA.m_bValueInited = false;
					gDiaInputA.DoModal();
					gDiaInputA.m_bVisible=false;
				}
#endif
			}else if(pSelElement->m_DnyAttr.m_push==BUTTON_A){

#if WRITE_ENABLE
				// TAG_NAME tn;
				// tagname_by_text(&tn,(LPCTSTR)pSelElement->m_DnyAttr.m_ButtonA.m_TagName);
				pmc_value_t tg;
				tg.Flags |= TF_Valid;
				if(pSelElement->m_DnyAttr.m_ButtonA.m_Fuction==0){// add
					tg.Value.fltValue=getFloat(pSelElement->m_DnyAttr.m_ButtonA.m_TagValue) + 
						pSelElement->m_DnyAttr.m_ButtonA.m_Delta;
					if(tg.Value.fltValue>pSelElement->m_DnyAttr.m_ButtonA.m_UpLimit)
						tg.Value.fltValue=pSelElement->m_DnyAttr.m_ButtonA.m_UpLimit;
				}else{// sub
					tg.Value.fltValue=getFloat(pSelElement->m_DnyAttr.m_ButtonA.m_TagValue) -
						pSelElement->m_DnyAttr.m_ButtonA.m_Delta;
					if(tg.Value.fltValue<pSelElement->m_DnyAttr.m_ButtonA.m_DownLimit)
						tg.Value.fltValue=pSelElement->m_DnyAttr.m_ButtonA.m_DownLimit;
				}
				writeAnalog(
					pSelElement->m_DnyAttr.m_ButtonA.m_TagName,
					tg.Value.fltValue
					);
				if(pSelElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
					((CMCButton*)pSelElement)->m_bPush=true;				
#endif
				SetCapture();
			}else if(pSelElement->m_DnyAttr.m_push==BUTTON_D){

#if WRITE_ENABLE
				TAG_NAME tn;
				tagname_by_text(&tn,(LPCTSTR)pSelElement->m_DnyAttr.m_ButtonD.m_TagName);
				pmc_value_t tg;
				tg.Flags |= TF_Valid;
				if(pSelElement->m_DnyAttr.m_ButtonD.m_Action==0){
					// 置位
					tg.Value.iValue=1;
					//proxy_write_tags(1, &tn, &tg.Value);	
					proxy_write_tags_z(1, &tn, &tg.Value, m_sCurUser.GetBuffer());	
				}else if(pSelElement->m_DnyAttr.m_ButtonD.m_Action==1){
					// 复位
					tg.Value.iValue=0;
					//proxy_write_tags(1, &tn, &tg.Value);
					proxy_write_tags_z(1, &tn, &tg.Value, m_sCurUser.GetBuffer());	
				}else if(pSelElement->m_DnyAttr.m_ButtonD.m_Action==2){
					// 取反
					if(pSelElement->m_DnyAttr.m_ButtonD.m_TagValue.Value.iValue){
						tg.Value.iValue=0;
					}else{
						tg.Value.iValue=1;
					}
					//proxy_write_tags(1, &tn, &tg.Value);
					proxy_write_tags_z(1, &tn, &tg.Value, m_sCurUser.GetBuffer());	
				}else if(pSelElement->m_DnyAttr.m_ButtonD.m_Action==3){
					// 正向脉冲
					tg.Value.iValue=1;
					//proxy_write_tags(1, &tn, &tg.Value);
					proxy_write_tags_z(1, &tn, &tg.Value, m_sCurUser.GetBuffer());	
					/*if(pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth != -1){
						CDelayWriter * writer;
						tg.Value.iValue = 0;
						writer = new CDelayWriter(
							tn, 
							tg.Value, 
							pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth
							);
					}*/
					if(pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth != -1){
						CDelayWriter_x * writer;
						tg.Value.iValue = 0;
						writer = new CDelayWriter_x(
							tn, 
							tg.Value, 
							pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth
							);
					}	
					/*
					tgPulse = tg;
					tgPulse.Value.iValue=0;
					tnPulse = tn;
					wdPulse = pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth;
					m_bPulse = true;
					*/
				}else if(pSelElement->m_DnyAttr.m_ButtonD.m_Action==4){
					// 负向脉冲
					tg.Value.iValue=0;
					//proxy_write_tags(1, &tn, &tg.Value);	
					proxy_write_tags_z(1, &tn, &tg.Value, m_sCurUser.GetBuffer());	
					if(pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth != -1){//非自动
						CDelayWriter_x * writer;
						//tg.Value.iValue = 1;
						tg.Value.iValue = 6;
						writer = new CDelayWriter_x(
							tn, 
							tg.Value, 
							pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth
							);
					}
				}
				if(pSelElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
					((CMCButton*)pSelElement)->m_bPush=true;				
#endif
			
				SetCapture();
			}else if(pSelElement->m_DnyAttr.m_push==TREND_LEFT){
				((CTrend*)pSelElement)->ScrlLeft();
			}else if(pSelElement->m_DnyAttr.m_push==TREND_RIGHT){
				((CTrend*)pSelElement)->ScrlRight();
			}else if(pSelElement->m_DnyAttr.m_push==TREND_VALUE){
				((CTrend *)pSelElement)->m_posX = point.x;
				((CTrend *)pSelElement)->m_posY = point.y;
			}else if(pSelElement->m_DnyAttr.m_push==TREND_TIME){
				((CTrend*)pSelElement)->config();
			}
		}
	}

	
#if 0 
    //OnPaint();
	//send message to refresh IDD_MCVIEW_DIALOG mox 20140109
	
	/*CWnd *pwnd = GetDlgItem(IDD_MCVIEW_DIALOG);
	HWND hWnd = pwnd->GetSafeHwnd();*/
	//::GetDlgItem(hWnd, IDD_MCVIEW_DIALOG);//取到窗口的句柄
	CMcviewDlg dlg;
	HWND hWnd = dlg.m_hWnd;
	if (hWnd != NULL)
	{
	::PostMessage(hWnd,WM_REFRESH_MESSAGE,0,0);
	}
#endif

	CDialog::OnLButtonDown(nFlags, point);

	if(m_pSelElement != NULL){
		if(m_pSelElement->m_DnyAttr.m_ClsPop){
			if(m_bPopup){
				OnCancel();
			}
		}
	}
}



HBRUSH CFView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		
	if(pWnd->GetDlgCtrlID()==IDC_ERROR){		
		// Set the text color to red.
		pDC->SetTextColor(RGB(255, 0, 0));

		// Set the background mode for text to transparent 
		// so background will show thru.
		pDC->SetBkMode(TRANSPARENT);

		// Return handle to our CBrush object.
		//hbr=WHITE_BRUSH;
		hbr=m_brushWhite;
	}

	return hbr;
}

void CFView::UpdateElements()
{
	//用动态属性更新显示
	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		pElement->UpdateView();
	}	
}

/*
	dispatch values gathered from RTDB to the dynamic attribute
	objects.
*/
long CFView::SetDynValues(pmc_value_t *pFatherValues)
{
	pmc_value_t *pValues;
	long lCount=0;
	CDny * pDny;

	POSITION pos;
	CElement *pElement;
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
			lCount=((CCell*)pElement)->SetDynValues(pFatherValues,lCount);
		}else{
			//blink
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Blink));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//button a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//button d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ButtonD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushAA));
			if(pDny->m_Used && !pDny->action_proc){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushD));
			if(pDny->m_Used  && !pDny->action_proc){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color brush da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorBrushDA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenAA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color pen da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorPenDA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text aa
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextAA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//color text da
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ColorTextDA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//direction
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Direction));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//enable
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Enable));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//fill h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//fill v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_FillV));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//input a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputA));
			if(pDny->m_Used){ 
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//input d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_InputD));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//pos h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//pos v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_PosV));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//size h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//size v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SizeV));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//slide h
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideH));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//slide v
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_SlideV));
			if(pDny->m_Used){ 
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//value a
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueA));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//value d
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_ValueD));
			if(pDny->m_Used && !pDny->action_proc){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
			//visible
			pDny=(CDny*)(&(pElement->m_DnyAttr.m_Visible));
			if(pDny->m_Used){
				pValues=pFatherValues+lCount;
				pDny->m_TagValue=*pValues;
				lCount++;
			}
		}
	}

	return lCount;
}

void CFView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	TRACE("lbup\n");

	CElement *pSelElement=m_pSelElement;

	if(pSelElement!=NULL){
		if(pSelElement->m_DnyAttr.m_en){
			if(pSelElement->m_DnyAttr.m_push==BUTTON_D){
				
				TAG_NAME tn;
				tagname_by_text(&tn,(LPCTSTR)pSelElement->m_DnyAttr.m_ButtonD.m_TagName);
				pmc_value_t tg;
				tg.Flags |= TF_Valid;
				if(pSelElement->m_DnyAttr.m_ButtonD.m_Action==3
					&&
					pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth == -1
					){
					tg.Value.iValue=0;
					proxy_write_tags_x(1, &tn, &tg.Value);//只将标签点置0，不记录操作	
				}else if(pSelElement->m_DnyAttr.m_ButtonD.m_Action==4
					&&
					pSelElement->m_DnyAttr.m_ButtonD.m_pulseWidth == -1
					){
					//tg.Value.iValue=1;
					tg.Value.iValue=6;
					proxy_write_tags_x(1, &tn, &tg.Value);
					//proxy_write_tags_z(1, &tn, &tg.Value, m_sCurUser.GetBuffer());	
				}
				if(pSelElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
					((CMCButton*)pSelElement)->m_bPush=false;
				
				ReleaseCapture();
			}
			if(pSelElement->m_DnyAttr.m_push==BUTTON_A){
				if(pSelElement->IsKindOf(RUNTIME_CLASS(CMCButton)))
					((CMCButton*)pSelElement)->m_bPush=false;				
				ReleaseCapture();
			}
		}
	}

	/*
	if(m_bPulse){
		Sleep(wdPulse);
		proxy_write_tags(1, &tnPulse, &tgPulse.Value);	
	}
	*/
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CFView::DrawOnPrinter(CDC *pDC)
{
	//将静态元素画在背景图里
	//画背景色
	CPen *pOldPen;
	pOldPen=(CPen *)pDC->SelectStockObject(NULL_PEN);
	CBrush brushBack(m_WindowColor);
	CBrush *pOldBrush;
	pOldBrush=(CBrush *)pDC->SelectObject(&brushBack); 
	long w,h;
	CElement::LPtoUP(m_lWndWidth,m_lWndHeight,&w,&h);

	gbIsPrint=true;
	CElement::UPtoLP(w,h,&w,&h);
	pDC->Rectangle(0,0,w,h);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetROP2(R2_COPYPEN);
	POSITION pos;
	CElement *pElement;
	for(pos=m_BkElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_BkElements.GetNext(pos)));
		pElement->Draw(pDC);
	}
	for(pos=m_TdElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_TdElements.GetNext(pos)));
		pElement->Draw(pDC);
	}


	//画所有元件
	for(pos=m_Elements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		pElement->Draw(pDC);
	}
	for(pos=m_TdElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_Elements.GetNext(pos)));
		((CTrend*)pElement)->DrawCurve(pDC);
	}
	gbIsPrint=false;
}

//1000原有的报表打印程序
void CFView::PrintView()
{
	if(!m_HaveElements) return;

	CPrintDialog dlg(FALSE);
	if(IDOK!=dlg.DoModal()) return;
	//dlg.GetDefaults();
	
	// is a default printer set up?
	HDC hdcPrinter = dlg.GetPrinterDC();
	if (hdcPrinter == NULL){
		MessageBox("无法找到缺省打印机",NULL,MB_OK|MB_ICONSTOP);
	}else{
		// create a CDC and attach it to the default printer
		CDC dcPrinter;
		dcPrinter.Attach(hdcPrinter);
		
		// call StartDoc() to begin printing
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("操作员画面");
		
		// if it fails, complain and exit gracefully
		if (dcPrinter.StartDoc(&docinfo) < 0){
			MessageBox("无法初始化打印机",NULL,MB_OK|MB_ICONSTOP);
		}else{
			// start a page
			if (dcPrinter.StartPage() < 0){
				MessageBox("无法初始化页",NULL,MB_OK|MB_ICONSTOP);
				dcPrinter.AbortDoc();
			}else{
				// actually do some printing
				dcPrinter.SetMapMode(MM_TWIPS);
				DrawOnPrinter(&dcPrinter);
				dcPrinter.EndPage();
				dcPrinter.EndDoc();
			}
		}
	}
}
//1000新的报表打印程序
void CFView::PrintView(CString strFileName)
//void CPrint::printView(CString strFileName)
{
	CDC memDC, dc;
	//CClientDC dc(this);
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//获取屏幕的句柄

	//取屏幕分辨率,即位图的长和宽
	int bmpWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int bmpHeight = ::GetSystemMetrics(SM_CYSCREEN);
	memDC.CreateCompatibleDC(&dc);
	CBitmap* bitmap = new CBitmap();

	//转换FileName的类型，由CString变为LPCTSTR
	USES_CONVERSION;
	LPCTSTR lpszBmpFile = A2CT((LPCSTR)strFileName);

	//加载要打印的位图
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, lpszBmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	bitmap->Attach(hBitmap);
	BITMAP bm;
	bitmap->GetBitmap(&bm);
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(bitmap);

	/////
	CPrintDialog dlg(FALSE);
	if(IDOK!=dlg.DoModal()) return;;
	//dlg.GetDefaults();

	// is a default printer set up?
	HDC hdcPrinter = dlg.GetPrinterDC();
	if (hdcPrinter == NULL)
	{
		AfxMessageBox("无法找到缺省打印机", NULL,MB_OK | MB_ICONSTOP);
	}
	else
	{
		// create a CDC and attach it to the default printer
		CDC dcPrinter;
		dcPrinter.Attach(hdcPrinter);

		// call StartDoc() to begin printing
		DOCINFO docinfo;
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("操作员画面");

		//mox+++

		CPrintInfo Info;
		Info.m_rectDraw.SetRect(0,0,dcPrinter.GetDeviceCaps(HORZRES),dcPrinter.GetDeviceCaps(VERTRES));
		Info.SetMaxPage (1);
		///

		// if it fails, complain and exit gracefully
		if (dcPrinter.StartDoc(&docinfo) < 0)
		{
			AfxMessageBox("无法初始化打印机",NULL,MB_OK|MB_ICONSTOP);
		}
		else
		{
			// start a page
			if (dcPrinter.StartPage() < 0)
			{
				AfxMessageBox("无法初始化页",NULL,MB_OK|MB_ICONSTOP);
				dcPrinter.AbortDoc();
			}
			else
			{
				// actually do some printing
				dcPrinter.SetMapMode(MM_TEXT);
				dcPrinter.StretchBlt(0, 0, dcPrinter.GetDeviceCaps(HORZRES),dcPrinter.GetDeviceCaps(VERTRES), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				dcPrinter.EndPage();
				memDC.SelectObject(pOldBitmap);
				delete bitmap;
				memDC.DeleteDC();
				dcPrinter.EndDoc(); 
			}
		}
		dcPrinter.Detach();
		dcPrinter.DeleteDC();
	}
}
void CFView::ReSubscrib()
{
}

BOOL CFView::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message){
		DealShortkey(pMsg->wParam);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CFView::DealShortkey(unsigned int nKey)
{
	CString sKey;
	char c;

	//0-9
	if(nKey>=48 && nKey<=57){
		c=nKey;
		sKey.Format("%c",c);
	//A-Z
	}else if(nKey>=65 && nKey<=90){
		c=nKey;
		sKey.Format("%c",c);
	//F1-F12
	}else if(nKey>=112 && nKey<=123){
		sKey.Format("F%d",nKey-111);
	}else{
		return;
	}

	//load shortkeys
	char line[1024];
	CArgs args;
	CString sConf=((CMcviewApp*)::AfxGetApp())->m_path+"file\\shortkey.conf";
	FILE *pFile=fopen((LPCTSTR)sConf,"rt");
	if(NULL!=pFile){
		//load new data
		while(NULL!=fgets(line,1024,pFile)){
			parse_arg_ey(line,&args,",\n");
			if(0==sKey.CompareNoCase(args.argv[1])){
				m_pCtrDlg->m_curView++;
				if(m_pCtrDlg->m_curView>9) m_pCtrDlg->m_curView=0;
				m_pCtrDlg->m_viewList[m_pCtrDlg->m_curView]=args.argv[0];
				CString sFileName;
				sFileName.Format("%s.gdf",args.argv[0]);
				theView.LoadFile(sFileName);
				break;
			}
		}

		fclose(pFile);
	}
}

static CString getTagsDesc(CString tagKey)
{
	TAG_NAME	tn;
	RTK_TAG		*pTag;
	CString		s;

	tagname_by_text(&tn, (LPCSTR)tagKey);
	lock_rtdb(__false, INFINITE);
	s = "N/A";
	pTag = query_tag_f(&tn);
	if(pTag){
		s = pTag->s.Description;
	}
	unlock_rtdb();
	return s;
}

void FillListWithElement(CDlgDnyTip & dlg, CElement * pSelElement)
{
	CString sTagName;
	if(pSelElement->IsKindOf(RUNTIME_CLASS(CCell))){
		CCell * cell;
		cell = (CCell*)pSelElement;
		POSITION pos;
		CElement *pElement;
		for(pos=cell->m_Elements.GetHeadPosition();pos!=NULL;){
			pElement=(CElement *)(cell->m_Elements.GetNext(pos));
			FillListWithElement(dlg, pElement);
		}
		return;
	}
	//input a
	if(pSelElement->m_DnyAttr.m_InputA.m_Used){
		dlg.sTips0 += "输入/模拟量";
		sTagName = pSelElement->m_DnyAttr.m_InputA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//input d
	if(pSelElement->m_DnyAttr.m_InputD.m_Used){
		dlg.sTips0 += "输入/开关量";
		sTagName = pSelElement->m_DnyAttr.m_InputD.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//slide h
	if(pSelElement->m_DnyAttr.m_SlideH.m_Used){
		dlg.sTips0 += "游标/水平";
		sTagName = pSelElement->m_DnyAttr.m_SlideH.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//slide v
	if(pSelElement->m_DnyAttr.m_SlideV.m_Used){
		dlg.sTips0 += "游标/垂直";
		sTagName = pSelElement->m_DnyAttr.m_SlideV.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//button a
	if(pSelElement->m_DnyAttr.m_ButtonA.m_Used){
		dlg.sTips0 += "按钮/模拟量";
		sTagName = pSelElement->m_DnyAttr.m_ButtonA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//button d
	if(pSelElement->m_DnyAttr.m_ButtonD.m_Used){
		dlg.sTips0 += "按钮/开关量";
		sTagName = pSelElement->m_DnyAttr.m_ButtonD.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//button wnd
	if(pSelElement->m_DnyAttr.m_ButtonWnd.m_Used){
		dlg.sTips0 += "按钮/窗口";
		sTagName = pSelElement->m_DnyAttr.m_ButtonWnd.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += " ";
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//操作级别
	//value a
	if(pSelElement->m_DnyAttr.m_ValueA.m_Used){
		dlg.sTips0 += "值/模拟量";
		sTagName = pSelElement->m_DnyAttr.m_ValueA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//value d
	if(pSelElement->m_DnyAttr.m_ValueD.m_Used){
		dlg.sTips0 += "值/开关量";
		sTagName = pSelElement->m_DnyAttr.m_ValueD.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color pen a
	if(pSelElement->m_DnyAttr.m_ColorPenA.m_Used){
		dlg.sTips0 += "线颜色/模拟量";
		sTagName = pSelElement->m_DnyAttr.m_ColorPenA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color pen aa
	if(pSelElement->m_DnyAttr.m_ColorPenAA.m_Used){
		dlg.sTips0 += "线颜色/模拟量报警";
		sTagName = pSelElement->m_DnyAttr.m_ColorPenAA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color pen d
	if(pSelElement->m_DnyAttr.m_ColorPenD.m_Used){
		dlg.sTips0 += "线颜色/开关量";
		sTagName = pSelElement->m_DnyAttr.m_ColorPenD.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color pen da
	if(pSelElement->m_DnyAttr.m_ColorPenDA.m_Used){
		dlg.sTips0 += "线颜色/开关量报警";
		sTagName = pSelElement->m_DnyAttr.m_ColorPenDA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color brush a
	if(pSelElement->m_DnyAttr.m_ColorBrushA.m_Used){
		dlg.sTips0 += "填充色/模拟量";
		sTagName = pSelElement->m_DnyAttr.m_ColorBrushA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color brush aa
	if(pSelElement->m_DnyAttr.m_ColorBrushAA.m_Used){
		dlg.sTips0 += "填充色/模拟量报警";
		sTagName = pSelElement->m_DnyAttr.m_ColorBrushAA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color brush d
	if(pSelElement->m_DnyAttr.m_ColorBrushD.m_Used){
		dlg.sTips0 += "填充色/开关量";
		sTagName = pSelElement->m_DnyAttr.m_ColorBrushD.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color brush da
	if(pSelElement->m_DnyAttr.m_ColorBrushDA.m_Used){
		dlg.sTips0 += "填充色/开关量报警";
		sTagName = pSelElement->m_DnyAttr.m_ColorBrushDA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color text a
	if(pSelElement->m_DnyAttr.m_ColorTextA.m_Used){
		dlg.sTips0 += "文本色/模拟量";
		sTagName = pSelElement->m_DnyAttr.m_ColorTextA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color text aa
	if(pSelElement->m_DnyAttr.m_ColorTextAA.m_Used){
		dlg.sTips0 += "文本色/模拟量报警";
		sTagName = pSelElement->m_DnyAttr.m_ColorTextAA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color text d
	if(pSelElement->m_DnyAttr.m_ColorTextD.m_Used){
		dlg.sTips0 += "文本色/开关量";
		sTagName = pSelElement->m_DnyAttr.m_ColorTextD.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//color text da
	if(pSelElement->m_DnyAttr.m_ColorTextDA.m_Used){
		dlg.sTips0 += "文本色/开关量报警";
		sTagName = pSelElement->m_DnyAttr.m_ColorTextDA.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//size h
	if(pSelElement->m_DnyAttr.m_SizeH.m_Used){
		dlg.sTips0 += "大小/水平";
		sTagName = pSelElement->m_DnyAttr.m_SizeH.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//size v
	if(pSelElement->m_DnyAttr.m_SizeV.m_Used){
		dlg.sTips0 += "大小/垂直";
		sTagName = pSelElement->m_DnyAttr.m_SizeV.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//pos h
	if(pSelElement->m_DnyAttr.m_PosH.m_Used){
		dlg.sTips0 += "位置/水平";
		sTagName = pSelElement->m_DnyAttr.m_PosH.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//pos v
	if(pSelElement->m_DnyAttr.m_PosV.m_Used){
		dlg.sTips0 += "位置/垂直";
		sTagName = pSelElement->m_DnyAttr.m_PosV.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//fill h
	if(pSelElement->m_DnyAttr.m_FillH.m_Used){
		dlg.sTips0 += "填充/水平";
		sTagName = pSelElement->m_DnyAttr.m_FillH.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//fill v
	if(pSelElement->m_DnyAttr.m_FillV.m_Used){
		dlg.sTips0 += "填充/垂直";
		sTagName = pSelElement->m_DnyAttr.m_FillV.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//visible
	if(pSelElement->m_DnyAttr.m_Visible.m_Used){
		dlg.sTips0 += "可见性";
		sTagName = pSelElement->m_DnyAttr.m_Visible.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//blink
	if(pSelElement->m_DnyAttr.m_Blink.m_Used){
		dlg.sTips0 += "闪烁";
		sTagName = pSelElement->m_DnyAttr.m_Blink.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
	//enable
	if(pSelElement->m_DnyAttr.m_Enable.m_Used){
		dlg.sTips0 += "有效性";
		sTagName = pSelElement->m_DnyAttr.m_Enable.m_TagName;
		dlg.sTips1 += sTagName;
		dlg.sTips2 += getTagsDesc(sTagName);
		dlg.sTips0 += "\n";
		dlg.sTips1 += "\n";
		dlg.sTips2 += "\n";
	}
}

void CFView::PlaceWindowAt(CWnd * wnd, LONG x, LONG y)
{
	// auto placing the window under cursor
	RECT r;
	RECT cr;
	LONG w, h;

	GetClientRect(&cr);
	wnd->GetWindowRect(&r);
	w = r.right - r.left;
	h = r.bottom - r.top;
	if(w + x > cr.right){
		r.left = cr.right - w - 10;
	}else{
		r.left = x;
	}
	r.right = r.left + w;
	if(h + y > cr.bottom){
		r.top = cr.bottom - h - 10;
	}else{
		r.top = y;
	}
	r.bottom = r.top + h;
	wnd->MoveWindow(&r, FALSE);
}

void CFView::OnRButtonDown(UINT nFlags, CPoint point)
{
	static CDlgDnyTip dlg;
	dlg.m_pCtrDlg = m_pCtrDlg;

	CElement *pSelElement = m_pSelElement;
	if(pSelElement == NULL) return;
	if(pSelElement->IsKindOf(RUNTIME_CLASS(CTrend))){
		((CTrend *)pSelElement)->m_posX = -1;//point.x;
		//((CTrend *)pSelElement)->m_posY = point.y;
		return;
	}
	
	
	CString sTagName;

	if(pSelElement->m_pFather){
		// search for outmost cell
		while(pSelElement->m_pFather){
			pSelElement = pSelElement->m_pFather;
		}
	}

	if(!dlg.m_hWnd){
		dlg.Create(IDD_DNYTIP, this);
	}

	if(!dlg.m_hWnd){
		return;
	}

	dlg.sTips0 = "";
	dlg.sTips1 = "";
	dlg.sTips2 = "";
	
	WINDOWPLACEMENT wp;
	dlg.GetWindowPlacement(&wp);
	if(!(wp.showCmd & SW_SHOW)){
		PlaceWindowAt(&dlg, point.x, point.y);
	}
	
	FillListWithElement(dlg, pSelElement);
	dlg.UpdateView();

	//dlg.DoModal();
	dlg.ShowWindow(SW_SHOWNORMAL);

	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CFView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	// return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CFView::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	CDialog::OnNcLButtonDblClk(nHitTest, point);
	OnCancel();
}

void CFView::setTrendTags(CString tag1, CString tag2, CString tag3, CString tag4, CString tag5)
{
	POSITION pos;
	
	pos = m_TdElements.GetHeadPosition();
	if(pos == NULL){
		return;
	}

	CTrend *pTrend = (CTrend*)(m_TdElements.GetNext(pos));
	pTrend->m_tagnames[0] = tag1;
	pTrend->m_tagnames[1] = tag2;
	pTrend->m_tagnames[2] = tag3;
	pTrend->m_tagnames[3] = tag4;
	pTrend->m_tagnames[4] = tag5;

	TAG_NAME	tn;
	RTK_TAG		*pTag;
	int i;
	for(i = 0; i < 5; i++){
		lock_rtdb(__false, INFINITE);
		tagname_by_text(&tn, (LPCSTR)(pTrend->m_tagnames[i]));
		pTag = query_tag_f(&tn);
		if(pTag){
			pTrend->m_fMin[i] = pTag->s.fltMinValue;
			pTrend->m_fMax[i] = pTag->s.fltMaxValue;
			// for xy only
			//pTrend->m_fMin[i] = pTag->s.fltMinValue;
			//pTrend->m_fMax[i] = pTag->s.fltMinValue + (pTag->s.fltMaxValue - pTag->s.fltMinValue) * 10.;
		}else{
			pTrend->m_fMin[i] = 0;
			pTrend->m_fMax[i] = 1;
		}
		unlock_rtdb();
	}

	//pTrend->m_autoScale = true;

	pTrend->initCache();
}
