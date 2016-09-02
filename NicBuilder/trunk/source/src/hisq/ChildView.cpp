// ChildView.cpp : implementation of the CChildView class
//

/*
改动历史
Date         Version    Author       Modify
2012-7-8     1.0.0.4    Zander Sun   1.字体显示增大--DrawTag()
                                     2.曲线画面的定位虚线上的时间显示在虚线的左侧--CChildView::DrawCurve()
2012-7-9     1.0.0.4    Zander Sun   1.增加对int16、int64、date数据类型的处理--CChildView::DrawCurve()


*/

#include "stdafx.h"
#include "config.h"
#include "hisq.h"
#include "ChildView.h"
#include "mainfrm.h"
//2012-7-9
#include "hisqcomm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4244)

const int cHorGap=40;
const int cTagWidth=100;
const int cVerGap=100;
const int cLabelHeight=50;

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	in_curve = false;
	pos_x = 0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

inline void DrawTag(CString str, CRect rt, UINT nFormat, CDC *pDC,float fsc,COLORREF clr=RGB(0,0,0))
{
	LOGFONT lft;
	lft.lfCharSet=DEFAULT_CHARSET;
	lft.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lft.lfEscapement=0;	
	strcpy(lft.lfFaceName,"宋体");
	lft.lfHeight=(LONG)(20*fsc);//2012-7-8
	lft.lfItalic=false;
	lft.lfOrientation=0;
	lft.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lft.lfPitchAndFamily=DEFAULT_PITCH;
	lft.lfQuality=DEFAULT_QUALITY;
	lft.lfStrikeOut=false;
	lft.lfUnderline=false;
	lft.lfWeight=FW_NORMAL;
	lft.lfWidth=0;

	CFont ft;
	ft.CreateFontIndirect(&lft);
	CFont *pOldFont=pDC->SelectObject(&ft);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(clr); 


	pDC->DrawText(str,rt,nFormat);
	pDC->SelectObject(pOldFont);
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawCurve(&dc);
}

static CString getTagName(PCTAG_NAME tn)
{
	char buf[256];
	tagname_to_text(tn, buf, sizeof(buf));
	return CString(buf);
}

