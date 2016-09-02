//---------------------------------------------------------------------------


#ifndef untDriverViewH
#define untDriverViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TframDriverView : public TFrame
{
__published:	// IDE-managed Components
    TEdit *edtInit;
    TLabel *lblInitString;
    TLabel *lblVendor;
    TButton *Button1;
    TLabel *lblVersion;
    TEdit *edtVersion;
    TEdit *edtVendor;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframDriverView(TComponent* Owner);
    void __fastcall UpdateView();
};
//---------------------------------------------------------------------------
extern PACKAGE TframDriverView *framDriverView;
//---------------------------------------------------------------------------
#endif
