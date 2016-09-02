//---------------------------------------------------------------------------

#ifndef untNewItemViewH
#define untNewItemViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

//---------------------------------------------------------------------------
class TfrmNewItem : public TForm
{
__published:	// IDE-managed Components
    void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmNewItem(TComponent* Owner);

public:
    TWinControl * frame;

    void ShowWith(TWinControl * f);
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmNewItem *frmNewItem;
//---------------------------------------------------------------------------
#endif
