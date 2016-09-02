//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

#include "untDCSView.h"
#include "configurable.h"
#include "untDCSItem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TframDCSView *framDCSView;


//---------------------------------------------------------------------------
__fastcall TframDCSView::TframDCSView(TComponent* Owner)
    : TFrame(Owner)
{
    lblList->Caption = "";
}
//---------------------------------------------------------------------------


void TframDCSView::UpdateView()
{
    //TODO: Add your source code here
    lblList->Caption = "";
    AnsiString s;
    s = "";
    CConfigurableItem *item;
    CDCSItem *dcs;
    dcs = (CDCSItem*)Tag;
    if(dcs){
        item = (CConfigurableItem *)dcs->GetFirstChild();
        while(item){
            s += item->m_sCaption + "       正在运行\r\n";
            item = (CConfigurableItem *)dcs->GetNextChild(item);
        }
        lblList->Caption = s;
        edtName->Text = dcs->m_sCaption;
    }
}

void __fastcall TframDCSView::Button1Click(TObject *Sender)
{
    InvokeAction(ca_Refresh);
}
//---------------------------------------------------------------------------
void __fastcall TframDCSView::Button2Click(TObject *Sender)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    AnsiString sUrl;
    #ifndef NDEBUG
    sUrl = g_AppPath + "pmcd.exe -d";
    #else
    sUrl = g_AppPath + "pmcd.exe -s";
    #endif
    CreateProcess(
        NULL,
        sUrl.c_str(),
        0,0,0,0,0,g_AppPath.c_str(),
        &si,&pi
        );
}
//---------------------------------------------------------------------------
