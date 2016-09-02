// MCDrawDoc.cpp : implementation of the CMCDrawDoc class
//

#include "stdafx.h"
#include "resource.h"
#include "MCDraw.h"
#include "MCDrawDoc.h"
#include "MCDrawView.h"
#include "MCDrawDoc.h"
#include "MCDrawView.h"
#include "diaselect.h"
#include "diasave.h"
#pragma hdrstop

#include "direct.h"
#include "shlobj.h"
#include "diareplace.h"
#include "rtkgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern long lWndWidth,lWndHeight;
extern CString sWndCaption;
extern CMCDrawApp theApp;
extern NODE_KEY gui_CurrentNode; 
CMCDrawDoc * theDoc;

/////////////////////////////////////////////////////////////////////////////
// CMCDrawDoc

IMPLEMENT_DYNCREATE(CMCDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CMCDrawDoc, CDocument)
	//{{AFX_MSG_MAP(CMCDrawDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(IDM_SELECT_PROJECT, OnSelectProject)
	ON_COMMAND(IDR_PROJECT_START, OnProjectStart)
	ON_COMMAND(IDR_PROJECT_RUN_CURRENT, OnProjectRunCurrent)
	ON_UPDATE_COMMAND_UI(IDR_PROJECT_RUN_CURRENT, OnUpdateProjectRunCurrent)
	ON_UPDATE_COMMAND_UI(IDM_SHOW_ALL_ATTRIBS, OnUpdateShowAllAttribs)
	ON_COMMAND(IDR_PROJECT_START_DB, OnProjectStartDb)
	ON_COMMAND(IDR_PROJECT_NAME, OnProjectName)
	ON_COMMAND(IDR_PROJECT_RESTART_DB, OnProjectRestartDb)
	ON_COMMAND(IDR_PROJECT_CONFIG_DB, OnProjectConfigDb)
	ON_COMMAND(IDR_PROJECT_STOPDB, OnProjectStopdb)
	ON_UPDATE_COMMAND_UI(IDR_PROJECT_STOPDB, OnUpdateProjectStopdb)
	ON_UPDATE_COMMAND_UI(IDR_PROJECT_START_DB, OnUpdateProjectStartDb)
	ON_UPDATE_COMMAND_UI(IDR_PROJECT_RESTART_DB, OnUpdateProjectRestartDb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMCDrawDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CMCDrawDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//		DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMCDraw to support typesafe binding
//	from VBA.  This IID must match the GUID that is attached to the 
//	dispinterface in the .ODL file.

// {F3B68DD7-186E-495B-B15A-D243AA1A3351}
static const IID IID_IMCDraw =
{ 0xf3b68dd7, 0x186e, 0x495b, { 0xb1, 0x5a, 0xd2, 0x43, 0xaa, 0x1a, 0x33, 0x51 } };

BEGIN_INTERFACE_MAP(CMCDrawDoc, CDocument)
	INTERFACE_PART(CMCDrawDoc, IID_IMCDraw, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCDrawDoc construction/destruction

CMCDrawDoc::CMCDrawDoc()
{
	EnableAutomation();
	AfxOleLockApp();

	m_sCurFile.Empty();
	theDoc = this;
	memset(&m_Node, 0, sizeof(m_Node));
}

CMCDrawDoc::~CMCDrawDoc()
{
	//不清除就会内存泄漏
	POSITION pos;
	for(pos=m_listElements.GetHeadPosition();pos!=NULL;){
		delete m_listElements.GetNext(pos);
	}

	AfxOleUnlockApp();
}

BOOL CMCDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}	




/////////////////////////////////////////////////////////////////////////////
// CMCDrawDoc serialization

void CMCDrawDoc::Serialize(CArchive& ar)
{
	 if (ar.IsStoring())
	{		  
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMCDrawDoc diagnostics

#ifdef _DEBUG
void CMCDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMCDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMCDrawDoc commands

BOOL CMCDrawDoc::OnSaveDocument(LPCTSTR fileName) 
{
	MSXML::IXMLDOMDocumentPtr pDoc;

	HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr))
	{
		 _com_error er(hr);
		 AfxMessageBox(er.ErrorMessage());
		 return FALSE;
	}

	if(docEncoding){
		CString s;
		BSTR str;
		s = CString("<?xml version=\"1.0\" ") + 
			"encoding=\"" + docEncoding 
			+ "\" ?><hmif></hmif>";
		str = s.AllocSysString();
		pDoc->loadXML(str);
		SysFreeString(str);
	}else{
		pDoc->loadXML(L"<?xml version=\"1.0\" ?><hmif></hmif>");
	}
	
	MSXML::IXMLDOMElementPtr pRoot=pDoc->documentElement;;
	_variant_t val;
	//window attribute
	val.vt=VT_UI4;
	val.ulVal=m_WindowColor;
	pRoot->setAttribute(L"c",val);
	val.ulVal=lWndWidth;
	pRoot->setAttribute(L"w",val);
	val.ulVal=lWndHeight;
	pRoot->setAttribute(L"h",val);
	val.vt=VT_BSTR;
	val.bstrVal=sWndCaption.AllocSysString();
	pRoot->setAttribute(L"cap",val);

	//save all visible element
	POSITION pos;
	CElement *pElement;
	for(pos=m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_listElements.GetNext(pos)));
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(pElement->m_lStatus==ST_NOMAL) pElement->Save(pRoot);
		}
	}
	
	CString sTemp;
	sTemp = theApp.m_path + "/" + "file/" + fileName;
	pDoc->save((LPCTSTR)sTemp);

	pDoc.Release();

	return TRUE;
}

