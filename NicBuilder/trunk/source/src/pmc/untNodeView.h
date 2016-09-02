//---------------------------------------------------------------------------


#ifndef untNodeViewH
#define untNodeViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "configurable.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------
class TframNodeView : public TFrame
{
__published:	// IDE-managed Components
    TButton *Button1;
    TLabel *Label1;
    TButton *Button2;
    TButton *Button3;
    TGroupBox *GroupBox1;
    TEdit *edtDownload;
    TSpeedButton *btnPickSave;
    TButton *btnStartSave;
    TGroupBox *GroupBox2;
    TEdit *edtUpload;
    TButton *btnStartRead;
    TSpeedButton *btnPickRead;
    TOpenDialog *ofd;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall btnPickSaveClick(TObject *Sender);
    void __fastcall btnPickReadClick(TObject *Sender);
    void __fastcall btnStartSaveClick(TObject *Sender);
    void __fastcall btnStartReadClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframNodeView(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TframNodeView *framNodeView;
//---------------------------------------------------------------------------
#endif
