//---------------------------------------------------------------------------

#ifndef untFormAddDeviceH
#define untFormAddDeviceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAddDevice : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *edtAddress;
    TLabel *Label2;
    TEdit *edtInit;
    TCheckBox *chkAutoGen;
    TButton *Button1;
    TButton *Button2;
    TGroupBox *GroupBox1;
    TEdit *edtGroupKey;
    TLabel *Label3;
    TButton *Button3;
    TImage *Image1;
    TLabel *Label4;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmAddDevice(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddDevice *frmAddDevice;
//---------------------------------------------------------------------------
#endif
