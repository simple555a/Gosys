#include "precomp.h"
#pragma hdrstop

#include "htmlhelp.h"
#include <rtkgui.h>

#include "pmcSeeker.h"

#pragma comment(lib, "pmcSeeker.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "untTagItem"
#pragma link "untTagItem"
#pragma link "PERFGRAP"
#pragma resource "*.dfm"

TfrmMain *frmMain;

#define MAX_ENTRIES_IN_STACK 256
static AnsiString s_UrlStack[MAX_ENTRIES_IN_STACK];
static int s_CurrentUrlStack, s_UrlStackDepth;

static FILE  *fp_log=fopen("debug\\dbdiag.txt", "wt");
CDCSItem g_DCS;
HWND g_hHelpWindow;

class TTagDrag : public TControl
{
public:
	TTreeNode * srcNode;
};
#define err(s) stBar->Panels->Items[0]->Text = (s)

static int PMC_API utils_puts_with_log(MSG_LEVEL level,char *s)
{
	if(fp_log){
		fputs(s,fp_log);
	}
	return fputs(s,stdout);
}

static void _cdecl _OnProgress(__uint t, __uint c, const char * msg)
{
	frmMain->stBar->Panels->Items[0]->Text = msg;
	if(t){
		frmMain->pgBar->Position = c*100/t;
	}
}

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}

void _InitViews()
{
	framAddDevice = new TframAddDevice(0);
	framAddGroup = new TframAddGroup(0);
	framAddTag = new TframAddTag(0);
	framConfigurable = new TframConfigurable(0);
	framDB = new TframDB(0);
	framDCSView = new TframDCSView(0);
	framDevices = new TframDevices(0);
	framDeviceView = new TframDeviceView(0);
	framGroup = new TframGroup(0);
	// framHisLogView = new TframHisLogView(0);
	framNodeView = new TframNodeView(0);
	#define cc(t) t = new T##t(0)
	cc(framPictureView);
	cc(framSerialPort);
	cc(framSerialPorts);
	cc(framTag);
}

