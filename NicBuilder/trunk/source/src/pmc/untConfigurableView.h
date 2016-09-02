//---------------------------------------------------------------------------


#ifndef untConfigurableViewH
#define untConfigurableViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TframConfigurable : public TFrame
{
__published:	// IDE-managed Components
    TLabel *Label1;
private:	// User declarations
public:		// User declarations
    __fastcall TframConfigurable(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TframConfigurable *framConfigurable;
//---------------------------------------------------------------------------
#endif
