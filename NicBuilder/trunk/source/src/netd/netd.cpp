/******************
netd��Ҫ���ܣ�
1������ ���ص�ʵʱ������״̬ ������ָ���ĵ�ַ
2������ ָ����ǩ��ʵʱֵ       ������ָ���ĵ�ַ
3������ ��ǰʱ��               ������ָ���ĵ�ַ

���У�
    ����Ŀ���ַ��Ĭ��Ϊ192.168.0.255��192.168.1.255,Ҳ����NicBuilder.ini��[netd]��net1��net2ָ����
	ָ����ǩ������gmem.ini��ָ��
*****************************/
/*
�Ķ���¼
Date       Version   Modifed by    Modify
2012-7-17  1.0.0.7   Zander Sun    1.������־��¼���ܣ����ڲ���
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
	//ȡ��ǰ���̵ľ��
    hProcess=GetCurrentProcess();
	//���õ�ǰ���̵����ȼ�
	//REALTIME_PRIORITY_CLASS--ָʾ�ý���ӵ�п��õ�������ȼ���
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


	//2012-7-17 ��־��¼
	logIsEnabled = __false;//__trueΪ������־��¼����
	open_log();

	serv.RegisterService(argc, argv);
    
	close_log();//2012-7-17 �ر���־

	return 1;
}
