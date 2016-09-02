//---------------------------------------------------------------------------


#ifndef untDevicesViewH
#define untDevicesViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <pmc.h>

//---------------------------------------------------------------------------
class TframDevices : public TFrame
{
__published:	// IDE-managed Components
    TButton *Button1;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframDevices(TComponent* Owner);
    NODE_KEY node;
};
//---------------------------------------------------------------------------
extern PACKAGE TframDevices *framDevices;
//---------------------------------------------------------------------------
#endif
