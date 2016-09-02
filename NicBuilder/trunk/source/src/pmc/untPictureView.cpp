//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TframPictureView *framPictureView = new TframPictureView(0);

//---------------------------------------------------------------------------
__fastcall TframPictureView::TframPictureView(TComponent* Owner)
    : TFrame(Owner)
{
    SHFILEINFO fi;
    // add an entry
    ZeroMemory(&fi, sizeof(fi));
    SHGetFileInfo("$$$$faked.bmp", FILE_ATTRIBUTE_NORMAL,
        &fi, sizeof(fi), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON
    );
    ImageList_AddIcon((_IMAGELIST*)ImageList1->Handle, fi.hIcon);
}

//---------------------------------------------------------------------------
void TframPictureView::UpdateView()
{
    char buf[1024*100];
    __uint i, count = sizeof(buf);
    CPictureItem *it;
    it = dynamic_cast<CPictureItem *>((CConfigurableItem*)Tag);
    assert(it);
    count = discover_files(
        &it->node,
        "file",
        "*.gdf",
        buf,
        count,
        10000);
    lv->Items->BeginUpdate();
    lv->Items->Clear();
    char *p=buf;
    AnsiString s;
    for(i=0; i<count; i++){
        TListItem *lit;
        lit = lv->Items->Add();
        lit->ImageIndex = 0;
        s = p+5;
        s.Delete(s.Length() - 3, 4);
        lit->Caption = s;
        p += strlen(p) + 1;
    }
    lv->Items->EndUpdate();
    node = it->node;
    edtName->Text = "";
}
void __fastcall TframPictureView::lvClick(TObject *Sender)
{
    //
}
//---------------------------------------------------------------------------
void __fastcall TframPictureView::lvDblClick(TObject *Sender)
{
    //
    Button3Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TframPictureView::Button3Click(TObject *Sender)
{
    //
    TListItem * lit;
    lit = lv->Selected;
    if(!lit){
        return;
    }
    CPictureItem *it;
    it = dynamic_cast<CPictureItem*>((CConfigurableItem*)Tag);
    if(!it){
        return;
    }
    AnsiString cmdline;
    cmdline = g_AppPath + "mcdraw.exe";
    cmdline += " -node:";
    cmdline += (char*)CNodeName(it->node);
    cmdline += " -file:\"file/";
    cmdline += lit->Caption + ".gdf";
    cmdline += "\"";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    CreateProcess(
        0,
        cmdline.c_str(),
        0,0,0,0,0,0,
        &si,
        &pi
        );
    utils_debug("Executing %s\n", cmdline.c_str());
    if(pi.hProcess){
        CloseHandle(pi.hProcess);
    }
    if(pi.hThread){
        CloseHandle(pi.hThread);
    }
}
//---------------------------------------------------------------------------
void __fastcall TframPictureView::Button1Click(TObject *Sender)
{
    char *buf;
    buf = "<?xml version=\"1.0\"?><hmif c=\"13158600\" w=\"6400\" h=\"4600\" cap=\"\"></hmif>";
    AnsiString file;
    if(edtName->Text == ""){
        Application->MessageBox("文件名称不能为空", "错误");
        return;
    }
    for(int i=0; i<lv->Items->Count; i++){
        TListItem *it;
        it = lv->Items->Item[i];
        if(it->Caption == edtName->Text){
            Application->MessageBox("文件已经存在.","错误");
            return;
        }
    }
    file = "file/";
    file += edtName->Text;
    file += ".gdf";
    if(!upload_file(
        &node,
        file.c_str(),
        buf,
        strlen(buf),
        1000
        ))
    {
        Application->MessageBox("上载文件失败.",Application->Title.c_str());
        return;
    }
    TListItem *it;
    it = lv->Items->Add();
    it->Caption = edtName->Text;
    it->ImageIndex = 0;
    //
}
//---------------------------------------------------------------------------
void __fastcall TframPictureView::Button2Click(TObject *Sender)
{
    DWORD res;
    AnsiString hint;
    TListItem * lit;
    lit = lv->Selected;
    if(!lit){
        return;
    }
    CPictureItem *it;
    it = dynamic_cast<CPictureItem*>((CConfigurableItem*)Tag);
    if(!it){
        return;
    }
    hint = "确定要删除";
    hint += lit->Caption;
    hint += "吗?";
    res = Application->MessageBox(hint.c_str(), Application->Title.c_str(), MB_YESNO);
    if(res != IDYES){
        return;
    }
    AnsiString fn;
    fn = "file/";
    fn += lit->Caption;
    fn += ".gdf";
    if(proxy_delete_file(
        &it->node,
        fn.c_str(),
        1000))
    {
        lv->Items->Delete(lit->Index);
    }
}
//---------------------------------------------------------------------------
