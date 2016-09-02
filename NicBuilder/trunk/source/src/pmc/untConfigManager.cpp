#include "precomp.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#pragma package(smart_init)

static void _cdecl _OnAddGroup(PCTAG_NAME name);

static void _cdecl _OnAddNode(PCNODE_KEY k)
{
    AnsiString Url;
    CConfigurableItem * it;
    RTK_CURSOR hNode;
    
    Url = ToUrl(k);
    it = g_DCS.Search(Url);
    if(it){
        it->Refresh();
    }else{
        it = new CNodeItem(k);
        g_DCS.AddChild(it);
        it->Refresh();
    }
    g_DCS.UI_UpdateView();
}

static void _cdecl _OnDropNode(PCNODE_KEY k)
{
    AnsiString Url;
    CConfigurableItem * it;
    RTK_CURSOR hNode;

    Url = ToUrl(k);
    it = g_DCS.Search(Url);
    if(it){
        it->GetParent()->RemoveChild(it);
    }
    g_DCS.UI_UpdateView();
}

static void _cdecl _OnAddEditTag(PCTAG_NAME name)
{
    AnsiString Url;
    CConfigurableItem *it;
    CGroupItem * gi;
    CTagItem * ti;

    Url = ToUrl(&name->node, &name->sname.group);
    it = g_DCS.Search(Url);
    gi = dynamic_cast<CGroupItem*>(it);
    if(!gi){
        _OnAddGroup(name);
        it = g_DCS.Search(Url);
        gi = dynamic_cast<CGroupItem*>(it);
    }
    if(!gi){
        return;
    }
    Url = ToUrl(name);
    it = g_DCS.Search(Url);
    ti = dynamic_cast<CTagItem*>(it);
    if(ti){
        // already exists
        return;
    }
    ti = new CTagItem(name);
    gi->AddChild(ti);
}

static void _cdecl _OnDropTag(PCTAG_NAME name)
{
    AnsiString Url;
    Url = ToUrl(name);
    CConfigurableItem * item;
    CTagItem *ti;
    item = g_DCS.Search(Url);
    if(!item){
        return;
    }
    ti = dynamic_cast<CTagItem*>(item);
    assert(ti);
    ti->GetParent()->RemoveChild(ti);
}

static void _cdecl _OnAddGroup(PCTAG_NAME name)
{
    AnsiString Url;
    CConfigurableItem *it;
    CGroupItem * gi;
    CDBItem *ni;

    Url = ToUrl(&name->node, &name->sname.group);
    it = g_DCS.Search(Url);
    gi = dynamic_cast<CGroupItem*>(it);
    if(gi){
        return;
    }
    Url = ToUrl(&name->node) + ".DB";
    it = g_DCS.Search(Url);
    ni = dynamic_cast<CDBItem*>(it);
    if(!ni){
        return;
    }
    gi = new CGroupItem(&name->node, &name->sname.group);
    ni->AddChild(gi);
}

static void _cdecl _OnModifyGroup(PCTAG_NAME name)
{
    AnsiString Url;
    CConfigurableItem *it;
    CGroupItem * gi;
    CDBItem *ni;

    Url = ToUrl(&name->node, &name->sname.group);
    it = g_DCS.Search(Url);
    gi = dynamic_cast<CGroupItem*>(it);
    if(!gi){
        _OnAddGroup(name);
        return;
    }
    gi->UI_UpdateView();
}

static void _cdecl _OnDropGroup(PCTAG_NAME name)
{
    AnsiString Url;
    Url = ToUrl(&name->node, &name->sname.group);
    CConfigurableItem * item;
    CGroupItem *gi;
    item = g_DCS.Search(Url);
    if(!item){
        return;
    }
    gi = dynamic_cast<CGroupItem*>(item);
    assert(gi);
    gi->GetParent()->RemoveChild(gi);
}

static void _cdecl _OnAddDevice(PCNODE_KEY n, PDEVICE_INFO di)
{
    AnsiString Url;
    Url = ToUrl(n) + ".DEVICES";
    CDeviceItems * ditems;
    ditems = dynamic_cast<CDeviceItems*>(g_DCS.Search(Url));
    if(!ditems){
        return;
    }
    CDeviceItem * ditem;
    Url = (char*)CDeviceName(di->k);
    ditem = dynamic_cast<CDeviceItem*>(ditems->Search(Url));
    if(ditem){
        ditem->info = *di;
    }else{
        ditem = new CDeviceItem(n, di);
        if(ditem){
            ditems->AddChild(ditem);
        }
    }
}

static void _cdecl _OnDelDevice(PCNODE_KEY n, PCDEVICE_KEY d)
{
    AnsiString Url;
    Url = ToUrl(n, d);
    CDeviceItem *ditem;
    ditem = dynamic_cast<CDeviceItem*>(g_DCS.Search(Url));
    if(ditem){
        // ditem->GetParent()->RemoveChild(ditem);
        ditem->info.flags |= DF_Deleted;
        ditem->UI_UpdateView();
    }
}

void __fastcall InitConfigManager()
{
    g_ProxyDispatchTable.OnAddTag = _OnAddEditTag;
    g_ProxyDispatchTable.OnDropTag = _OnDropTag;
    g_ProxyDispatchTable.OnTagModified = _OnAddEditTag;
    g_ProxyDispatchTable.OnAddGroup = _OnAddGroup;
    g_ProxyDispatchTable.OnGroupModified = _OnModifyGroup;
    g_ProxyDispatchTable.OnDropGroup = _OnDropGroup;
    g_ProxyDispatchTable.OnAddNode = _OnAddNode;
    g_ProxyDispatchTable.OnDropNode = _OnDropNode;
    g_ProxyDispatchTable.OnAddDevice = _OnAddDevice;
    g_ProxyDispatchTable.OnDeviceModified = _OnAddDevice;
    g_ProxyDispatchTable.OnDropDevice = _OnDelDevice;
}
