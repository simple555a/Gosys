
#include "ToSqlService.h"




int main(int argc, char **argv)
{

	/*HWND hwnd = NULL;
	if ( hwnd = ::FindWindow( "ConsoleWindowClass", NULL ) )
	{
		::ShowWindow( hwnd, SW_HIDE );
	}*/

	CPmcToSqlService serv;

	serv.RegisterService(argc, argv);

	//以下代码为测试用
	/*CKrtBackupThread::init();
	CKrtStateMonitorThread::init();
	while(1)
	{
		Sleep(1000);
	}*/

	return 0;
};