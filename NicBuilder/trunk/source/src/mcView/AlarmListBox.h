#if !defined(AFX_ALARMLISTBOX_H__6E7444D8_468B_479A_B285_BBD226BFB511__INCLUDED_)
#define AFX_ALARMLISTBOX_H__6E7444D8_468B_479A_B285_BBD226BFB511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmListBox window

class CAlarmListBox : public CListBox
{
// Construction
public:
	CAlarmListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAlarmListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAlarmListBox)
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMLISTBOX_H__6E7444D8_468B_479A_B285_BBD226BFB511__INCLUDED_)
