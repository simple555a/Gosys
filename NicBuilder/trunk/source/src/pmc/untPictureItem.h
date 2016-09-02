//---------------------------------------------------------------------------

#ifndef untPictureItemH
#define untPictureItemH

#include "configurable.h"

class CPictureItem : public CConfigurableItem
{
public:
    CPictureItem(PCNODE_KEY node);
    virtual void UI_UpdateView();
    virtual void ReloadContents();
    NODE_KEY node;
};

//---------------------------------------------------------------------------
#endif
