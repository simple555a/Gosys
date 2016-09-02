//---------------------------------------------------------------------------


#ifndef untSerialPortsViewH
#define untSerialPortsViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TframSerialPorts : public TFrame
{
__published:	// IDE-managed Components
    TButton *Button1;
    TLabel *Label1;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframSerialPorts(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TframSerialPorts *framSerialPorts;
//---------------------------------------------------------------------------
#endif
