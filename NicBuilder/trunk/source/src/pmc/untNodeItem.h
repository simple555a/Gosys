//---------------------------------------------------------------------------

#ifndef untNodeItemH
#define untNodeItemH

#include "configurable.h"

//---------------------------------------------------------------------------

class CNodeItem : public CConfigurableItem
{
public:
    CNodeItem(PCNODE_KEY key);
    virtual void ReloadContents();
    NODE_KEY key;
};

class CDBItem : public CConfigurableItem
{
public:
    CDBItem(PCNODE_KEY);
    virtual void UI_UpdateView();
    virtual void UI_CreateSub();
    virtual void ReloadContents();
    NODE_KEY key;
};

#endif
