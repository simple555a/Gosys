//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframDriverView *framDriverView=new TframDriverView(0);

//---------------------------------------------------------------------------
__fastcall TframDriverView::TframDriverView(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TframDriverView::Button1Click(TObject *Sender)
{
    InvokeAction(ca_Save);    
}
//---------------------------------------------------------------------------
void __fastcall TframDriverView::UpdateView()
{
    CDriverItem * it;
    it = (CDriverItem*)Tag;
    assert(it);
    edtVendor->Text = it->info.vendor;
    edtVersion->Text =
        AnsiString((it->info.version>>48) & 0xffff)
        +"."+
        AnsiString((it->info.version>>32) & 0xffff)
        +"."+
        AnsiString((it->info.version>>16) & 0xffff)
        +"."+
        AnsiString((it->info.version>>0) & 0xffff);
    edtInit->Text = it->info.parameter;
}
