//---------------------------------------------------------------------------

#ifndef configurableH
#define configurableH

#pragma warn -pck

#include <windows.h>
#include "untTreeNode.h"
#include "controls.hpp"
#include "forms.hpp"
#include "comctrls.hpp"
#include <pmc.h>
#include <set>

enum CConfigState{
    cs_Creation,
    cs_Editing,
    cs_Browsing,
};

enum CConfigAction{
    ca_Add=0,
    ca_Edit,
    ca_Save,
    ca_Cut,
    ca_Delete,
    ca_Copy,
    ca_Paste,
    ca_Search,
    ca_CreateSub,
    ca_Refresh,
    ca_Help,
};

class CConfigurableItem : public CTreeNode
{
	/* view elements */
public:
    CConfigurableItem();

    bool Support(CConfigAction action);
    CConfigurableItem * Search(const AnsiString & RelativeUrl);

	AnsiString m_sCaption;
    AnsiString m_SearchKey;
    AnsiString m_ClassName;
    AnsiString m_sHelpTopic;
    bool  m_bExpandable;
    int   m_iRefreshCount;

	TWinControl *m_PropertyPage;
    TWinControl *m_AddPage;
    TWinControl *m_CreateSubPage;
    TTreeNode   *m_ViewItem;

    virtual void Refresh();
    virtual void ReloadContents();
    virtual void UI_UpdateView();
    virtual void UI_AddItem(TForm *parent, CConfigurableItem * _template = 0);
    virtual void UI_CreateSub();
    virtual void Delete();
    virtual void Copy();
    virtual void Paste();
    virtual bool CanPaste();
    virtual void Save();
    virtual CConfigurableItem *Search(const char * text);
protected:
    std::set<CConfigAction> m_SupportedActions;
private:
	static int m_iBaseIcon;

    // overide of CTreeNode
    virtual void OnAddChild(CTreeNode * NewChild);
    virtual void OnDeleteChild(CTreeNode * child);
    // overidable
    virtual void OnUpdate();
public:
	int m_iIcon;
    CConfigState state;
};

extern CConfigurableItem * g_CurItem;
extern AnsiString g_AppPath;

void ConfigChanged(__uint event_type, ...);

void InvokeAction(CConfigAction act, ...);

//---------------------------------------------------------------------------
#endif
