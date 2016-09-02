//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
    : TForm(Owner)
{
    //
}
//---------------------------------------------------------------------------


void __fastcall TForm3::lstDropDown(TObject *Sender)
{
    // update drop down list
    int i;
    char drv[4];
    UINT type;
    lst->Items->Clear();
    for(i=0; i<26; i++){
        sprintf(drv, "%c:\\", 'A'+i);
        type=GetDriveType(drv);
        // search removeable drive only
        if(type!=DRIVE_REMOVABLE && CheckBox1->Checked){
            continue;
        }
        if(type == DRIVE_NO_ROOT_DIR){
            continue;
        }
        if(type==DRIVE_CDROM){
            continue;
        }
        lst->Items->Add(drv);
    }
}
//---------------------------------------------------------------------------
