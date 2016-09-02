//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAddGroup *frmAddGroup;
static DRIVER_INFO drivers[128];

//---------------------------------------------------------------------------
__fastcall TfrmAddGroup::TfrmAddGroup(TComponent* Owner)
    : TForm(Owner)
{
    ZeroMemory(&name, sizeof(name));
}

//---------------------------------------------------------------------------
void __fastcall TfrmAddGroup::FormShow(TObject *Sender)
{
    __uint count, maxc, i;
    maxc = 1024;
    count = discover_drivers(&name.node, drivers, &maxc, 1000);
    cmbDevice->Items->Clear();
    if(!count){
        lblHint->Caption = "�ڵ���û�л����������.���������������";
        btnSave->Enabled = false;
    }else{
        lblHint->Caption = "";
    }
    for(i=0; i<count; i++){
        cmbDevice->Items->Add(drivers[i].description);
    }
    cmbDevice->ItemIndex = 0;
    edtName->Text = "";
    edtCName->Text = "";
}
//---------------------------------------------------------------------------

bool __fastcall TfrmAddGroup::ValidateData(PRTK_GROUP grp)
{
    if(edtName->Text.Length() > sizeof(GROUP_KEY)){
        m_sHint = AnsiString("�ؼ��ֲ��ܳ���") + sizeof(GROUP_KEY) + "���ֽ�";
        return false;
    }
    grp->key = CGroupName(edtName->Text.c_str());
    if(edtName->Text.Length() > sizeof(grp->description)-1){
        m_sHint = AnsiString("���������ܳ���") + (sizeof(grp->description) - 1) + "���ֽ�";
        return false;
    }
    strncpy(grp->description, edtCName->Text.c_str(), sizeof(grp->description));
    grp->description[sizeof(grp->description) - 1] = 0;
    grp->npu = drivers[cmbDevice->ItemIndex].name;
    return true;
}
void __fastcall TfrmAddGroup::btnSaveClick(TObject *Sender)
{
    RTK_GROUP grp;
    if(!ValidateData(&grp)){
        MessageBox(Handle,
        m_sHint.c_str(),
        "����",
        MB_OK);
        return;
    }
    btnSave->Enabled = false;
    if( add_groups(1, &name.node, &grp, 1000) ){
        lblHint->Caption = AnsiString((char *)CGroupName(grp.key)) + " ��ӳɹ�";
    }else{
        lblHint->Caption = AnsiString((char *)CGroupName(grp.key)) + " ���ʧ��";
    }
    btnSave->Enabled = true;
    edtName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddGroup::Button1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddGroup::FormKeyPress(TObject *Sender, char &Key)
{
    if(Key == VK_ESCAPE){
        Close();
    }
}
//---------------------------------------------------------------------------

