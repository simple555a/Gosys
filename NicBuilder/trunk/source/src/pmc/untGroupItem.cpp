//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
CGroupItem::CGroupItem(PCNODE_KEY node, PCGROUP_KEY group)
{
    ZeroMemory(&key, sizeof(key));
    key.node = *node;
    key.sname.group = *group;
    m_sCaption = (char *)CGroupName(*group);
    if(!framGroup){
        framGroup = new TframGroup(0);
    }
    m_ClassName = "数据分组";
    m_CreateSubPage = framAddTag;
    m_PropertyPage = framGroup;
    m_SearchKey = (char *)CGroupName(*group);
    m_AddPage = framAddGroup;
    m_SupportedActions.insert(ca_Add);
    m_SupportedActions.insert(ca_Edit);

    PRTK_GROUP p;
    RTK_CURSOR h;
    h = open_group_f(&key.node, &key.sname.group);
    p = (PRTK_GROUP)cursor_get_item(h);
    if(p){
        if(p->d.flags & GF_System){
            m_iIcon = 29;
        }else{
            m_iIcon = 1;
            m_SupportedActions.insert(ca_Delete);
        }
    }
    close_handle(h);
}

void CGroupItem::ReloadContents()
{
    RemoveChildren();
    // discover_tags(&key.node, &key.sname.group, 20480);
    RTK_CURSOR hNode, hGroup, hTag;
    PRTK_TAG pTag;
    CTagItem *item;
    hNode = open_node(&key.node);
    if(!hNode){
        return;
    }
    hGroup = open_group(hNode, &key.sname.group);
    if(!hGroup){
        close_handle(hNode);
        return;
    }
    hTag = cursor_open_first_subitem(hGroup);
    pTag = (PRTK_TAG)cursor_get_item(hTag);
    while( pTag ){
        key.sname.tag = pTag->key;
        item = new CTagItem(&key);
        AddChild(item);
        cursor_move_next(hTag);
        pTag = (PRTK_TAG)cursor_get_item(hTag);
    }
    close_handle(hNode);
    close_handle(hGroup);
    // m_ViewItem->AlphaSort();
}

void CGroupItem::UI_UpdateView()
{
    ((TframGroup*)m_PropertyPage)->UpdateView();
}

void CGroupItem::UI_AddItem(TForm *parent, CConfigurableItem * _template)
{
    framAddGroup->edtCName->Text = "";
    framAddGroup->node = key.node;
    framAddGroup->edtName->Text = "";
    framAddGroup->lblHint->Caption = "";
}

void CGroupItem::Save()
{
    RTK_GROUP g;
    TframGroup * frm;
    g.key = key.sname.group;
    frm = (TframGroup *)m_PropertyPage;
    frm->Validate(&g);
    edit_groups(1, &key.node, &g, 1000);
}

void CGroupItem::UI_CreateSub()
{
    framAddTag->framTag1->m_TagName = key;
    framAddTag->lblHint->Caption = "";
    framAddTag->framTag1->UpdateView();
}

void CGroupItem::Delete()
{
    drop_groups(1, &key.node, &key.sname.group, 2000);
}
