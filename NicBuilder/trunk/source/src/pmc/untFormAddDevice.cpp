//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAddDevice *frmAddDevice;
//---------------------------------------------------------------------------
__fastcall TfrmAddDevice::TfrmAddDevice(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDevice::FormCreate(TObject *Sender)
{
    AnsiString f;
    try{
        f = g_AppPath + "res\\splitter_long.bmp";
        Image1->Picture->LoadFromFile(f);
    }catch(...){
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddDevice::Button2Click(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
