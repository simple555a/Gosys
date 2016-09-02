//---------------------------------------------------------------------------

#ifndef untFormAddGroupH
#define untFormAddGroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <pmc.h>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TfrmAddGroup : public TForm
{
__published:	// IDE-managed Components
    TComboBoxEx *cmbDevice;
    TLabel *Label7;
    TEdit *edtCName;
    TLabel *Label6;
    TButton *btnSave;
    TLabel *Label1;
    TEdit *edtName;
    TButton *Button1;
    TLabel *lblHint;
    TButton *Button2;
    TImage *Image1;
    TLabel *Label4;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmAddGroup(TComponent* Owner);
    bool __fastcall ValidateData(PRTK_GROUP grp);

    TAG_NAME name;
    AnsiString m_sHint;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddGroup *frmAddGroup;
//---------------------------------------------------------------------------
#endif