BOOL CMCDrawDoc::OnOpenDocument(LPCTSTR fileName) 
{
	CString sTranslated;
	MSXML::IXMLDOMDocumentPtr pDoc;
	CString sTemp;
	_bstr_t strFile=L"hmif";

	HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);
	if (FAILED(hr)){
		 _com_error er(hr);
		 AfxMessageBox(er.ErrorMessage());
		 return FALSE;
	}

	sTemp = theApp.m_path + "/file/" + fileName;
	if(pDoc->load((LPCTSTR )sTemp)==FALSE){
		 AfxMessageBox("无法打开文件");
		 return FALSE;
	}

	MSXML::IXMLDOMElementPtr pRoot=pDoc->documentElement;
	if(pRoot->baseName!=strFile){
		 AfxMessageBox("文件类型错误");
		 return FALSE;
	}

	//清除原有内容
	POSITION pos;
	
	pos=this->GetFirstViewPosition();
	CMCDrawView * pView = (CMCDrawView*)this->GetNextView(pos);
	pView->m_pCurElement=NULL;
	pView->m_SelElements.RemoveAll();

	for(pos=m_listElements.GetHeadPosition();pos!=NULL;){
		delete m_listElements.GetNext(pos);
	}	 
	m_listElements.RemoveAll();

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
	val=pRoot->getAttribute(L"w");
	val.ChangeType(VT_UI4);
	lWndWidth=val.ulVal;
	val=pRoot->getAttribute(L"h");
	val.ChangeType(VT_UI4);
	lWndHeight=val.ulVal;
	val=pRoot->getAttribute(L"cap");
	sWndCaption=val.bstrVal;

//!!! note if a node is not exist 
//use selectSingleNode will raise a fatal error under windows 98 !!!
//this is a test

