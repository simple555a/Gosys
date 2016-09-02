//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProgress *frmProgress;

char    config_files[1024*100];
__uint  iFiles, iCurFile;
bool    bInProgress, bAbort;

static void (_cdecl *_OldOnProgress)(__uint t, __uint c, const char * msg);

static void _cdecl _OnProgress(__uint t, __uint c, const char * msg)
{
    frmProgress->lblThis->Caption = msg;
    if(t){
        frmProgress->pbThis->Position = c*100/t;
    }
}


//---------------------------------------------------------------------------
__fastcall TfrmProgress::TfrmProgress(TComponent* Owner)
    : TForm(Owner)
{
    frmProgress = this;
}

#define err(s) Application->MessageBox(s, "����");

#define dbg(s) lb->Items->Add(s)

bool TfrmProgress::Download()
{
    FILE *fp;
    fp = fopen(file.c_str(), "wb");
    pbThis->Visible = true;
    if(!fp){
        err("���ļ�ʧ��.");
        return false;
    }
    if( !discover_configfiles(
        &node,
        config_files,
        sizeof(config_files),
        20480
        ) )
    {
        err("δ����Ŀ��ڵ�������ļ�");
        fclose(fp);
        return false;
    }
    char *p;
    iFiles=0;
    p = config_files;
    while(*p){
        iFiles++;
        p += strlen(p) + 1;
    }
    if(!iFiles){
        err("δ����Ŀ��ڵ�������ļ�");
        fclose(fp);
        return false;
    }
    p = config_files;
    iCurFile = 0;
    while(*p){
        char *fbuf;
        __uint fsize;
        if(!download_file(
            &node,
            p,
            &fbuf,
            &fsize))
        {
            err("�����ļ�ʧ��.");
            fclose(fp);
            return false;
        }

        dbg(AnsiString(p) + "(" + IntToStr(fsize) + "�ֽ�)");

        fwrite("$$$FILE:", 1, 8, fp);
        __u32 namelen = strlen(p);
        fwrite(&namelen, 1, sizeof(namelen), fp);
        fwrite(&fsize, 1, sizeof(fsize), fp);
        fwrite(p, 1, strlen(p) + 1, fp);
        fwrite(fbuf, 1, fsize, fp);
        proxy_release_data(fbuf);
        iCurFile++;
        pbTotal->Position = iCurFile*100/iFiles;
        p += strlen(p) + 1;

        Sleep(100);
        Application->ProcessMessages();
        if(bAbort){
            int res = Application->MessageBox(
                "��ȷ��Ҫֹͣ������", Application->Title.c_str(), MB_YESNO);
            if(res == ID_YES){
                break;
            }
            bAbort = false;
        }
    }
    fclose(fp);
    return true;
}

bool TfrmProgress::Upload()
{
    FILE *fp;
    fp = fopen(file.c_str(), "rb");
    if(!fp){
        err("���ļ�ʧ��.");
        return false;
    }
    pbThis->Visible = false;
    char *p;
    __u32 fsize, nsize;
    char *fname, *fdata;
    __uint totalbytes, bytes;
    fseek(fp, 0, SEEK_END);
    totalbytes = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    while( !feof(fp) ){
        char sig[9];
        sig[8] = 0;
        if( fread(sig, 1, 8, fp) != 8 ){
            if(feof(fp)){
                break;
            }
            err("�ļ���ʽ����ȷ.");
            fclose(fp);
            return false;
        }
        if( strcmp(sig, "$$$FILE:") ){
            err("�ļ���ʽ����ȷ.");
            fclose(fp);
            return false;
        }
        fread(&nsize, 1, sizeof(nsize), fp);
        fread(&fsize, 1, sizeof(fsize), fp);
        p = new char[fsize + MAX_PATH];
        if(!p){
            err("�ڴ����ʧ��.");
            fclose(fp);
            return false;
        }
        if( fread(p, 1, fsize+nsize+1, fp) != fsize+nsize+1) {
            err("�ļ���ʽ����ȷ.");
            fclose(fp);
            delete p;
            return false;
        }
        fname = p;
        if(fname[nsize] != 0){
            err("�ļ���ʽ����ȷ.");
            fclose(fp);
            delete p;
            return false;
        }
        fdata = fname + nsize + 1;
        if(!upload_file(
            &node,
            fname,
            fdata,
            fsize,
            (fsize + 1023)/1024 * 100
            ))
        {
            err((AnsiString(fname) + ":�ļ�����ʧ��.").c_str());
            fclose(fp);
            delete p;
            return false;
        }
        delete p;
        bytes = ftell(fp);
        pbTotal->Position = bytes*100/totalbytes;
        Sleep(200);
        Application->ProcessMessages();
        if(bAbort){
            int res = Application->MessageBox(
                "��ȷ��Ҫֹͣ�ϴ�������Ϣ��", Application->Title.c_str(), MB_YESNO);
            if(res == ID_YES){
                break;
            }
            bAbort = false;
        }
    }
    fclose(fp);
    return true;
}

//---------------------------------------------------------------------------
void __fastcall TfrmProgress::FormCreate(TObject *Sender)
{
    _OldOnProgress = g_ProxyDispatchTable.OnProgress;
    g_ProxyDispatchTable.OnProgress = _OnProgress;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProgress::FormDestroy(TObject *Sender)
{
    g_ProxyDispatchTable.OnProgress = _OldOnProgress;
}

//---------------------------------------------------------------------------
void __fastcall TfrmProgress::Timer1Timer(TObject *Sender)
{
    bool ret;
    Timer1->Enabled = false;
    bInProgress = true;
    bAbort = false;
    if(bDownload){
        if( Download() ){
            // Application->MessageBox( ("������Ϣ�Ѿ����浽 ." + file).c_str(), "����");
        }
    }else{
        if( Upload() ){
            // Application->MessageBox("������Ϣ�Ѿ�����(�ڵ���Ҫ����λ������Ч).", "����");
        }
    }
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmProgress::btnCancelClick(TObject *Sender)
{
    bAbort = true;
}
//---------------------------------------------------------------------------
