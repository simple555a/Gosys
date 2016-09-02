//---------------------------------------------------------------------------


#ifndef untDCSViewH
#define untDCSViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TframDCSView : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *edtName;
    TLabel *Label2;
    TImage *Image1;
    TLabel *lblList;
    TButton *Button1;
    TButton *Button2;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframDCSView(TComponent* Owner);
    void UpdateView();
};
//---------------------------------------------------------------------------
extern PACKAGE TframDCSView *framDCSView;
//---------------------------------------------------------------------------
#endif
