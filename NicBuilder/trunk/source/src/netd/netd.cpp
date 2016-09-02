/******************
netd主要功能：
1、发送 本地的实时库服务的状态 到两个指定的地址
2、发送 指定标签的实时值       到两个指定的地址
3、发送 当前时间               到两个指定的地址

其中，
    两个目标地址：默认为192.168.0.255和192.168.1.255,也可用NicBuilder.ini中[netd]的net1和net2指定。
	指定标签名：在gmem.ini中指定
*****************************/
/*
改动记录
Date       Version   Modifed by    Modify
2012-7-17  1.0.0.7   Zander Sun    1.增加日志记录功能，用于测试
*/
#include "windows.h"
#include "stdio.h"

#include "utils.h"
#include "netservice.h"
//2012-7-17
#include "netLog.h"

int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
    HANDLE hProcess;
	//取当前进程的句柄
    hProcess=GetCurrentProcess();
	//设置当前进程的优先级
	//REALTIME_PRIORITY_CLASS--指示该进程拥有可用的最高优先级。
    if(!SetPriorityClass(hProcess,REALTIME_PRIORITY_CLASS)){
        return 1;
    }

    // create the service-object
	CNetService serv;
	
	// RegisterService() checks the parameterlist for predefined switches
	// (such as -d or -i etc.; see NTService.h for possible switches) and
	// starts the services functionality.
	// You can use the return value from "RegisterService()"
	// as your exit-code.


	//2012-7-17 日志记录
	logIsEnabled = __false;//__true为启动日志记录功能
	open_log();

	serv.RegisterService(argc, argv);
    
	close_log();//2012-7-17 关闭日志

	return 1;
}