void CChildView::DrawCurve(CDC *pDC,bool bPrint)
{
	CMainFrame* pmf=(CMainFrame*)::AfxGetMainWnd();

	CRect rectClient;
	float fsx=1.,fsy=1.,fsc=1.;
	pDC->SetMapMode(MM_ANISOTROPIC);
	GetClientRect(rectClient);
	pDC->SetWindowExt(600+cTagWidth+2*cHorGap,1000+cLabelHeight+2*cVerGap);
	pDC->SetViewportExt(rectClient.right,-rectClient.bottom);
	if(bPrint){
		CPoint pt(pDC->GetDeviceCaps(HORZRES),pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP (&pt);
		fsx=(float)(pt.x/(600+cTagWidth+2*cHorGap));
		fsy=(float)(-pt.y/(1000+cLabelHeight+2*cVerGap));
		if(fsx>fsy) fsc=fsy;
		else fsc=fsx;
	}

	pDC->SetViewportOrg(0,fsy*rectClient.bottom);
	pDC->Rectangle(fsx*(cHorGap+cTagWidth),fsy*(cVerGap+cLabelHeight),
		fsx*(cHorGap+600+cTagWidth),fsy*(cVerGap+1000+cLabelHeight));
	int i,j;
	CPen penGray(PS_SOLID,0,RGB(192,192,192));
	CPen *pOldPen=(CPen*)pDC->SelectObject(&penGray);
	if(pmf->m_bShowGrid){
		for(i=1;i<pmf->m_iVerGrid;i++){
			pDC->MoveTo(fsx*(cHorGap+cTagWidth+i*600/pmf->m_iVerGrid),fsy*(cVerGap+cLabelHeight));
			pDC->LineTo(fsx*(cHorGap+cTagWidth+i*600/pmf->m_iVerGrid),fsy*(cVerGap+cLabelHeight+1000));
		}
		for(i=1;i<pmf->m_iHorGrid;i++){
			pDC->MoveTo(fsx*(cHorGap+cTagWidth),fsy*(cVerGap+cLabelHeight+i*1000/pmf->m_iHorGrid));
			pDC->LineTo(fsx*(cHorGap+cTagWidth+600),fsy*(cVerGap+cLabelHeight+i*1000/pmf->m_iHorGrid));
		}
	}
	pDC->SelectObject(pOldPen);

	CString str;
	CRect rtTag;
	FILETIME ftm;
	SYSTEMTIME stm;
	int xOld,yOld;
	int x,y;
	
	if(pos_x){
		CPen penPos(PS_DASH,0,RGB(0,0,0));
		pOldPen=pDC->SelectObject(&penPos);
		pDC->MoveTo(fsx*(cHorGap+cTagWidth+pos_x),fsy*(cVerGap+cLabelHeight));
		pDC->LineTo(fsx*(cHorGap+cTagWidth+pos_x),fsy*(cVerGap+cLabelHeight+1000));
		pDC->SelectObject(pOldPen); 
		ftm = hislogTimeByKey(cacheInfo.startTime+pos_x*cacheInfo.step);
		FileTimeToSystemTime(&ftm,&stm);
		str.Format("%4d年%2d月%2d日 %2d:%2d:%2d",stm.wYear,stm.wMonth,stm.wDay,
			stm.wHour,stm.wMinute,stm.wSecond);	
// 		rtTag.SetRect(fsx*(cHorGap+cTagWidth+pos_x),fsy*(cVerGap+1.5*cLabelHeight+1000),
// 			fsx*(cHorGap+cTagWidth+pos_x+300),fsy*(cVerGap+1000));
		/*以虚线顶端为基准向左画一个长为200的矩形，然后把时间字符串放在矩形中，右对齐，
		这样时间字符串显示在虚线的左侧*/
		rtTag.SetRect(fsx*(cHorGap+cTagWidth+pos_x-200),fsy*(cVerGap+1.5*cLabelHeight+1000),
			fsx*(cHorGap+cTagWidth+pos_x),fsy*(cVerGap+1000));
		DrawTag(str,rtTag,DT_RIGHT|DT_TOP,pDC,fsc);//2012-7-8
	}

	//time	
	cacheGetInfo(archiveCache, &cacheInfo);
	ftm = hislogTimeByKey(cacheInfo.startTime);
	FileTimeToSystemTime(&ftm,&stm);
	str.Format("%4d年%2d月%2d日 %2d:%2d:%2d",stm.wYear,stm.wMonth,stm.wDay,
		stm.wHour,stm.wMinute,stm.wSecond);	
	rtTag.SetRect(fsx*(cHorGap+cTagWidth),fsy*(cVerGap+cLabelHeight),
		fsx*(cHorGap+cTagWidth+300),fsy*(cVerGap));
	DrawTag(str,rtTag,DT_LEFT|DT_TOP,pDC,fsc);

	ftm = hislogTimeByKey(cacheInfo.startTime + cacheInfo.step * cacheInfo.valueCount);
	FileTimeToSystemTime(&ftm,&stm);
	str.Format("%4d年%2d月%2d日 %2d:%2d:%2d",stm.wYear,stm.wMonth,stm.wDay,
		stm.wHour,stm.wMinute,stm.wSecond);	
	rtTag.SetRect(fsx*(cHorGap+cTagWidth+300),fsy*(cVerGap+cLabelHeight),
		fsx*(cHorGap+cTagWidth+600),fsy*(cVerGap));
	DrawTag(str,rtTag,DT_RIGHT|DT_TOP,pDC,fsc);

	PCpmc_value_t value;
	CString strValue;
	//float fy;
	//2012-7-8
// 	SYSTEMTIME stm_z;
// 	FILETIME ftm_z;
	string str_mid;//2012-7-9
	double fy;
	for(i=0; i<cacheInfo.tagCount; i++){
		CPen pen(PS_SOLID,pmf->m_width[i],pmf->m_color[i]);
		pOldPen=(CPen*)pDC->SelectObject(&pen);
		if(!cacheGetFlag(archiveCache, i) || pmf->m_names[i].IsEmpty()){
			//
		}else{
			value = cacheGetValue(archiveCache, i, pos_x);
			switch(get_value_type(value->Flags)){
			case dt_real4:
				strValue.Format(pmf->m_format,value->Value.fltValue);		
				break;
			case dt_bool:
				if(0==value->Value.b){
					strValue.Format("0");;		
				}else{
					strValue.Format("1");;		
				}
				break;
			case dt_int32:
				strValue.Format("%d",value->Value.iValue);
				break;
			case dt_real8:
				strValue.Format(pmf->m_format,value->Value.dblValue);
				break;
			//2012-7-8
			case dt_int16:
				strValue.Format("%d", value->Value.i16);
				break;
			case dt_int64:
				strValue.Format("%d", value->Value.i64);
				break;
			case dt_date:
// 				memcpy(&ftm_z, &value->Value.date, sizeof(__r8));
// 				FileTimeToSystemTime(&ftm_z, &stm_z);
// 				str.Format("%4d年%2d月%2d日 %2d:%2d:%2d",stm_z.wYear,stm_z.wMonth,stm_z.wDay,
// 		        stm_z.wHour,stm_z.wMinute,stm_z.wSecond);
// 				str_mid = dateAsString(value->Value.date);
// 				strValue = str_mid.data();
// 				break;
			default:
				strValue.Format(".");		
			}

			rtTag.SetRect(fsx*(cHorGap),fsy*(cVerGap+cLabelHeight+(10-i)*100),
				fsx*(cHorGap+cTagWidth-15),fsy*(cVerGap+cLabelHeight+(9-i)*100));
			CString strTag;
			strTag = getTagName(&cacheInfo.names[i]);
			str.Format("%d#:%s\n%s\n%s\n%g - %g",i+1,strTag,pmf->m_discription[i],strValue,pmf->m_fMin[i],pmf->m_fMax[i]);
			DrawTag(str,rtTag,DT_WORDBREAK,pDC,fsc,pmf->m_color[i]);

			xOld=-1;
			for(j=0; j<600; j++){
				value = cacheGetValue(archiveCache, i, j);
				if(!(value->Flags & TF_Valid)){
					xOld = -1;
				}else{
					x = fsx*(cHorGap+cTagWidth+j);
					switch(get_value_type(value->Flags)){
					case dt_real4:
						fy = pmf->m_fMax[i] - pmf->m_fMin[i];
						if(fy < 0.000001) fy = 0.000001;
						y = (value->Value.fltValue-pmf->m_fMin[i]) / fy * 1000;
						y = fsy*(cVerGap+cLabelHeight+y);
						break;
					case dt_bool:
						fy=pmf->m_fMax[i]-pmf->m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						if(0==value->Value.b){
							y = (0-pmf->m_fMin[i]) / fy * 1000;
						}else{
							y = (1-pmf->m_fMin[i]) / fy * 1000;
						}
						y = fsy*(cVerGap+cLabelHeight+y);
						break;
					case dt_int32:
						fy=pmf->m_fMax[i]-pmf->m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y = (value->Value.iValue-pmf->m_fMin[i]) / fy * 1000;
						y = fsy*(cVerGap+cLabelHeight+y);
						break;
					case dt_real8:
						fy=pmf->m_fMax[i]-pmf->m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y = (value->Value.dblValue-pmf->m_fMin[i]) / fy * 1000;
						y = fsy*(cVerGap+cLabelHeight+y);
						break;
					//2012-7-9
					case dt_int16:
						fy=pmf->m_fMax[i]-pmf->m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y = (value->Value.iValue-pmf->m_fMin[i]) / fy * 1000;
						y = fsy*(cVerGap+cLabelHeight+y);
						break;
					case dt_int64:
						fy=pmf->m_fMax[i]-pmf->m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y = (value->Value.iValue-pmf->m_fMin[i]) / fy * 1000;
						y = fsy*(cVerGap+cLabelHeight+y);
						break;
					case dt_date:
						fy=pmf->m_fMax[i]-pmf->m_fMin[i];
						if(fy<0.000001) fy=0.000001;
						y = (value->Value.dblValue-pmf->m_fMin[i]) / fy * 1000;
						y = fsy*(cVerGap+cLabelHeight+y);
						break;						
					}
					
					if(y>=fsy*(cVerGap+cLabelHeight) && y<=fsy*(cVerGap+cLabelHeight+1000)){
						if(xOld>0){
							pDC->MoveTo(xOld,yOld);
							pDC->LineTo(x,y);
						}else{
							pDC->SetPixelV(x,y,pmf->m_color[i]);
						}
						xOld=x;
						yOld=y;
					}else{
						xOld=-1;
					}
				}
			}
		}
		pDC->SelectObject(pOldPen);
	}
 }

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(in_curve){
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return true;
	}
	
	return CWnd ::OnSetCursor(pWnd, nHitTest, message);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(in_curve){
		float x;
		x = cur_x - cTagWidth - cHorGap;
		pos_x = (int)x;
		InvalidateRect(NULL);
	}
	
	CWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	pos_x = 0;
	InvalidateRect(NULL);

	CWnd ::OnRButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rectClient;
	GetClientRect(rectClient);
	cur_x = point.x * (600+cTagWidth+2*cHorGap) / rectClient.Width();
	cur_y = point.y * (1000+cLabelHeight+2*cVerGap) / rectClient.Height();
	
	in_curve = true;
	if(cur_x < (cTagWidth + cHorGap)){
		in_curve = false;
	}
	if(cur_x > (600 + cTagWidth + cHorGap)){
		in_curve = false;
	}
	if(cur_y < (cVerGap)){
		in_curve = false;
	}
	if(cur_y > (1000 + cVerGap)){
		in_curve = false;
	}

	CWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd ::OnKeyDown(nChar, nRepCnt, nFlags);
}
