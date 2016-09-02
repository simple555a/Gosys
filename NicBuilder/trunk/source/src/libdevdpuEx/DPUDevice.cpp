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
	//���������������ݺ�ָ������
	_pPctls = NULL;
}

CDPUDevice::~CDPUDevice()
{
}

__i32 CDPUDevice::init_device(PDEVICE_INFO handle,ACE_Reactor * reactor)
{
	__u8 dpuAddr = 0;
	__u8 clientAddr = 0;
	__u32 outtime = 50;//���ӳ�ʱ
	__u16 refreshTime = 400;   // ����ȫ����ʱ�䣬 Zander 2013-8-27 NJS
	CArgs args;
	parse_arg_ex(handle->parameter, &args, " ;-");
	dpuAddr = (__u8)ACE_OS::atoi(args.argv[0]);
	clientAddr = (__u8)ACE_OS::atoi(args.argv[1]);

	//��ʼ��Э��ֿ�
	_pPctls = new CONetPctlDepository();
	_pPctls->init_pctls(&_db);
#ifdef  _DEBUG
	//_pPctls->dubeg_pctls();
#endif

	//���ϲ�����reactor���ݽ���,�ϲ�reactorָ��Ϊ�գ�����ζ�������Լ�Ҫ����reactor
	_pTask = new CNetTask(&_db,__false,_pPctls);
	_pTask->create_reactor(reactor);

	_pTask->set_onet_client_addr(clientAddr);
	_pTask->set_onet_dpu_addr(dpuAddr);
	_pTask->set_cont_outtime(outtime);
	_pTask->set_refresh_time(refreshTime);

	// ��ʼ��CRC�б�
 //   InitCRCTable();

	//��ʼ�������ַ
	_pTask->init_net_config(handle->address);
	//��ʼ�������ǩ��
	_pTask->init_net_tag();

	////���ӿ�����
	//Zander 2013-8-29
	if (_pTask->connect_all())
	{
		_pTask->m_is_dpu_exist = __true;
	} 
	else
	{
		_pTask->m_is_dpu_exist = __false;
	}

	//��������Ӧ��������IO�¼�
	_pTask->open(0);

	return RET_OK;
}

__i32 CDPUDevice::uninit_device()
{

	//�ر�����
	if (_pTask)
	{
		_pTask->close();
		delete _pTask;
		_pTask = NULL;
	}
	//���Э��ֿ�
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