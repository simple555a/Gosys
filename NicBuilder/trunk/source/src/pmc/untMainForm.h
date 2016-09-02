//---------------------------------------------------------------------------

#ifndef untMainFormH
#define untMainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "untTagItem.h"
#include <ImgList.hpp>
#include "untTagItem.h"
#include <Graphics.hpp>
#include "PERFGRAP.h"

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
    TActionManager *ActionManager1;
    TAction *actRefresh;
    TAction *actAdd;
    TAction *actDelete;
    TAction *actFind;
    TStatusBar *stBar;
    TPopupMenu *PopupMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TTimer *Timer1;
    TProgressBar *pgBar;
    TImageList *ImageList1;
    TToolBar *ToolBar1;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    TToolButton *ToolButton3;
    TToolButton *ToolButton4;
    TMainMenu *MainMenu1;
    TMenuItem *F1;
    TMenuItem *X1;
    TMenuItem *H1;
    TMenuItem *I1;
    TMenuItem *A1;
    TMenuItem *N9;
    TToolButton *ToolButton5;
    TToolButton *ToolButton6;
    TToolButton *ToolButton8;
    TToolButton *ToolButton10;
    TPanel *pnlRight;
    TSplitter *Splitter1;
    TPanel *Panel1;
    TImage *Image1;
    TLabel *lblCaption;
    TScrollBox *pnlConfig;
    TToolButton *ToolButton17;
    TToolButton *ToolButton18;
    TAction *actCopy;
    TAction *actPaste;
    TAction *actSave;
    TAction *actSaveAll;
    TAction *actHelp;
    TToolButton *ToolButton7;
    TAction *actCut;
    TMenuItem *E1;
    TMenuItem *aaaaaa1;
    TMenuItem *bbbbbbbb1;
    TMenuItem *cccccccc1;
    TMenuItem *dddddddd1;
    TMenuItem *F2;
    TMenuItem *N10;
    TAction *actExit;
    TAction *actAbout;
    TToolButton *ToolButton9;
    TToolButton *ToolButton11;
    TAction *actGoBack;
    TAction *actGoForward;
    TScrollBox *ScrollBox1;
    TImage *Image2;
    TLabel *httpAdd;
    TLabel *httpSave;
    TLabel *httpDelete;
    TLabel *httpPrevious;
    TLabel *httpNext;
    TPanel *Panel2;
    TTreeView *tv;
    TToolBar *ToolBar2;
    TToolButton *ToolButton12;
    TToolButton *ToolButton13;
    TToolButton *ToolButton14;
    TToolButton *ToolButton15;
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall actAddExecute(TObject *Sender);
    void __fastcall actDeleteExecute(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall tvExpanding(TObject *Sender, TTreeNode *Node,
          bool &AllowExpansion);
    void __fastcall actRefreshExecute(TObject *Sender);
    void __fastcall actSaveExecute(TObject *Sender);
    void __fastcall actFindExecute(TObject *Sender);
    void __fastcall actExitExecute(TObject *Sender);
    void __fastcall actAboutExecute(TObject *Sender);
    void __fastcall actGoBackExecute(TObject *Sender);
    void __fastcall actGoForwardExecute(TObject *Sender);
    void __fastcall httpAddMouseEnter(TObject *Sender);
    void __fastcall httpAddMouseLeave(TObject *Sender);
    void __fastcall httpAddClick(TObject *Sender);
    void __fastcall tvMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall actHelpExecute(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall tvChange(TObject *Sender, TTreeNode *Node);
    void __fastcall tvClick(TObject *Sender);
    void __fastcall tvDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
    void __fastcall tvDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
private:	// User declarations
    void __fastcall SetMoreLess();
    void __fastcall UpdateActionStatus();
    void __fastcall ScrollBoxWndProc(Messages::TMessage &Message);
    void (__closure __fastcall *OldScrollBoxWndProc)(Messages::TMessage &Message);
public:		// User declarations
    __fastcall TfrmMain(TComponent* Owner);
    void __fastcall RefreshDB(void);
    void NavigateTo(PCTAG_NAME name);
    void ShowPage(TWinControl * pg);
    TTreeNode *__fastcall FindNodeByName(PCTAG_NAME);
    TTreeNode *__fastcall TfrmMain::FindSubNode(TTreeNode *start, void*val);
    void GetPath(TAG_NAME &);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