//IXMLDOMNodePtr pTest;
//hr=pRoot->raw_selectSingleNode(L"this must failed",&pTest);
//if(FAILED(hr)){
//	TRACE("this test succeed");
//}else{
//	TRACE("this test failed");
//}
//pRoot->selectSingleNode(L"fasf");

	MSXML::IXMLDOMElementPtr pItem;
	CElement * pElement;
	for(pItem=pRoot->firstChild;pItem!=NULL;pItem=pItem->nextSibling){
		if(pItem->baseName==strGO){
			val=pItem->getAttribute(L"type");
			if(val==strRectangle){
				pElement=new CRectangle();
				((CRectangle*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strArc){
				pElement=new CArc();
				((CArc*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strEllipse){
				pElement=new CEllipse();
				((CEllipse*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strLine){
				pElement=new CLine();
				((CLine*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strMCButton){
				pElement=new CMCButton();
				((CMCButton*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strMeta){
				pElement=new CMeta();
				((CMeta*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strRoundRect){
				pElement=new CRoundRect();
				((CRoundRect*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strText){
				pElement=new CText();
				((CText*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strPolygon){
				pElement=new CPolygon();
				((CPolygon*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strPolyLine){
				pElement=new CPolyLine();
				((CPolyLine*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strTrend){
				pElement=new CTrend();
				((CTrend*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}else if(val==strCell){
				pElement=new CCell();
				((CCell*)pElement)->Load(pItem);
				m_listElements.AddTail(pElement);
			}
		}
	}

	pDoc.Release();
	UpdateAllViews(NULL);

	m_sCurFile=fileName;
	documentChanged();

	return TRUE;
}

void CMCDrawDoc::OnFileOpen() 
{
	int iRet;
	CString sTemp;

	if(!m_sCurFile.IsEmpty()){
		sTemp.Format("是否保存当前文件<%s>", m_sCurFile);
		iRet=MessageBox(theApp.m_pMainWnd->m_hWnd,sTemp, "提示", MB_YESNOCANCEL| MB_ICONQUESTION );
		if(iRet==IDYES){
			OnFileSave();
		}else if(iRet==IDNO){
		}else{
			return;
		}
	}else if(!m_listElements.IsEmpty()){
		sTemp.Format("是否保存当前组态内容");
		iRet=MessageBox(theApp.m_pMainWnd->m_hWnd,sTemp, "提示", MB_YESNOCANCEL| MB_ICONQUESTION );
		if(iRet==IDYES){
			OnFileSaveAs();
		}else if(iRet==IDNO){
		}else{
			return;
		}
	}

	CString sFilePath;
	CDiaSelect dlg;
	dlg.m_select = m_sCurFile.Left(m_sCurFile.GetLength() - 4);
	dlg.m_dir = CString(get_working_dir())+"file";
	dlg.m_wildcards = "*.gdf";
	if(dlg.DoModal() != IDOK){
		return; 
	}

	OnOpenDocument(dlg.m_select + ".gdf");
}

void CMCDrawDoc::OnFileSave() 
{
	if(m_sCurFile.IsEmpty()){
		CString sFilePath;	
		CDiaSave dlg;
		dlg.m_dir = "file\\*.gdf";
		dlg.m_baseDir = "file";
		dlg.m_wildCards = "*.gdf";
		dlg.m_selectMode = 0;
		if(dlg.DoModal()!=IDOK){
			return;
		}
		m_sCurFile = dlg.m_select + ".gdf";
	}
	this->OnSaveDocument(m_sCurFile);
	documentChanged();
}

void CMCDrawDoc::OnFileSaveAs() 
{
	CString sFilePath=m_sCurFile;
	m_sCurFile.Empty();
	OnFileSave();
	if(m_sCurFile.IsEmpty()){
		m_sCurFile=sFilePath;
	}

	documentChanged();
}

void CMCDrawDoc::OnFileNew() 
{
	int iRet;
	CString sTemp;
	if(!m_sCurFile.IsEmpty()){
		sTemp.Format("是否保存当前文件<%s>",m_sCurFile);
		iRet=MessageBox(theApp.m_pMainWnd->m_hWnd,sTemp, "提示", MB_YESNOCANCEL| MB_ICONQUESTION );
		if(iRet==IDYES){
			OnFileSave();
		}else if(iRet==IDNO){
		}else{
			return;
		}
	}else if(!m_listElements.IsEmpty()){
		sTemp.Format("是否保存当前组态内容");
		iRet=MessageBox(theApp.m_pMainWnd->m_hWnd,sTemp, "提示", MB_YESNOCANCEL| MB_ICONQUESTION );
		if(iRet==IDYES){
			OnFileSaveAs();
		}else if(iRet==IDNO){
		}else{
			return;
		}
	}

	lWndWidth=TOTAL_WIDTH;
	lWndHeight=TOTAL_HEIGHT;
	sWndCaption="";
	m_WindowColor=RGB(200,200,200);

	//清除原有内容
	POSITION pos;
	
	pos=this->GetFirstViewPosition();
	CMCDrawView * pView = (CMCDrawView*)this->GetNextView(pos);
	pView->m_pCurElement=NULL;
	pView->m_SelElements.RemoveAll();

	for(pos=m_listElements.GetHeadPosition();pos!=NULL;){
		delete m_listElements.GetNext(pos);
	}	 
	m_listElements.RemoveAll();

	m_sCurFile.Empty();
	documentChanged();

	UpdateAllViews(NULL);
}

int CMCDrawDoc::AllowNew()
{
	if(!m_bDemo) return 1;

	POSITION pos;
	CElement *pElement;
	int nCount=0;
	//有动态连接的或单元
	for(pos=m_listElements.GetHeadPosition();pos!=NULL;){
		pElement=((CElement*)(m_listElements.GetNext(pos)));//->Draw(&dcMem);
		if(pElement->m_pFather==NULL || pElement->m_lStatus==ST_ZORDER){
			while(pElement->m_pChild!=NULL){
				if(pElement->m_pChild->m_lStatus==ST_DISCARD) break;
				pElement=pElement->m_pChild;
				if(pElement->m_lStatus==ST_ZORDER) break;
			}
			if(ST_NOMAL==pElement->m_lStatus){
				if(pElement->IsKindOf(RUNTIME_CLASS(CCell))){
					nCount+=((CCell*)pElement)->GetDynCount();
				}else if(pElement->m_DnyAttr.m_Linked){
					nCount++;
				}
			}
		}
	}
	
	if(nCount>=MAX_DEMO_DYN_COUNT) return 0;
	else return 1;
}


BOOL CMCDrawDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	int iRet;
	CString sTemp;
	if(!m_sCurFile.IsEmpty()){
		sTemp.Format("是否保存当前文件<%s>",m_sCurFile);
		iRet=MessageBox(theApp.m_pMainWnd->m_hWnd,sTemp, "提示", MB_YESNOCANCEL| MB_ICONQUESTION );
		if(iRet==IDYES){
			OnFileSave();
		}else if(iRet==IDNO){
		}else{
			return FALSE;
		}
	}else if(!m_listElements.IsEmpty()){
		sTemp.Format("是否保存当前组态内容");
		iRet=MessageBox(theApp.m_pMainWnd->m_hWnd,sTemp, "提示", MB_YESNOCANCEL| MB_ICONQUESTION );
		if(iRet==IDYES){
			OnFileSaveAs();
		}else if(iRet==IDNO){
		}else{
			return FALSE;
		}
	}

	return CDocument::CanCloseFrame(pFrame);
}


void CMCDrawDoc::OnSelectProject() 
{
	char szNewPath[MAX_PATH];
	BROWSEINFO bs;
	LPITEMIDLIST list;
	bool failed = false;

	// TODO: Add your command handler code here
	if(!m_sCurFile.IsEmpty()){
		OnFileNew();
	}
	if(!m_sCurFile.IsEmpty()){
		return;
	}
	memset(&bs, 0, sizeof(bs));
	bs.hwndOwner = theApp.GetMainWnd()->m_hWnd;
	bs.pidlRoot = NULL;
	bs.lpszTitle = "选择工程所在的目录";
	bs.pszDisplayName = szNewPath;
	strncpy(szNewPath, get_working_dir(), sizeof(szNewPath));
	szNewPath[sizeof(szNewPath) - 1] = 0;
	bs.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS ; //| BIF_NEWDIALOGSTYLE;
	list = SHBrowseForFolder(&bs);
	if(!list){
		return;
	}

	if(!SHGetPathFromIDList(list, szNewPath)){
		failed = true;
	}
	CoTaskMemFree(list);

	if(failed){
		return;
	}
	
	set_working_dir(szNewPath);
	theApp.m_path = szNewPath;
	SetCurrentDirectory(szNewPath);
	_mkdir(theApp.m_path + "/file");

	documentChanged();
}

static bool _exec_command(const char * cmd, ...)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	char cmdLine[1024];
	va_list va;
	
	va_start(va, cmd);
	_vsnprintf(cmdLine, sizeof(cmdLine), cmd, va);
	cmdLine[sizeof(cmdLine) - 1] = 0;
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	if(CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}else{
		return false;
	}
	return true;
}

static NODE_KEY _getLocalNodeName()
{
	NODE_KEY nk;
	char serverName[rtkm_node_key_length + 1];
	GetPrivateProfileString(
		"PMC",
		"ServerName",
		"local-server",
		serverName,
		sizeof(serverName),
		get_config_file()
		);
	nk = CNodeName(serverName);
	return nk;
}

class CWaitPredicate{
public:
	virtual bool end() = 0;
};

static void _wait(const char * caption, CWaitPredicate * w)
{
	HANDLE ctx;
	HANDLE hStop;
	LARGE_INTEGER liDueTime;
	
	liDueTime.QuadPart=-2000000;
	
	ctx = RtkGuiWDStart(WDS_PROGRESS | WDS_STATE | WDS_CANCEL, NULL);
	RtkGuiWDSetCaption(ctx, caption);
	hStop = CreateWaitableTimer(NULL, FALSE, NULL);
	SetWaitableTimer(hStop, &liDueTime, 0, NULL, 0, 0);
	
	bool bStop = false;
	DWORD dwWait;
	RTK_TIME start, now;
	MSG msg;
	
	rtk_time_mark(&start);
	/* wait 1 second, process messages inside */
	while(!bStop){
		dwWait = MsgWaitForMultipleObjects(
			1, &hStop, FALSE, 
			INFINITE,
			QS_ALLINPUT
			);
		rtk_time_mark(&now);
		RtkGuiWDSetPos(ctx, rtk_time_diff(&now, &start) * 20.);	
		switch(dwWait){
		case WAIT_OBJECT_0 + 1:
			while(PeekMessage(&msg,NULL,0,0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (RtkGuiWDGetBtnState(ctx) & WDC_CANCEL){
				bStop = true;
			}
			if(w->end()){
				bStop = true;
			}
			break;
		case WAIT_TIMEOUT:
		case WAIT_OBJECT_0:
			bStop = TRUE;
			break;
		}
	}
	
	CloseHandle(hStop);
	RtkGuiWDDestroy(ctx);
}

void CMCDrawDoc::OnProjectStart() 
{
	// TODO: Add your command handler code here
	// execute command 'mcview -p:<projDir> -switchable'
	if(!_exec_command(
		"%smcview.exe -p:%s -switchable",
		get_exe_dir(),
		get_working_dir()		
		))
	{
		AfxMessageBox("不能启动运行程序");
	}
}

void CMCDrawDoc::OnProjectRunCurrent() 
{
	// TODO: Add your command handler code here
	// execute command 'mcview -p:<projDir> -switchable -f:<fileName>'
	if(!_exec_command(
		"%smcview.exe -p:%s -switchable -f:%s", 
		get_exe_dir(),
		get_working_dir(),
		(LPCSTR)m_sCurFile
		))
	{
		AfxMessageBox("不能启动运行程序");
	}
}

void CMCDrawDoc::OnUpdateProjectRunCurrent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_sCurFile.IsEmpty()){
		pCmdUI->Enable(FALSE);
	}else{
		pCmdUI->Enable();
	}	
}

void CMCDrawDoc::OnUpdateShowAllAttribs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}

void CMCDrawDoc::documentChanged()
{
	CString sCap;
	char projName[_MAX_PATH];
	GetPrivateProfileString("PMC","ProjectName",get_working_dir(),projName,sizeof projName,get_config_file());
	if(m_sCurFile.IsEmpty()){
		sCap.Format("MC Draw - 新建文件 - [%s]",  projName);
	}else{
		sCap.Format("MC Draw - %s - [%s]", m_sCurFile, projName);
	}
	theApp.GetMainWnd()->SetWindowText(sCap);		
}

class CWaitStartDBPredicate : public CWaitPredicate{
	virtual bool end()
	{
		NODE_KEY nk;
		nk = _getLocalNodeName();
		return query_node(&nk)? true : false;
	}
};

class CWaitStopDBPredicate : public CWaitPredicate{
	virtual bool end()
	{
		NODE_KEY nk;
		nk = _getLocalNodeName();
		return !query_node(&nk)? true : false;
	}
};

void CMCDrawDoc::OnProjectStartDb() 
{
	// TODO: Add your command handler code here
	// execute command 'pmcd -p:<projDir>'
	if(!_exec_command(
		"%spmcd.exe -p:%s -d", 
		get_exe_dir(),
		get_working_dir(),
		(LPCSTR)m_sCurFile
		))
	{
		AfxMessageBox("不能启动运行程序");
	}
	CWaitStartDBPredicate p;
	_wait("正在启动实时数据库，请等待", &p);
}

void CMCDrawDoc::OnProjectName() 
{
	char serverName[rtkm_node_key_length + 1];
	// TODO: Add your command handler code here
	GetPrivateProfileString(
		"PMC",
		"ServerName",
		"local-server",
		serverName,
		sizeof(serverName),
		get_config_file()
		);
    CDiaReplace dia;
    dia.m_original = serverName;
    dia.m_replaced = serverName;
    if(dia.DoModal() != IDOK){
		return;
	}
	WritePrivateProfileString(
		"PMC", 
		"ServerName", 
		dia.m_replaced, 
		get_config_file()
		);
	_exec_command(
		"%skill.exe -n:%s -m:\"rename %s\"",
		get_exe_dir(),
		(LPCSTR)dia.m_original,
		(LPCSTR)dia.m_replaced
		);
	CWaitStopDBPredicate p1;
	_wait("正在停止实时数据库，请等待", &p1);
	CWaitStartDBPredicate p2;
	_wait("正在启动实时数据库，请等待", &p2);
}

void CMCDrawDoc::OnProjectRestartDb() 
{
	// TODO: Add your command handler code here
	// execute command 'kill <serverName> restart'
	char serverName[rtkm_node_key_length + 1];

	GetPrivateProfileString(
		"PMC",
		"ServerName",
		"local-server",
		serverName,		
		sizeof(serverName),
		get_config_file()
		);

	if(!_exec_command(
		"%skill.exe -n:%s -b:system -m:restart", 
		get_exe_dir(),
		serverName
		))
	{
		AfxMessageBox("不能启动运行程序");
	}
	CWaitStopDBPredicate p1;
	_wait("正在停止实时数据库，请等待", &p1);
	CWaitStartDBPredicate p2;
	_wait("正在启动实时数据库，请等待", &p2);
}

void CMCDrawDoc::OnProjectConfigDb() 
{
	// TODO: Add your command handler code here
	// execute command 'pmc.exe'

	if(!_exec_command(
		"%spmc.exe", 
		get_exe_dir()
		))
	{
		AfxMessageBox("不能启动运行程序");
	}
}

void CMCDrawDoc::OnProjectStopdb() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// execute command 'kill <serverName> restart'
	char serverName[rtkm_node_key_length + 1];
	
	GetPrivateProfileString(
		"PMC",
		"ServerName",
		"local-server",
		serverName,		
		sizeof(serverName),
		get_config_file()
		);
	
	if(!_exec_command(
		"%skill.exe -n:%s -b:system -m:exit", 
		get_exe_dir(),
		serverName
		))
	{
		AfxMessageBox("不能启动运行程序");
	}	
	CWaitStopDBPredicate p1;
	_wait("正在停止实时数据库，请等待", &p1);
}

void CMCDrawDoc::OnUpdateProjectStopdb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	NODE_KEY nk;
	nk = _getLocalNodeName();
	pCmdUI->Enable(query_node(&nk)? true : false);
}

void CMCDrawDoc::OnUpdateProjectStartDb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	NODE_KEY nk;
	nk = _getLocalNodeName();
	pCmdUI->Enable(query_node(&nk)? false : true);
}

void CMCDrawDoc::OnUpdateProjectRestartDb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	NODE_KEY nk;
	nk = _getLocalNodeName();
	pCmdUI->Enable(query_node(&nk)? true : false);	
}
