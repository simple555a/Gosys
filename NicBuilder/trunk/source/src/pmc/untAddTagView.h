//---------------------------------------------------------------------------


#ifndef untAddTagViewH
#define untAddTagViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "PERFGRAP.h"
#include <ExtCtrls.hpp>
#include "untTagView.h"
//---------------------------------------------------------------------------
class TframAddTag : public TFrame
{
__published:	// IDE-managed Components
    TframTag *framTag1;
    TButton *btnNext;
    TButton *btnHelp;
    TLabel *lblHint;
    void __fastcall btnNextClick(TObject *Sender);
    void __fastcall btnHelpClick(TObject *Sender);
        void __fastcall framTag1cmbTypeChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframAddTag(TComponent* Owner);
    void DoAdd();
};
//---------------------------------------------------------------------------
extern PACKAGE TframAddTag *framAddTag;
//---------------------------------------------------------------------------
#endif
