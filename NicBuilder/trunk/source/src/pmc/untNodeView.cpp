//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop
#include <rtk_packet.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframNodeView *framNodeView;

//---------------------------------------------------------------------------
__fastcall TframNodeView::TframNodeView(TComponent* Owner)
    : TFrame(Owner)
{
    edtDownload->Text = "";
    edtUpload->Text = "";
}

//---------------------------------------------------------------------------
void __fastcall TframNodeView::Button1Click(TObject *Sender)
{
    CNodeItem *it;
    it = (CNodeItem*)Tag;
    RTK_ADDR dest;
    node_to_host(&it->key, &dest.host);
    dest.port = PORT_ALL_SERVER;
    send_rtk_data(
        vbus_system,
        &dest,
        PT_System,
        "exit",
        5);
}
//---------------------------------------------------------------------------

void __fastcall TframNodeView::Button2Click(TObject *Sender)
{
    CNodeItem *it;
    it = (CNodeItem*)Tag;
    RTK_ADDR dest;
    node_to_host(&it->key, &dest.host);
    dest.port = PORT_ALL_SERVER;
    send_rtk_data(
        vbus_system,
        &dest,
        PT_System,
        "restart",
        8);
}
//---------------------------------------------------------------------------

void __fastcall TframNodeView::Button3Click(TObject *Sender)
{
    CNodeItem *it;
    it = (CNodeItem*)Tag;
    RTK_ADDR dest;
    node_to_host(&it->key, &dest.host);
    dest.port = PORT_ALL_SERVER;
    send_rtk_data(
        vbus_system,
        &dest,
        PT_System,
        "pause",
        6);
}
//---------------------------------------------------------------------------

void __fastcall TframNodeView::btnPickSaveClick(TObject *Sender)
{
    ofd->Options >> ofFileMustExist;
    if( !ofd->Execute() ){
        return;
    }
    edtDownload->Text = ofd->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TframNodeView::btnPickReadClick(TObject *Sender)
{
    ofd->Options << ofFileMustExist;
    if( !ofd->Execute() ){
        return;
    }
    edtUpload->Text = ofd->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TframNodeView::btnStartSaveClick(TObject *Sender)
{
    CNodeItem * it;
    it = dynamic_cast<CNodeItem*>((CConfigurableItem *)Tag);
    if(!it){
        return;
    }
    if(edtDownload->Text == ""){
        return;
    }
    TfrmProgress * frm;
    frm = new TfrmProgress(Application->MainForm);
    if(frm){
        frm->Caption = "��������:";
        frm->Caption += (char*)CNodeName(it->key);
        frm->node = it->key;
        frm->file = edtDownload->Text;
        frm->bDownload = true;
        frm->ShowModal();
        delete frm;
    }
}
//---------------------------------------------------------------------------

void __fastcall TframNodeView::btnStartReadClick(TObject *Sender)
{
    CNodeItem * it;
    it = dynamic_cast<CNodeItem*>((CConfigurableItem *)Tag);
    if(!it){
        return;
    }
    if(edtUpload->Text == ""){
        return;
    }
    int res;
    AnsiString Hint = "ȷ��Ҫ��\r\n";
    Hint += edtUpload->Text + "\r\n";
    Hint += "�е���������Ϣ��\r\n";
    Hint += (char*)CNodeName(it->key);
    Hint += "?\r\n����:�ڵ���ԭ�е�������Ϣ����ʧ�������������Ϣ���ڽڵ���������Ч.";
    res = Application->MessageBox(
        Hint.c_str(),
        Application->Title.c_str(),
        MB_YESNO
        );
    if(res != IDYES){
        return;
    }
    TfrmProgress * frm;
    frm = new TfrmProgress(Application->MainForm);
    if(frm){
        frm->Caption = "��������:";
        frm->Caption += (char*)CNodeName(it->key);
        frm->node = it->key;
        frm->file = edtUpload->Text;
        frm->bDownload = false;
        frm->ShowModal();
        delete frm;
    }
}
//---------------------------------------------------------------------------

