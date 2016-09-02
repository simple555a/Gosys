//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

#pragma package(smart_init)

CNodeItem::CNodeItem(PCNODE_KEY key)
{
    this->key = *key;
    m_iIcon = 2;
    m_sCaption = (char *)CNodeName(*key);
    m_SearchKey = m_sCaption;
    m_ClassName = "功能节点";
    if(!framNodeView){
        framNodeView = new TframNodeView(0);
    }
    m_PropertyPage = framNodeView;
}

void CNodeItem::ReloadContents()
{
    CConfigurableItem *item;
    item = (CConfigurableItem *)new CDBItem(&key);
    AddChild(0, item);
    // item = (CConfigurableItem *)new CPictureItem(&key);
    // AddChild(0, item);
    // item = (CConfigurableItem *)new CHislogItem(&key);
    // AddChild(0, item);
    item = (CConfigurableItem *)new CSerialPortItems(&key);
    AddChild(0, item);
    item = (CConfigurableItem *)new CDeviceItems(&key);
    AddChild(0, item);
}

CDBItem::CDBItem(PCNODE_KEY key)
{
    this->key = *key;
    m_iIcon = 0;
    m_sCaption = "实时数据服务";
    if(!framDB){
        framDB = new TframDB(0);
        framDB->key = *key;
    }
    m_PropertyPage = framDB;
    m_CreateSubPage = framAddGroup;
    m_SearchKey = "DB";
    m_SupportedActions.insert(ca_CreateSub);
}

void CDBItem::ReloadContents()
{
    CConfigurableItem *item;
    // discover_groups(&key, 2000);
    RTK_CURSOR hNode, hGroup;
    PRTK_GROUP pGroup;

    hNode = open_node(&key);
    if(!hNode){
        return;
    }
    hGroup = cursor_open_first_subitem(hNode);
    pGroup = (PRTK_GROUP)cursor_get_item(hGroup);
    while(pGroup){
        item = new CGroupItem(&key, &pGroup->key);
        AddChild(item);
        cursor_move_next(hGroup);
        pGroup = (PRTK_GROUP)cursor_get_item(hGroup);
    }
    close_handle(hGroup);
    close_handle(hNode);
}

void CDBItem::UI_UpdateView()
{
    framDB->key = key;
    ((TframDB*)m_PropertyPage)->UpdateView();
}

void CDBItem::UI_CreateSub()
{
    framAddGroup->edtCName->Text = "";
    framAddGroup->node = key;
    framAddGroup->edtName->Text = "";
    framAddGroup->lblHint->Caption = "";
}
