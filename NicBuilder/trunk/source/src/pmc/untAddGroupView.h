//---------------------------------------------------------------------------


#ifndef untAddGroupViewH
#define untAddGroupViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TframAddGroup : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label6;
    TEdit *edtCName;
    TLabel *Label4;
    TComboBox *cmbPeriod;
    TLabel *Label1;
    TEdit *edtName;
    TButton *btnNext;
    TButton *btnHelp;
    TLabel *lblHint;
    void __fastcall btnNextClick(TObject *Sender);
    void __fastcall btnHelpClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframAddGroup(TComponent* Owner);
    void Show();
    void DoAdd();
    NODE_KEY node;
};
//---------------------------------------------------------------------------
extern PACKAGE TframAddGroup *framAddGroup;
//---------------------------------------------------------------------------
#endif