void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	int i;
	bool bStayOnTop = false;
	for(i=1; i<=ParamCount(); i++){
		if(LowerCase(ParamStr(i)) == "-st"){
			bStayOnTop = true;
		}
	}
	
	frmSplash = new TfrmSplash(0);
	frmSplash->Visible = true;
	frmSplash->Update();

	_InitViews();

	// OldScrollBoxWndProc = pnlConfig->WindowProc;
	// pnlConfig->WindowProc = ScrollBoxWndProc;

	utils_puts = utils_puts_with_log;
	// pnlConfig->Caption = "";
	// lblHelp->Caption = "在这里书写帮助\r\n可以使用超级链接";
	// ImageList1->FileLoad(rtBitmap, g_AppPath + "res\\16x16.bmp", clWhite);

	g_ProxyDispatchTable.OnProgress = _OnProgress;
	InitConfigManager();

	//
	TTreeNode *node;
	node = tv->Items->AddChild(0, g_DCS.m_sCaption);
	node->Data = &g_DCS;
	g_DCS.m_ViewItem = node;
	node->ImageIndex = g_DCS.m_iIcon;
	node->SelectedIndex = g_DCS.m_iIcon;
	node->HasChildren = true;
	node->Expand(false);

	pgBar->Parent = stBar;
	pgBar->Left = stBar->Panels->Items[0]->Width + 3;
	pgBar->Top = 3;
	pgBar->Visible = true;
	pgBar->Width = stBar->Panels->Items[1]->Width - 4;
	pgBar->Height = stBar->Height - 4;
	// ((TWinControl*)pgBar)->Ctl3D = false;

	httpAdd->Tag = ca_Add;
	httpSave->Tag = ca_Save;
	httpDelete->Tag = ca_Delete;

	init_proxy(frmSplash->Handle, PROXY_ENABLE_DISPATCHER | PROXY_ENABLE_BGLOADER);

	Sleep(1500);
	frmSplash->Visible = false;
	delete frmSplash;

	if(bStayOnTop){
		FormStyle = fsStayOnTop;
	}
	
	Sleep(800);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	uninit_proxy();
	if(fp_log){
		fclose(fp_log);
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmMain::actAddExecute(TObject *Sender)
{
	//
	InvokeAction(ca_Add);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actDeleteExecute(TObject *Sender)
{
	InvokeAction(ca_Delete);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SetMoreLess()
{
	/*
	framTag->Visible = g_bShowMore;
	if(g_bShowMore){
		actMoreLess->Caption = "<<";
		ClientWidth = framTag->Left + framTag->Width;
	}else{
		actMoreLess->Caption = ">>...";
		ClientWidth = framTag->Left;
	}
	*/
}


void TfrmMain::NavigateTo(PCTAG_NAME n)
{
	AnsiString url;
	url = ToUrl(n);
	CConfigurableItem * it;
	it = g_DCS.Search(url);
	utils_trace("Url=%s, %08x", url.c_str(), it);
	if(it){
		it->m_ViewItem->Selected = true;
		it->m_ViewItem->MakeVisible();
	}
}

TTreeNode *__fastcall TfrmMain::FindSubNode(TTreeNode *start, void *_val)
{
	return 0;
}

TTreeNode *__fastcall TfrmMain::FindNodeByName(PCTAG_NAME n)
{
	return 0;
}



void TfrmMain::GetPath(TAG_NAME &name)
{
}

void __fastcall TfrmMain::tvExpanding(TObject *Sender, TTreeNode *Node,
	  bool &AllowExpansion)
{
	CConfigurableItem *item, *sitem;

	item = (CConfigurableItem*)Node->Data;

	if(item->m_iRefreshCount){
		return;
	}

	item->m_PropertyPage->Tag = (int)item;
	item->Refresh();

	Node->HasChildren = item->m_bExpandable && item->GetItems();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actRefreshExecute(TObject *Sender)
{
	InvokeAction(ca_Refresh);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::UpdateActionStatus()
{
	actCopy->Enabled = g_CurItem->Support(ca_Copy);
	actPaste->Enabled = g_CurItem->Support(ca_Paste) &&
		g_CurItem->CanPaste();
	actAdd->Enabled = g_CurItem->Support(ca_Add);
	actCut->Enabled = g_CurItem->Support(ca_Cut);
	actDelete->Enabled = g_CurItem->Support(ca_Delete);
	actGoBack->Enabled = s_CurrentUrlStack > 1;
	actSave->Enabled = g_CurItem->Support(ca_Edit);
	actGoForward->Enabled = s_CurrentUrlStack < s_UrlStackDepth;
}

void __fastcall TfrmMain::actSaveExecute(TObject *Sender)
{
	InvokeAction(ca_Save);
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::actFindExecute(TObject *Sender)
{
	//
	if(Discover()){
		RTK_TAG t;
		if(GetFirstTag(&t)){
			TAG_NAME tn;
			tn.node = t.node;
			tn.sname.group = t.group;
			tn.sname.tag = t.key;
			utils_debug(
				"Navigating to %s.%s.%s\n",
				(char*)CNodeName(tn.node),
				(char*)CGroupName(tn.sname.group),
				(char*)CTagName(tn.sname.tag)
				);
			NavigateTo(&tn);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actExitExecute(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actAboutExecute(TObject *Sender)
{
	// AboutBox->ShowModal();
	pmcAbout(Handle, "数据库及系统组态");
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::actGoBackExecute(TObject *Sender)
{
	//
	assert(s_CurrentUrlStack > 1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actGoForwardExecute(TObject *Sender)
{
	//
	assert(s_CurrentUrlStack <= s_UrlStackDepth);
}
//---------------------------------------------------------------------------

void TfrmMain::ShowPage(TWinControl *pg)
{
	TWinControl * ppOld;
	ppOld = (TWinControl*)pnlConfig->Tag;
	if(ppOld == pg){
		return;
	}
	pg->Align = alClient;
	pg->Visible = true;
	if(ppOld){
		pnlConfig->RemoveComponent(ppOld);
		pnlConfig->Tag = 0;
		ppOld->Parent = 0;
		ppOld->Visible = false;
	}
	pnlConfig->InsertComponent(pg);
	pg->Parent = pnlConfig;
	pg->Tag = (int)g_CurItem;
	pnlConfig->Tag = (int)pg;
	pnlConfig->Refresh();
}


void __fastcall TfrmMain::ScrollBoxWndProc(Messages::TMessage &Message)
{
	if(Message.Msg == WM_ERASEBKGND){
		return;
	}
	OldScrollBoxWndProc(Message);
}

void __fastcall TfrmMain::httpAddMouseEnter(TObject *Sender)
{
	TLabel * lbl;
	lbl = (TLabel *)Sender;
	lbl->Font->Style = lbl->Font->Style << fsUnderline;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::httpAddMouseLeave(TObject *Sender)
{
	TLabel * lbl;
	lbl = (TLabel *)Sender;
	lbl->Font->Style = lbl->Font->Style >> fsUnderline;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::httpAddClick(TObject *Sender)
{
	//
	TLabel * lbl = dynamic_cast<TLabel*>(Sender);
	if(lbl){
		InvokeAction((CConfigAction)lbl->Tag);
	}
}

class CWaitCursor
{
public:
	CWaitCursor()
	{
		crOldCursor = Screen->Cursor;
	}
	virtual ~CWaitCursor()
	{
		Screen->Cursor = crOldCursor;
	}
	void Set(TCursor c)
	{
		Screen->Cursor = c;
	}
protected:
	TCursor crOldCursor;
};

//---------------------------------------------------------------------------
void InvokeAction(CConfigAction act, ...)
{
	CConfigurableItem * it;
	TTreeView *tv = frmMain->tv;
	DWORD res;
	CWaitCursor c;

	if(!g_CurItem){
		return;
	}

	try{
		switch(act){
		case ca_Add:
			c.Set(crHourGlass);
			if(g_CurItem->m_AddPage){
				frmMain->lblCaption->Caption = AnsiString("新建") + g_CurItem->m_ClassName;
				frmMain->ShowPage(g_CurItem->m_AddPage);
				g_CurItem->UI_AddItem(frmMain);
			}
		break;
		case ca_Refresh:
			c.Set(crHourGlass);
			g_CurItem->Refresh();
			g_CurItem->UI_UpdateView();
		break;
		case ca_CreateSub:
			if(g_CurItem->m_CreateSubPage){
				frmMain->lblCaption->Caption = AnsiString("新建配置项目");
                                //frmMain->lblCaption->Caption = AnsiString("新建") + g_CurItem->m_ClassName;
				frmMain->ShowPage(g_CurItem->m_CreateSubPage);
				g_CurItem->UI_CreateSub();
			}
		break;
		case ca_Delete:
			res = MessageBox(
				frmMain->Handle,
				(AnsiString("确定要删除 ") + g_CurItem->m_sCaption + " 吗?").c_str(),
				Application->Title.c_str(),
				MB_YESNO + MB_ICONQUESTION
				);
			if(res != IDYES){
				return;
			}
			c.Set(crHourGlass);
			tv->Items->BeginUpdate();
			g_CurItem->Delete();
			tv->Items->EndUpdate();
			g_CurItem = 0;
		break;
		case ca_Save:
			c.Set(crHourGlass);
			tv->Items->BeginUpdate();
			g_CurItem->Save();
			tv->Items->EndUpdate();
			g_CurItem->m_ViewItem->HasChildren = g_CurItem->m_bExpandable && g_CurItem->GetItems();
		break;
		case ca_Help:
			c.Set(crHourGlass);
			AnsiString Url;
			if(file_exists((g_AppPath + "NicBuilder.chm").c_str())){
				Url = g_AppPath + "NicBuilder.chm";
				Url += g_CurItem->m_sHelpTopic;
				html_help(GetDesktopWindow(), Url.c_str(), HH_DISPLAY_TOPIC, NULL);
			}else{
				Url = g_AppPath + "help/" + g_CurItem->m_ClassName + ".htm";
				if(!file_exists(Url.c_str())){
					Url = g_AppPath + "help/" + g_CurItem->m_ClassName + ".txt";
				}
				ShellExecute(frmMain->Handle, "open", Url.c_str(), NULL, NULL, SW_SHOW);
			}
		break;
		}
	}catch (Exception &exception){
		 Application->ShowException(&exception);
	}
}

void __fastcall TfrmMain::tvMouseUp(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y)
{
	//
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actHelpExecute(TObject *Sender)
{
	//
	InvokeAction(ca_Help);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	if( Key == VK_F1 ){
		InvokeAction(ca_Help);
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::tvChange(TObject *Sender, TTreeNode *Node)
{
	TWinControl *ppNew;
	CConfigurableItem *item;

	if(!Node){
		return;
	}
	item = (CConfigurableItem*)Node->Data;

	g_CurItem = item;

	if(!item->m_iRefreshCount){
		item->Refresh();
	}
	Node->HasChildren = item->GetItems();

	ppNew = item->m_PropertyPage;
	ppNew->Tag = (int)item;
	lblCaption->Caption = item->m_sCaption;
	item->UI_UpdateView();
	ShowPage(ppNew);

	UpdateActionStatus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tvClick(TObject *Sender)
{
	//
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tvDragOver(TObject *Sender, TObject *Source,
	  int X, int Y, TDragState State, bool &Accept)
{
	//	  
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tvDragDrop(TObject *Sender, TObject *Source,
	  int X, int Y)
{
	//
}
//---------------------------------------------------------------------------

