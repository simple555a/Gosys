//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update (function names modified)   

#include "precomp.h"
#include "NetTask.h"
#include "DPUDevice.h"
#include "CRCheck.h"
//#include "nicIOSSComm.h"
//#include "nicArgs.h"

CDPUDevice::CDPUDevice()
{
	_pTask = NULL;
	//将缓冲区所有内容和指针清零
	_pPctls = NULL;
}

CDPUDevice::~CDPUDevice()
{
}

__i32 CDPUDevice::init_device(PDEVICE_INFO handle,ACE_Reactor * reactor)
{
	__u8 dpuAddr = 0;
	__u8 clientAddr = 0;
	__u32 outtime = 50;//连接超时
	__u16 refreshTime = 400;   // 订阅全数据时间， Zander 2013-8-27 NJS
	CArgs args;
	parse_arg_ex(handle->parameter, &args, " ;-");
	dpuAddr = (__u8)ACE_OS::atoi(args.argv[0]);
	clientAddr = (__u8)ACE_OS::atoi(args.argv[1]);

	//初始化协议仓库
	_pPctls = new CONetPctlDepository();
	_pPctls->init_pctls(&_db);
#ifdef  _DEBUG
	//_pPctls->dubeg_pctls();
#endif

	//将上层分配的reactor传递进来,上层reactor指针为空，则意味着驱动自己要建立reactor
	_pTask = new CNetTask(&_db,__false,_pPctls);
	_pTask->create_reactor(reactor);

	_pTask->set_onet_client_addr(clientAddr);
	_pTask->set_onet_dpu_addr(dpuAddr);
	_pTask->set_cont_outtime(outtime);
	_pTask->set_refresh_time(refreshTime);

	// 初始化CRC列表
 //   InitCRCTable();

	//初始化网络地址
	_pTask->init_net_config(handle->address);
	//初始化网络标签名
	_pTask->init_net_tag();

	////连接控制器
	//Zander 2013-8-29
	if (_pTask->connect_all())
	{
		_pTask->m_is_dpu_exist = __true;
	} 
	else
	{
		_pTask->m_is_dpu_exist = __false;
	}

	//启动处理反应器，处理IO事件
	_pTask->open(0);

	return RET_OK;
}

__i32 CDPUDevice::uninit_device()
{

	//关闭网络
	if (_pTask)
	{
		_pTask->close();
		delete _pTask;
		_pTask = NULL;
	}
	//清空协议仓库
	if ((_pPctls))
	{
		_pPctls->clear_all_pctl();
		delete _pPctls;
		_pPctls = NULL;
	}
	
	__i32 i=0;
	for (i= 0; i <IOCARD_MAX_SIZE; i++)
	{
		if((_db.ioCards[i].channels))
		{
			ACE_ASSERT(_db.ioCards[i].ioCurNum > 0);
			ACE_OS::free(_db.ioCards[i].channels);
		}
	}
	return RET_OK;
}