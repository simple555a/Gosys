//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframSerialPort *framSerialPort;

//---------------------------------------------------------------------------
__fastcall TframSerialPort::TframSerialPort(TComponent* Owner)
    : TFrame(Owner)
{
    rbMark->Tag = (int)"m";
    rbNone->Tag = (int)"n";
    rbOdd->Tag = (int)"o";
    rbEven->Tag = (int)"e";
    rbBlank->Tag = (int)"b";

    rb5->Tag = (int)"5";
    rb6->Tag = (int)"6";
    rb7->Tag = (int)"7";
    rb8->Tag = (int)"8";

    rb1->Tag = (int)"1";
    rb2->Tag = (int)"2";
    rb1_5->Tag = (int)"1.5"; 
}

void __fastcall TframSerialPort::UpdataView()
{
    CSerialPortItem * it;
    it = (CSerialPortItem*)Tag;
    switch(it->m_baud){
    case 4800:
        cmbBaud->ItemIndex = 0;
    break;
    case 9600:
        cmbBaud->ItemIndex = 1;
    break;
    case 19200:
        cmbBaud->ItemIndex = 2;
    break;
    case 38400:
        cmbBaud->ItemIndex = 3;
    break;
    case 57600:
        cmbBaud->ItemIndex = 4;
    break;
    case 115200:
        cmbBaud->ItemIndex = 5;
    break;
    default:
        cmbBaud->ItemIndex = 4;
    break;
    }

    switch(it->m_checkmode){
    case 'm':
        rbMark->Checked = true;
    break;
    case 'o':
        rbOdd->Checked = true;
    break;
    case 'e':
        rbEven->Checked = true;
    break;
    case 'b':
        rbBlank->Checked = true;
    break;
    case 'n':
        rbNone->Checked = true;
    break;
    }

    switch(it->m_databits){
    case 5:
        rb5->Checked = true;
    break;
    case 6:
        rb6->Checked = true;
    break;
    case 7:
        rb7->Checked = true;
    break;
    case 8:
        rb8->Checked = true;
    break;
    }

    switch(it->m_stopbits){
    case 1:
        rb1->Checked = true;
    break;
    case 2:
        rb2->Checked = true;
    break;
    case 3:
        rb1_5->Checked = true;
    break;
    }
}

AnsiString TframSerialPort::Validate()
{
    TComponent * comp;
    TRadioButton * rb;

    int i;
    AnsiString s;
    s = cmbBaud->Text + ",";
    for(i=0; i<ComponentCount; i++){
        rb = dynamic_cast<TRadioButton*>(Components[i]);
        if(rb && rb->Checked){
            s += (char *)rb->Tag;
            s += ",";
        }
    }
    if(s[s.Length() - 1] == ','){
        s[s.Length() - 1] = 0;
    }
    return s;
}
void __fastcall TframSerialPort::btnSaveClick(TObject *Sender)
{
    InvokeAction(ca_Save);    
}
//---------------------------------------------------------------------------

