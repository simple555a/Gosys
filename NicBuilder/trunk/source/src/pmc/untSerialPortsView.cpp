//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop
#include <rtk_packet.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframSerialPorts *framSerialPorts = new TframSerialPorts(0);

//---------------------------------------------------------------------------
__fastcall TframSerialPorts::TframSerialPorts(TComponent* Owner)
    : TFrame(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TframSerialPorts::Button1Click(TObject *Sender)
{
    //
    CSerialPortItems *it;
    it = dynamic_cast<CSerialPortItems *>((CConfigurableItem*)Tag);
    if(!it){
        return;
    }
    RTK_ADDR addr;
    node_to_host(&it->key, &addr.host);
    addr.port = PORT_ALL_SERVER;
    char *cmd="reset_spac";
    Button1->Enabled = false;
    send_rtk_data(
        vbus_system,
        &addr,
        PT_System,
        cmd,
        strlen(cmd)+1
        );
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    Sleep(500);
    Screen->Cursor = Save_Cursor;
    Button1->Enabled = true;
}
//---------------------------------------------------------------------------
