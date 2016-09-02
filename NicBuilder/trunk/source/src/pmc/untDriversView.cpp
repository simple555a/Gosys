//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TframDrivers *framDrivers = new TframDrivers(0);

//---------------------------------------------------------------------------
__fastcall TframDrivers::TframDrivers(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TframDrivers::Button1Click(TObject *Sender)
{
    CDriverItems *it = (CDriverItems*)Tag;
    TfrmAddDriver *frm = new TfrmAddDriver(Application->MainForm);
    frm->node = it->node;
    frm->ShowModal();
    delete frm;
}
//---------------------------------------------------------------------------
