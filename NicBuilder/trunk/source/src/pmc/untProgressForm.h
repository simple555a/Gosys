//---------------------------------------------------------------------------

#ifndef untProgressFormH
#define untProgressFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmProgress : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblThis;
    TProgressBar *pbThis;
    TButton *btnCancel;
    TLabel *lblTotal;
    TProgressBar *pbTotal;
    TTimer *Timer1;
    TListBox *lb;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmProgress(TComponent* Owner);
    bool Download();
    bool Upload();
    NODE_KEY node;
    bool bDownload;
    AnsiString file;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmProgress *frmProgress;
//---------------------------------------------------------------------------
#endif
