//---------------------------------------------------------------------------
#ifndef untSerialPortItemH
#define untSerialPortItemH

#include "configurable.h"

//---------------------------------------------------------------------------
class CSerialPortItems : public CConfigurableItem
{
public:
    CSerialPortItems(PCNODE_KEY );
    virtual void ReloadContents();
    NODE_KEY key;
};

class CSerialPortItem : public CConfigurableItem
{
public:
    CSerialPortItem(PCNODE_KEY, int i);
    virtual void ReloadContents();
    virtual void UI_UpdateView();
    virtual void Save();
    NODE_KEY key;
    int m_PortNo;
    
    int m_baud;
    int m_stopbits;
    int m_checkmode;
    int m_databits;
};

//---------------------------------------------------------------------------
#endif
