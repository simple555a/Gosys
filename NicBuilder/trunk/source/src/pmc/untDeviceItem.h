//---------------------------------------------------------------------------

#ifndef untDeviceItemH
#define untDeviceItemH

#include "configurable.h"

class CDeviceItems : public CConfigurableItem
{
public:
    CDeviceItems(PCNODE_KEY node);
    virtual void UI_CreateSub();
    virtual void ReloadContents();
    NODE_KEY node;
};

class CDeviceItem : public CConfigurableItem
{
public:
    CDeviceItem(PCNODE_KEY node, DEVICE_INFO *info);
    virtual void ReloadContents();
    virtual void UI_UpdateView();
    virtual void UI_AddItem(TForm * parent, CConfigurableItem * _temp);
    virtual void Delete();
    virtual void Save();
public:
    NODE_KEY    node;
    DEVICE_INFO info;
    bool        active;
};

//---------------------------------------------------------------------------
#endif
