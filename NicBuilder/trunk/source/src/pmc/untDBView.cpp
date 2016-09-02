//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframDB *framDB;

//---------------------------------------------------------------------------
__fastcall TframDB::TframDB(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------

void TframDB::UpdateView()
{
    int nGroups;
    int nTags;

    RTK_CURSOR hNode;
    hNode = open_node(&key);
    if(!hNode){
        nGroups = 0;
    }else{
        nGroups = cursor_get_subitem_count(hNode);
    }
    RTK_CURSOR hGroup;
    hGroup = cursor_open_first_subitem(hNode);
    nTags = 0;
    while(!cursor_is_end(hGroup)){
        nTags += cursor_get_subitem_count(hGroup);
        cursor_move_next(hGroup);
    }
    close_handle(hGroup);
    close_handle(hNode);
    lblHint->Caption = "分组总数: " + AnsiString(nGroups);
    lblTagCount->Caption = "标签总数：" + AnsiString(nTags);
}
void __fastcall TframDB::btnRefreshClick(TObject *Sender)
{
    CDBItem * it;
    it = (CDBItem*)Tag;
    if(it){
        it->Refresh();
    }
}
//---------------------------------------------------------------------------

void __fastcall TframDB::btnAddClick(TObject *Sender)
{
    InvokeAction(ca_CreateSub);
}
//---------------------------------------------------------------------------

