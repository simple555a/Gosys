//---------------------------------------------------------------------------
#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PERFGRAP"
#pragma resource "*.dfm"
#pragma link "untTagView"
TframAddTag *framAddTag = new TframAddTag(0);

//---------------------------------------------------------------------------
__fastcall TframAddTag::TframAddTag(TComponent* Owner)
    : TFrame(Owner)
{
    /*
    framTag1->ClientHeight = framTag1->grpSwitchProperty_2->Top +
        framTag1->grpSwitchProperty_2->Height + 5;
    */
}
//---------------------------------------------------------------------------

#define err(s) lblHint->Caption = s

void TframAddTag::DoAdd()
{
    RTK_TIME t1, t2;

    AnsiString e;
    rtk_time_mark(&t1);
    lblHint->Caption = "正在添加 " + framTag1->edtTagName->Text + " ...";
    if(framTag1->ValidateView()){
        if(query_tag_f(&framTag1->m_TagName)){
            err(framTag1->edtTagName->Text + "已经存在.");
        }else{
            if(add_tags(1, &framTag1->m_TagName, &framTag1->m_Tag.s, 2000)){
                err(framTag1->edtTagName->Text + " 添加成功.");
            }else{
                err(framTag1->edtTagName->Text + " 添加失败.");
            }
        }
    }else{
        err(framTag1->m_Hint);
    }
    rtk_time_mark(&t2);
    AnsiString d="";
    d.cat_printf(" 花费时间: %.3f 毫秒", rtk_time_diff(&t2, &t1)*1000);
    lblHint->Caption += d;
}

void __fastcall TframAddTag::btnNextClick(TObject *Sender)
{
    //
    DoAdd();
}
//---------------------------------------------------------------------------

void __fastcall TframAddTag::btnHelpClick(TObject *Sender)
{
    //
}
//---------------------------------------------------------------------------


void __fastcall TframAddTag::framTag1cmbTypeChange(TObject *Sender)
{
  framTag1->cmbTypeChange(Sender);

}
//---------------------------------------------------------------------------

