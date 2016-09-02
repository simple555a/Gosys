//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <pmc.h>
#include "unit1.h"
//---------------------------------------------------------------------------
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit2.cpp", Form2);
USEFORM("Unit3.cpp", Form3);

AnsiString remoteFile;
AnsiString localFile;

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         localFile=get_exe_dir();
         localFile+="license.dat";
#if 0
         {
            int i;
            for(i=0; i<_argc; i++){
                if(!strcmpi(_argv[i],"-autorun")){
                    remoteFile=get_exe_dir();
                    remoteFile+="license.dat";
                    localFile="c:/actech/pmc3/license.dat";
                }
            }
         }
         if(!file_exists(localFile.c_str())){
            localFile="c:/actech/pmc3/license.dat";
         }
#endif
         Application->Title = "Actech授权管理工具";
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(TForm2), &Form2);
         Application->CreateForm(__classid(TForm3), &Form3);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
