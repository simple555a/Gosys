#if !defined(AFX_OPTGRID_H__0009E0C1_FC69_11D4_82FA_0080C8F63C8D__INCLUDED_)
#define AFX_OPTGRID_H__0009E0C1_FC69_11D4_82FA_0080C8F63C8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptGrid dialog

class COptGrid : public CPropertyPage
{
	DECLARE_DYNCREATE(COptGrid)

// Construction
public:
	COptGrid();
	~COptGrid();

// Dialog Data
	//{{AFX_DATA(COptGrid)
	enum { IDD = IDD_OPT_GRID };
	long	m_HorGrid;
	BOOL	m_LockOnGrid;
	long	m_VerGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptGrid)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptGrid)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTGRID_H__0009E0C1_FC69_11D4_82FA_0080C8F63C8D__INCLUDED_)
