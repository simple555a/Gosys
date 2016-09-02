#include "precomp.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CConfigurableItem * g_CurItem;
int CConfigurableItem::m_iBaseIcon;
AnsiString g_AppPath;

CConfigurableItem::CConfigurableItem()
{
    m_bExpandable = true;
    m_iIcon = 0;
    if(!framConfigurable){
        framConfigurable = new TframConfigurable(0);
    }
    m_PropertyPage = framConfigurable;
    m_AddPage = 0;
    m_iRefreshCount = 0;
    state = cs_Editing;
    m_SupportedActions.clear();
    m_ViewItem = 0;
    m_SearchKey = "";
    m_ClassName = "配置项目";
}

void CConfigurableItem::UI_UpdateView()
{
}

void CConfigurableItem::Refresh()
{
    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    RemoveChildren();
    ReloadContents();
    m_iRefreshCount++;
    m_PropertyPage->Tag = (int)this;
    UI_UpdateView();
    Screen->Cursor = Save_Cursor;
    m_ViewItem->HasChildren = GetItems();
}

/*
    通知viewitem删除自己
*/
void CConfigurableItem::OnDeleteChild(CTreeNode * child)
{
    CConfigurableItem * ConfigItem = dynamic_cast<CConfigurableItem*>(child);
    assert(ConfigItem);
    TTreeNode *node;
    node = ConfigItem->m_ViewItem;
    if(node){
        node->Delete();
    }
    delete child;
}

void CConfigurableItem::OnUpdate()
{
}

void CConfigurableItem::UI_AddItem(TForm *parent, CConfigurableItem * _tempalte)
{
}

void CConfigurableItem::UI_CreateSub()
{
}

bool CConfigurableItem::Support(CConfigAction action)
{
    return m_SupportedActions.find(action) == m_SupportedActions.end()?
        false : true;
}

void CConfigurableItem::OnAddChild(CTreeNode * child)
{
    CConfigurableItem * ConfigItem = dynamic_cast<CConfigurableItem*>(child);
    assert(ConfigItem);
    TTreeNodes *nodes;
    TTreeNode *node,*NewNode;
    node = m_ViewItem;
    if(!node){
        return;
    }
    nodes = node->Owner;
    NewNode = nodes->AddChild(node, ConfigItem->m_sCaption);
    NewNode->HasChildren = ConfigItem->m_bExpandable;
    NewNode->ImageIndex = ConfigItem->m_iIcon;
    NewNode->SelectedIndex = ConfigItem->m_iIcon;
    // associate ConfigItem and NewNode with each other
    ConfigItem->m_ViewItem = NewNode;
    NewNode->Data = ConfigItem;
}

void CConfigurableItem::ReloadContents()
{
}

void CConfigurableItem::Delete()
{
}

void CConfigurableItem::Copy()
{
}

void CConfigurableItem::Paste()
{
}

bool CConfigurableItem::CanPaste()
{
    return false;
}

void CConfigurableItem::Save()
{
}

CConfigurableItem *CConfigurableItem::Search(const char * text)
{
    return 0;
}

struct CSearchContext{
    CConfigurableItem * Result;
    char * Key;
};

static __bool _SearchAction(CTreeNode * node, __uint context)
{
    CConfigurableItem *it;
    CSearchContext * ctx = (CSearchContext*)context;
    it = dynamic_cast<CConfigurableItem*>(node);
    assert(it);
    if(!it->m_SearchKey.AnsiCompareIC(ctx->Key)){
        ctx->Result = it;
        return false;
    }
    return true;
}

CConfigurableItem * CConfigurableItem::Search(const AnsiString & RelativeUrl)
{
    CArgs args;
    CSearchContext ctx;
    parse_arg_ex(RelativeUrl.c_str(), &args, ".");
    if(!args.argc){
        return 0;
    }
    CConfigurableItem *currentParent;
    currentParent = this;
    for(int i=0; i<args.argc; i++){
        ctx.Result = 0;
        ctx.Key = args.argv[i];
        currentParent->ForEach(_SearchAction, false, exclude, (__uint)&ctx);
        if(!ctx.Result){
            break;
        }
        currentParent = ctx.Result;
    }
    return ctx.Result;
}
