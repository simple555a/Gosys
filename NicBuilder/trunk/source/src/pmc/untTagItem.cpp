//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)

static int _getTypeIcon(int type)
{
	int m_iIcon;
	switch(type){
    case dt_real4:
    case dt_real8:
        m_iIcon = 12;
        break;
    case dt_bool:
         m_iIcon = 3;
         break;
    case dt_int8:
    case dt_int16:
    case dt_int32:
    case dt_uint8:
    case dt_uint16:
    case dt_uint32:
    case dt_int64:
    case dt_uint64:
         m_iIcon = 31;
         break;
    case dt_date:
         m_iIcon = 33;
         break;
    default:
    	assert(0);
    }
    return m_iIcon;
}

CTagItem::CTagItem(PCTAG_NAME name)
{
	key = *name;
	PRTK_TAG pTag;
	m_sCaption = (char *)CTagName(key.sname.tag);
	pTag =query_tag_f(&key);
	if(pTag){
		m_iIcon = _getTypeIcon(get_value_type(pTag->s.Flags));
	}
	m_bExpandable = false;
	if(!framTag){
		framTag = new TframTag(0);
	}
	m_PropertyPage = framTag;
	m_AddPage = framAddTag;
	m_SearchKey = (char*)CTagName(key.sname.tag);
	m_ClassName = "Êý¾Ý±êÇ©";
	if( !(pTag->s.Flags & TF_System) ){
		m_SupportedActions.insert(ca_Delete);
		m_SupportedActions.insert(ca_Add);
	}
	m_SupportedActions.insert(ca_Edit);
}

void CTagItem::UI_UpdateView()
{
	TframTag * frame = (TframTag*)m_PropertyPage;
	frame->setTag(&key);
	PRTK_TAG pTag;
	pTag = query_tag_f(&key);
    m_iIcon = _getTypeIcon(get_value_type(pTag->s.Flags));
	m_ViewItem->ImageIndex = m_iIcon;
	m_ViewItem->SelectedIndex = m_iIcon;
	frame->UpdateView();
}

void CTagItem::UI_AddItem(TForm *parent, CConfigurableItem * _template)
{
	framAddTag->framTag1->setTag(&key);
	framAddTag->lblHint->Caption = "";
	framAddTag->framTag1->UpdateView();
}

void CTagItem::Delete()
{
	drop_tags(1, &key, 1000);
}

void CTagItem::Save()
{
	TframTag* framTag = (TframTag*)m_PropertyPage;
	if(framTag->ValidateView()){
		edit_tags(1, &framTag->m_TagName, &framTag->m_Tag.s, 1000);
		UI_UpdateView();
	}
}
