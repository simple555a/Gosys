//---------------------------------------------------------------------
#include "precomp.h"
#pragma hdrstop


//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
    ProductName->Caption = Application->Title;
    Version->Caption = "3.0";
    Copyright->Caption = "CNCS";
    Comments->Caption = "�����к˶�������ϵͳ�������޹�˾";
}
//--------------------------------------------------------------------- 
