// MCDrawDoc.h : interface of the CMCDrawDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include <afxcoll.h>

#if !defined(AFX_MCDRAWDOC_H__4C7E039A_0EB9_4309_8945_3C5EC7290647__INCLUDED_)
#define AFX_MCDRAWDOC_H__4C7E039A_0EB9_4309_8945_3C5EC7290647__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMCDrawDoc : public CDocument
{
protected: // create from serialization only
	CMCDrawDoc();
	DECLARE_DYNCREATE(CMCDrawDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCDrawDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bRemote;
	NODE_KEY m_Node;

    bool m_bDemo;
	COLORREF m_WindowColor;
	CMapStringToOb m_mapNameToOb;
	CObList m_listElements;
	virtual ~CMCDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void documentChanged();

// Generated message map functions
public:
	int AllowNew();
	// void OpenFile(CString sFilePath);
	//{{AFX_MSG(CMCDrawDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	afx_msg void OnSelectProject();
	afx_msg void OnProjectStart();
	afx_msg void OnProjectRunCurrent();
	afx_msg void OnUpdateProjectRunCurrent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowAllAttribs(CCmdUI* pCmdUI);
	afx_msg void OnProjectStartDb();
	afx_msg void OnProjectName();
	afx_msg void OnProjectRestartDb();
	afx_msg void OnProjectConfigDb();
	afx_msg void OnProjectStopdb();
	afx_msg void OnUpdateProjectStopdb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProjectStartDb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProjectRestartDb(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMCDrawDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
	CString m_sCurFile;
};

extern CMCDrawDoc * theDoc;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCDRAWDOC_H__4C7E039A_0EB9_4309_8945_3C5EC7290647__INCLUDED_)
