//---------------------------------------------------------------------------


#ifndef untSerialPortViewH
#define untSerialPortViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TframSerialPort : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TComboBox *cmbBaud;
    TLabel *Label2;
    TPanel *Panel1;
    TRadioButton *rbOdd;
    TRadioButton *rbEven;
    TRadioButton *rbBlank;
    TRadioButton *rbMark;
    TRadioButton *rbNone;
    TLabel *Label3;
    TPanel *Panel2;
    TRadioButton *rb5;
    TRadioButton *rb6;
    TRadioButton *rb8;
    TRadioButton *rb7;
    TLabel *Label4;
    TPanel *Panel3;
    TRadioButton *rb1;
    TRadioButton *rb1_5;
    TRadioButton *rb2;
    TImage *Image1;
    TLabel *Label5;
    TButton *btnSave;
    void __fastcall btnSaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframSerialPort(TComponent* Owner);
    void __fastcall UpdataView();
    AnsiString Validate();
};
//---------------------------------------------------------------------------
extern PACKAGE TframSerialPort *framSerialPort;

#endif
