//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TframDevices *framDevices;

//---------------------------------------------------------------------------
__fastcall TframDevices::TframDevices(TComponent* Owner)
    : TFrame(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TframDevices::Button1Click(TObject *Sender)
{
    CDeviceItems * it;
    it = dynamic_cast<CDeviceItems*>((CConfigurableItem*)Tag);
    assert(it);
    framAddDevice->node = it->node;
    framAddDevice->InitView(); 
    InvokeAction(ca_CreateSub);    
}
//---------------------------------------------------------------------------
