//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframGroup *framGroup;

//---------------------------------------------------------------------------
__fastcall TframGroup::TframGroup(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TframGroup::btnRefreshClick(TObject *Sender)
{
    CConfigurableItem * item;
    item = (CConfigurableItem *)Tag;
    if(item){
        item->Refresh();
        UpdateView();
    }
}
//---------------------------------------------------------------------------

void __fastcall TframGroup::btnNewTagClick(TObject *Sender)
{
    CGroupItem * group;
    group = (CGroupItem*)Tag;
    group = dynamic_cast<CGroupItem*>(group);
    if(!group){
        return;
    }
/*
    CTagItem ti(&group->key);
    ti.UI_AddItem(Application->MainForm, 0);
*/
    framAddTag->framTag1->m_TagName = group->key;
    InvokeAction(ca_CreateSub);
}

//---------------------------------------------------------------------------
void __fastcall TframGroup::UpdateView()
{
    CGroupItem * group;
    group = (CGroupItem*)Tag;
    group = dynamic_cast<CGroupItem*>(group);
    if(!group){
        return;
    }

    RTK_CURSOR hGroup,hTag;
    PRTK_TAG pTag;
    PRTK_GROUP pGroup;
    hGroup = open_group_f(&group->key.node, &group->key.sname.group);
    if(!hGroup){
        return;
    }
    pGroup = (PRTK_GROUP)cursor_get_item(hGroup);

    edtCName->Text = pGroup->description;
    cmbPeriod->Text = pGroup->period;

    AnsiString s;
    s = "标签总数: ";
    s += cursor_get_subitem_count(hGroup);
    lblTagCount->Caption = s;

    s = "模拟量总数: ";
    int iAnalog=0, iSwitch=0;

    hTag = cursor_open_first_subitem(hGroup);
    pTag = (PRTK_TAG)cursor_get_item(hTag);
    while(pTag){
        if(get_value_type(pTag->s.Flags) != dt_bool){
            iAnalog++;
        }else{
            iSwitch++;
        }
        cursor_move_next(hTag);
        pTag = (PRTK_TAG)cursor_get_item(hTag);
    }
    close_handle(hTag);

    s += iAnalog;
    lblAnalog->Caption = s;
    s = "开关量总数: ";
    s += iSwitch;
    lblDigital->Caption = s;

    close_handle(hGroup);
}

void __fastcall TframGroup::Validate(PRTK_GROUP grp)
{
    strncpy(grp->description, edtCName->Text.c_str(), sizeof(grp->description));
	grp->description[sizeof(grp->description) - 1] = 0;
    grp->period = cmbPeriod->Text.ToInt();
}

void __fastcall TframGroup::btnSaveClick(TObject *Sender)
{
    InvokeAction(ca_Save);    
}
//---------------------------------------------------------------------------

