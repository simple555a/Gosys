//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframAddGroup *framAddGroup;

//---------------------------------------------------------------------------
__fastcall TframAddGroup::TframAddGroup(TComponent* Owner)
    : TFrame(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TframAddGroup::btnNextClick(TObject *Sender)
{
    //
    DoAdd();  
}
//---------------------------------------------------------------------------

void __fastcall TframAddGroup::btnHelpClick(TObject *Sender)
{
    //
}

//---------------------------------------------------------------------------
void TframAddGroup::Show()
{
    lblHint->Caption = "";
    edtName->Text = "";
    edtCName->Text = "";
}

#define err(s) lblHint->Caption = s

void TframAddGroup::DoAdd()
{
    AnsiString e;
    RTK_GROUP g;
    memset(&g, 0, sizeof(g));
    if(edtName->Text == ""){
        lblHint->Caption = "����: �����ֲ���Ϊ��";
        return;
    }
    if(!is_valid_name(edtName->Text.c_str())){
        lblHint->Caption = "����:������ֻ�ܰ�����ĸ�����ֺ��»���";
        return;
    }
    if(edtName->Text.Length() > sizeof(GROUP_KEY)){
        e = AnsiString("����: �������ܳ��� ") + IntToStr(sizeof(GROUP_KEY)) + " ���ֽ�";
        err(e);
        return;
    }
    if(edtCName->Text.Length() > sizeof(g.description)){
        err(AnsiString("����: ���������ܳ���")+IntToStr(sizeof(g.description))+"���ֽ�");
        return;
    }
    strncpy(g.description, edtCName->Text.c_str(), sizeof(g.description));
    g.description[sizeof(g.description) - 1] = 0;
    g.key = (GROUP_KEY)CGroupName(edtName->Text.c_str());
    g.period = cmbPeriod->Text.ToInt();
    if(add_groups(
        1,
        &node,
        &g,
        5000)){
        err(AnsiString((char*)CNodeName(node)) + "." + edtName->Text + "��ӳɹ�");
    }else{
        err(AnsiString((char*)CNodeName(node)) + "." + edtName->Text + "���ʧ��");
    }
}


