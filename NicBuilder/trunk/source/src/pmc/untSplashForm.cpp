//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSplash *frmSplash;
//---------------------------------------------------------------------------
__fastcall TfrmSplash::TfrmSplash(TComponent* Owner)
    : TForm(Owner)
{
    try{
        Image1->Picture->LoadFromFile(g_AppPath + "res/pmc.bmp");
        Image1->AutoSize = true;
    }catch(...){
    }
    AutoSize = true;
    Position = poScreenCenter;
}
//---------------------------------------------------------------------------
