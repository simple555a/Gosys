#if !defined(AFX_TESTBTN_H__C6A817B3_92EA_4A1F_8CA2_F9BD073D0642__INCLUDED_)
#define AFX_TESTBTN_H__C6A817B3_92EA_4A1F_8CA2_F9BD073D0642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestBtn window

class CTestBtn : public CButton
{
// Construction
public:
	CTestBtn();

	BOOL m_bIsChecked;  //2016.02.15 GM �жϰ�ť��״̬
// Attributes
protected:

	//��ť����߿�
	CPen m_BoundryPen;
	
	//���ָ�����ڰ�ť֮��ʱ��ť���ڱ߿�
	CPen m_InsideBoundryPenLeft;
	CPen m_InsideBoundryPenRight;
	CPen m_InsideBoundryPenTop;
	CPen m_InsideBoundryPenBottom;
	
	//��ť��ý���ʱ��ť���ڱ߿�
	CPen m_InsideBoundryPenLeftSel;
	CPen m_InsideBoundryPenRightSel;
	CPen m_InsideBoundryPenTopSel;
	CPen m_InsideBoundryPenBottomSel;
	
	//��ť�ĵ�ɫ��������Ч����Ч����״̬
	CBrush m_FillActive;
	CBrush m_FillInactive;

	
	//��ť��״̬
	BOOL m_bOver;		//���λ�ڰ�ť֮��ʱ��ֵΪtrue����֮Ϊflase
	BOOL m_bTracking;	//����갴��û���ͷ�ʱ��ֵΪtrue

	BOOL m_bFocus;		//��ťΪ��ǰ��������ʱ��ֵΪtrue
	BOOL m_bSelected;	//��ť�������Ǹ�ֵΪtrue
 


	COLORREF ActiveColor;
	COLORREF InactiveColor;
	int		 m_RgbFill1;
	int		 m_RgbFill2;
	int		 m_RgbFill3;
	//��ʱ��
//	UINT m_nTimerRefresh;
//	LPDRAWITEMSTRUCT lpDrawItemStruct1;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestBtn)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);	
	virtual void DoGradientFill(CDC *pDC, CRect* rect);
	virtual void DrawInsideBorder(CDC *pDC, CRect* rect);
	void setColor(COLORREF color1, COLORREF color2, int m_Rgb1,int m_Rgb2,int m_Rgb3);
//	void setColor(COLORREF color1, COLORREF color2);
	virtual ~CTestBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTestBtn)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBTN_H__C6A817B3_92EA_4A1F_8CA2_F9BD073D0642__INCLUDED_)
