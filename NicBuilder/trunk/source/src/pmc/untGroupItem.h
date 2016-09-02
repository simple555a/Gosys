//---------------------------------------------------------------------------

#ifndef untGroupItemH
#define untGroupItemH
//---------------------------------------------------------------------------

#include "configurable.h"

class CGroupItem : public CConfigurableItem
{
public:
    CGroupItem(PCNODE_KEY node, PCGROUP_KEY group);
    virtual void UI_UpdateView();
    virtual void UI_AddItem(TForm *parent, CConfigurableItem * _template = 0);
    virtual void UI_CreateSub();
    virtual void ReloadContents();
    virtual void Delete();
    virtual void Save();
    TAG_NAME key;
};

#endif
