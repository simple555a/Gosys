//---------------------------------------------------------------------------


#ifndef untDeviceViewH
#define untDeviceViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TframDeviceView : public TFrame
{
__published:	// IDE-managed Components
    TLabel *lblInitString;
    TEdit *edtInit;
    TLabel *lblVersion;
    TEdit *edtAddress;
    TButton *Button1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edtVendor;
    TEdit *edtType;
    TButton *Button2;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframDeviceView(TComponent* Owner);
    void __fastcall UpdateView();
    bool ValidateView(PDEVICE_INFO di);
};
//---------------------------------------------------------------------------
extern PACKAGE TframDeviceView *framDeviceView;
//---------------------------------------------------------------------------
#endif
