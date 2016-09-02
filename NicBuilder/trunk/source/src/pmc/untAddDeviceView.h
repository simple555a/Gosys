//---------------------------------------------------------------------------


#ifndef untAddDeviceViewH
#define untAddDeviceViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TframAddDevice : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TButton *Button1;
    TButton *Button2;
    TGroupBox *GroupBox2;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *edtAddress;
    TEdit *edtInit;
    TLabel *Label7;
    TEdit *edtName;
    TLabel *Label8;
    TListBox *lstVendors;
    TListBox *lstTypes;
    TLabel *Label9;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall lstVendorsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframAddDevice(TComponent* Owner);
    void InitView();
    NODE_KEY node;
};
//---------------------------------------------------------------------------
extern PACKAGE TframAddDevice *framAddDevice;
//---------------------------------------------------------------------------
#endif
