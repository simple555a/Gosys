#include<utils.h>
#include<f8.h>
#include<f8_kproxy.h>
#include<f8_kserver.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libkserver.lib")
#pragma comment(lib,"libkadapter.lib")
#pragma comment(lib,"libkproxy.lib")
#pragma comment(lib,"libfkern.lib")
#pragma comment(lib,"libfbd.lib")
#pragma comment(lib,"libCPres.lib")
#pragma comment(lib,"libCPmm.lib")
#pragma comment(lib,"libcobj.lib")
#pragma comment(lib,"libutils.lib")
#pragma comment(lib,"librtk.lib")



int main(int c,char**v)
{
	WSADATA wsa;
	WSAStartup(0x202,&wsa);

	char buf[1024];
	int leakDetect = -1;
	__bool debugMode;

	BEGIN_ARGS
		ARG("-leak:",leakDetect,"int","")
		ARG("--debug",debugMode,"tbool","debug mode")
	END_ARGS

	if(process_args_1(c,v) < 0)
	{
		print_usage_1();
		return -1;
	}
	kproxy_init(leakDetect);
	while(!feof(stdin))
	{
		utils_trace("# ");
		utils_gets(buf,sizeof(buf));
		if(buf[strlen(buf)-1] == '\n')
		{
			buf[strlen(buf)-1] = '\0';
		}
		if(!strcmp(buf,"q"))
		{
			break;
		}
		if(*buf)
		{
			kproxy_shell(buf);
		}
	}

	kproxy_shell("close");
	kproxy_uninit();

	WSACleanup();
	return 0;
	
}