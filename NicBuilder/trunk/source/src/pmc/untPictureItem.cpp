//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)

CPictureItem::CPictureItem(PCNODE_KEY key)
{
    this->node = *key;
    m_iIcon = 13;
    m_sCaption = "¼à¿Ø»­Ãæ";
    m_SearchKey = "PICT";
    m_ClassName = "¼à¿Ø»­Ãæ";
    m_bExpandable = false;
    m_PropertyPage = framPictureView;
}

void CPictureItem::ReloadContents()
{
    RemoveChildren();
}

void CPictureItem::UI_UpdateView()
{
    framPictureView->UpdateView();
}
