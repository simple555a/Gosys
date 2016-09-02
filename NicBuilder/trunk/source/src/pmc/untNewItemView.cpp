//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmNewItem *frmNewItem;

//---------------------------------------------------------------------------
__fastcall TfrmNewItem::TfrmNewItem(TComponent* Owner)
    : TForm(Owner)
{
    frame = 0;
}
//---------------------------------------------------------------------------

void TfrmNewItem::ShowWith(TWinControl *f)
{
    frame = f;
    InsertComponent(frame);
    frame->Parent = this;
    ShowModal();
}

void __fastcall TfrmNewItem::FormDestroy(TObject *Sender)
{
    if(frame){
        RemoveComponent(frame);
        frame->Parent = 0;
    }
    frame = 0;
}
//---------------------------------------------------------------------------
