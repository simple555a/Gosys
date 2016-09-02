//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "untTagItem"
#pragma link "untTagView"
#pragma resource "*.dfm"
TfrmAddTag *frmAddTag;

//---------------------------------------------------------------------------
__fastcall TfrmAddTag::TfrmAddTag(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTag::FormShow(TObject *Sender)
{
    framTag->UpdateView();
    lblHint->Caption = "";
}

#define err(s) lblHint->Caption = s

//---------------------------------------------------------------------------
void __fastcall TfrmAddTag::FormKeyPress(TObject *Sender, char &Key)
{
    if(Key == VK_ESCAPE){
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddTag::btnCloseClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

