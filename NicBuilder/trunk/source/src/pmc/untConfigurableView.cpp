//---------------------------------------------------------------------------

#include "precomp.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TframConfigurable *framConfigurable;
//---------------------------------------------------------------------------
__fastcall TframConfigurable::TframConfigurable(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
