//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <FileCtrl.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TButton *Button1;
    TListView *lst;
    TActionManager *ActionManager1;
    TAction *expLic;
    TAction *impLic;
    TListView *lst2;
    TLabel *Label1;
    TLabel *Label2;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TOpenDialog *of;
    TLabel *Label3;
    void __fastcall expLicExecute(TObject *Sender);
    void __fastcall impLicExecute(TObject *Sender);
    void __fastcall expLicUpdate(TObject *Sender);
    void __fastcall impLicUpdate(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;

extern AnsiString remoteFile;
extern AnsiString localFile;

//---------------------------------------------------------------------------
#endif
