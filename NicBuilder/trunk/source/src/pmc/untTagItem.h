//---------------------------------------------------------------------------

#ifndef untTagItemH
#define untTagItemH

#include "configurable.h"

//---------------------------------------------------------------------------
class CTagItem : public CConfigurableItem
{
public:
    CTagItem(PCTAG_NAME);
    virtual void UI_UpdateView();
    virtual void UI_AddItem(TForm *parent, CConfigurableItem * _template);
    virtual void Delete();
    virtual void Save();
    TAG_NAME key;
};

#endif
