//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit3.h"
#include "pmc_license.h"

#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "libutils.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

static void scanLocalLicenses();
static void scanRemoteLicenses();

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    if(localFile==""){
        AnsiString s;
        s=get_exe_dir();
        s=s+"license.dat";
        localFile=s;
    }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::expLicExecute(TObject *Sender)
{
    //
    TListItem *it;
    //
    // remove entry
    it=lst->Selected;
    if(!removeLicense(localFile.c_str(),(int)it->Data,/*ignored*/1)){
        return;
    }
    pmc_license_t lic;
    memset(&lic, 0, sizeof lic);
    lic.count=1;
    lic.runLevel=StrToInt(it->SubItems->Strings[0]);
    lic.devLevel=StrToInt(it->SubItems->Strings[1]);
    strncpy(lic.comment,it->SubItems->Strings[2].c_str(),sizeof lic.comment);
    if(!insertLicense(remoteFile.c_str(),&lic)){
        insertLicense(localFile.c_str(),&lic);
    }
    scanLocalLicenses();
    scanRemoteLicenses();
#if 0
    /* create autorun.inf */
    AnsiString autoRun=Form3->lst->Text+"autorun.inf";
    if(!file_exists(autoRun.c_str())){
        AnsiString s;
        s=ExtractFileName(Application->ExeName);
        WritePrivateProfileString("autoRun","open",(s+" -autorun").c_str(),autoRun.c_str());
        CopyFile(Application->ExeName.c_str(),(Form3->lst->Text+s).c_str(),TRUE);
    }
#endif
}
//---------------------------------------------------------------------------

void __fastcall TForm1::impLicExecute(TObject *Sender)
{
    TListItem *it;
    //
    // remove entry
    it=lst2->Selected;
    if(!removeLicense(remoteFile.c_str(),(int)it->Data,/*ignored*/1)){
        return;
    }
    pmc_license_t lic;
    memset(&lic, 0, sizeof lic);
    lic.count=1;
    lic.runLevel=StrToInt(it->SubItems->Strings[0]);
    lic.devLevel=StrToInt(it->SubItems->Strings[1]);
    strncpy(lic.comment,it->SubItems->Strings[2].c_str(),sizeof lic.comment);
    if(!insertLicense(localFile.c_str(),&lic)){
        // restore source entry
        insertLicense(remoteFile.c_str(), &lic);
    }
    scanLocalLicenses();
    scanRemoteLicenses();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::expLicUpdate(TObject *Sender)
{
    //
    ((TAction*)Sender)->Enabled = lst->Selected!=NULL && remoteFile!=""? true : false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::impLicUpdate(TObject *Sender)
{
    //
    ((TAction*)Sender)->Enabled = lst2->Selected!=NULL? true : false;
}

static __bool __cdecl  eProc(const char * file, int entryId, struct pmc_license_t * lic, int context)
{
    TListView *lst;
    TListItem *it;
    lst=(TListView*)context;
    it = lst->Items->Add();
    it->Caption = IntToStr(lic->count);
    it->SubItems->Add(IntToStr(lic->runLevel));
    it->SubItems->Add(IntToStr(lic->devLevel));
    it->SubItems->Add(lic->comment);
    it->Data=(void*)entryId;
	return __true;
}

static void scanLocalLicenses()
{
    Form1->lst->Clear();
    enumLicense(localFile.c_str(),eProc,(int)Form1->lst);
    Form1->Label3->Caption=localFile;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    //
    Caption = Application->Title;
    scanLocalLicenses();
}

static void scanRemoteLicenses()
{
    Form1->lst2->Clear();
    enumLicense(remoteFile.c_str(),eProc,(int)Form1->lst2);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
    int re;
    re=Form3->ShowModal();
    if (re!=mrOk){
        return;
    }
    remoteFile=Form3->lst->Text+"license.dat";
    scanRemoteLicenses();
}
//---------------------------------------------------------------------------

#pragma comment(lib, "libutils.lib")

void __fastcall TForm1::Button4Click(TObject *Sender)
{
    of->InitialDir=ExtractFilePath(localFile);
    of->FileName=localFile;
    printf("pick in %s\n", localFile.c_str());
    if(!of->Execute()) {
        return;
    }
    localFile=of->FileName;
    scanLocalLicenses();
}
//---------------------------------------------------------------------------

