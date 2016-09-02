//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframDeviceView *framDeviceView = new TframDeviceView(0);

//---------------------------------------------------------------------------
__fastcall TframDeviceView::TframDeviceView(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TframDeviceView::UpdateView()
{
    CDeviceItem * it;
    it = (CDeviceItem * )Tag;
    assert(it);
    edtAddress->Text = it->info.address;
    edtInit->Text = it->info.parameter;
    edtVendor->Text = it->info.v.description;
    edtType->Text = it->info.t.description;
}

#define err(s) Application->MessageBox((s).c_str(), Application->Title.c_str())

void __fastcall TframDeviceView::Button1Click(TObject *Sender)
{
    InvokeAction(ca_Save);    
}
//---------------------------------------------------------------------------

bool TframDeviceView::ValidateView(PDEVICE_INFO di)
{
    if(edtAddress->Text.Length() > sizeof(di->address)){
        err(AnsiString("设备地址不能超过") + sizeof(di->address) + "字节");
        return false;
    }
    if(edtInit->Text.Length() > sizeof(di->parameter)){
        err(AnsiString("设备初始化命令字不能超过") + sizeof(di->parameter) + "字节");
        return false;
    }
    strncpy(di->address, edtAddress->Text.c_str(), sizeof(di->address));
    di->address[sizeof(di->address) - 1] = 0;
    strncpy(di->parameter, edtInit->Text.c_str(), sizeof(di->parameter));
    di->parameter[sizeof(di->parameter) - 1] = 0;
    return true;
}
void __fastcall TframDeviceView::Button2Click(TObject *Sender)
{
    InvokeAction(ca_Help);    
}
//---------------------------------------------------------------------------

