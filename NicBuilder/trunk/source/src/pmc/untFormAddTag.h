//---------------------------------------------------------------------------

#ifndef untFormAddTagH
#define untFormAddTagH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "untTagItem.h"
#include "untTagView.h"
//---------------------------------------------------------------------------
class TfrmAddTag : public TForm
{
__published:	// IDE-managed Components
    TframTag *framTag;
    TButton *btnAdd;
    TButton *btnClose;
    TLabel *lblHint;
    TButton *Button1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall btnCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmAddTag(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddTag *frmAddTag;
//---------------------------------------------------------------------------
#endif
