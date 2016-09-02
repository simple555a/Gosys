//---------------------------------------------------------------------------


#ifndef untDriversViewH
#define untDriversViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TframDrivers : public TFrame
{
__published:	// IDE-managed Components
    TButton *Button1;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TframDrivers(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TframDrivers *framDrivers;
//---------------------------------------------------------------------------
#endif
