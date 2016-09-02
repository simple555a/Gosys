//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)

CDCSItem::CDCSItem()
{
    char buf[256];
    GetPrivateProfileString(
        "pmc",
        "name",
        "NicBuilder 工业监控系统",
        buf,
        sizeof(buf),
        get_config_file()
        );
    m_iIcon = 25;
    m_sCaption = buf;
    if(!framDCSView){
        framDCSView = new TframDCSView(0);
        framDCSView->Tag = (int)this;
    }
    m_PropertyPage = framDCSView;
    m_SearchKey = "DCS";
    m_ClassName = "NicBuilder 工业监控系统";
}

void CDCSItem::ReloadContents()
{
    RTK_CURSOR hNode;
    PRTK_NODE  pNode;
    CNodeItem  *item;

    RemoveChildren();
    // discover all running nodes
    // discover_nodes(1000);
    hNode = open_first_node();
    pNode = (PRTK_NODE)cursor_get_item(hNode);
    while( pNode ){
        item = new CNodeItem(&pNode->key);
        AddChild(item);
        cursor_move_next(hNode);
        pNode = (PRTK_NODE)cursor_get_item(hNode);
    }
    close_handle(hNode);
}

void CDCSItem::UI_UpdateView()
{
    framDCSView->UpdateView();
}

void CDCSItem::Save()
{
    m_sCaption = framDCSView->edtName->Text;
    WritePrivateProfileString(
        "pmc",
        "name",
        m_sCaption.c_str(),
        get_config_file()
        );
    m_ViewItem->Text = m_sCaption;
}
