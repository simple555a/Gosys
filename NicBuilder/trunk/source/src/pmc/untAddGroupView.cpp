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
        lblHint->Caption = "错误: 组名字不能为空";
        return;
    }
    if(!is_valid_name(edtName->Text.c_str())){
        lblHint->Caption = "错误:组名称只能包含字母、数字和下划线";
        return;
    }
    if(edtName->Text.Length() > sizeof(GROUP_KEY)){
        e = AnsiString("错误: 组名不能超过 ") + IntToStr(sizeof(GROUP_KEY)) + " 个字节";
        err(e);
        return;
    }
    if(edtCName->Text.Length() > sizeof(g.description)){
        err(AnsiString("错误: 中文名不能超过")+IntToStr(sizeof(g.description))+"个字节");
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
        err(AnsiString((char*)CNodeName(node)) + "." + edtName->Text + "添加成功");
    }else{
        err(AnsiString((char*)CNodeName(node)) + "." + edtName->Text + "添加失败");
    }
}


