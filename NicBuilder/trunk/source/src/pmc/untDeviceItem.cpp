//---------------------------------------------------------------------------


#include "precomp.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CDeviceItems::CDeviceItems(PCNODE_KEY node)
{
    this->node = *node;
    m_iIcon = 10;
    m_sCaption = "IO 设备";
    m_SearchKey = "DEVICES";
    m_ClassName = "IO 设备";
    if(!framDevices){
        framDevices = new TframDevices(0);
    }
    m_PropertyPage = framDevices;
    if(!framAddDevice){
        framAddDevice = new TframAddDevice(0);
    }
    m_CreateSubPage = framAddDevice;
}

void CDeviceItems::ReloadContents()
{
    DEVICE_INFO buf[64];
    CArgs args;
    __uint i;
    CDeviceItem * it;

    __uint count = sizeof(buf) / sizeof(buf[0]);

    count = discover_devices(&node, buf, &count, 1000);
    for(i=0; i<count; i++){
        it = new CDeviceItem(&node, &buf[i]);
        it->active = true;
        AddChild(0, it);
    }
}

void CDeviceItems::UI_CreateSub()
{
    framAddDevice->node = node;
    framAddDevice->InitView();
}

CDeviceItem::CDeviceItem(PCNODE_KEY node, DEVICE_INFO *di)
{
    this->node = *node;
    info = *di;
    m_bExpandable = false;
    if(info.flags & DF_Deleted){
        m_iIcon = 26;
    }else{
        if(info.flags & DF_Active){
            m_iIcon = 27;
        }else{
            m_iIcon = 28;
        }
    }
    m_sCaption = (char*)CDeviceName(info.k);
    m_ClassName = "IO 设备";
    m_SearchKey = m_sCaption;
    m_PropertyPage = framDeviceView;
    m_AddPage = framAddDevice;
    m_SupportedActions.insert(ca_Add);
    m_SupportedActions.insert(ca_Edit);
    m_SupportedActions.insert(ca_Delete);
    active = false;
}

void CDeviceItem::ReloadContents()
{
}

void CDeviceItem::UI_UpdateView()
{
    framDeviceView->UpdateView();
    if(info.flags & DF_Deleted){
        m_iIcon = 26;
    }else{
        if(info.flags & DF_Active){
            m_iIcon = 27;
        }else{
            m_iIcon = 28;
        }
    }
    m_ViewItem->ImageIndex = m_iIcon;
    m_ViewItem->SelectedIndex = m_iIcon;
}

void CDeviceItem::UI_AddItem(TForm * parent, CConfigurableItem *)
{
    framAddDevice->node = node;
    framAddDevice->InitView();
}

void CDeviceItem::Save()
{
    DEVICE_INFO di;
    di = info;
    if(framDeviceView->ValidateView(&di)){
        proxy_edit_device(&node, &di.k, di.address, di.parameter, 5000);
    }
}

void CDeviceItem::Delete()
{
    proxy_del_device(&node, &info.k, 5000);
}
