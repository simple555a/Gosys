//---------------------------------------------------------------------------

#ifndef untDCSItemH
#define untDCSItemH

#include "configurable.h"

class CDCSItem : public CConfigurableItem
{
public:
    CDCSItem();
    virtual void ReloadContents();
    virtual void UI_UpdateView();
    virtual void Save();
};

extern CDCSItem g_DCS;

//---------------------------------------------------------------------------
#endif
