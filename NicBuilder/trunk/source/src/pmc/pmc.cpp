//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("untMainForm.cpp", frmMain);
USEFORM("untTagView.cpp", framTag); /* TFrame: File Type */
USEFORM("untSerialPortView.cpp", framSerialPort); /* TFrame: File Type */
USEFORM("untConfigurableView.cpp", framConfigurable); /* TFrame: File Type */
USEFORM("untGroupView.cpp", framGroup); /* TFrame: File Type */
USEFORM("untDBView.cpp", framDB); /* TFrame: File Type */
USEFORM("untDCSView.cpp", framDCSView); /* TFrame: File Type */
USEFORM("untAboutForm.cpp", AboutBox);
USEFORM("untSplashForm.cpp", frmSplash);
USEFORM("untNodeView.cpp", framNodeView); /* TFrame: File Type */
USEFORM("untDeviceView.cpp", framDeviceView); /* TFrame: File Type */
USEFORM("untSerialPortsView.cpp", framSerialPorts); /* TFrame: File Type */
USEFORM("untPictureView.cpp", framPictureView); /* TFrame: File Type */
USEFORM("untAddTagView.cpp", framAddTag); /* TFrame: File Type */
USEFORM("untAddGroupView.cpp", framAddGroup); /* TFrame: File Type */
USEFORM("untAddDeviceView.cpp", framAddDevice); /* TFrame: File Type */
USEFORM("untDevicesView.cpp", framDevices); /* TFrame: File Type */
USEFORM("untProgressForm.cpp", frmProgress);
//---------------------------------------------------------------------------
#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtkproxy.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "librtknet.lib")
#pragma comment(lib, "libconfigman.lib")

#pragma comment(lib, "libutils_bcb.lib")
#pragma comment(lib, "librtkproxy_bcb.lib")
#pragma comment(lib, "librtknet_bcb.lib")

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		g_AppPath = ExtractFilePath(Application->ExeName);
		Application->Title = "pmc 实时数据库组态";
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
