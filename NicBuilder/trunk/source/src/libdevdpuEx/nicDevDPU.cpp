//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update(function names modified)                             
//= (19-1)         1.0.0.19      Zander Sun      2013-9-22       update_tag():控制器状态读取后，增加了小IP控制器由备切到主的判断机制。

#include "precomp.h"
#include "DPUDevice.h"

FILE* fp=NULL;
__r4 f_last = 0;

#include "nicRefACE.h"

#ifdef _DEBUG
#pragma comment(lib, "ACEd.lib")
#else
#pragma comment(lib, "ACE.lib")
#endif

//全局生成1个反应器，window下可以处理最高60个对象事件,可以挂载20个NicSys2K的设备
ACE_Reactor *g_ractor =NULL;

// 创建一个reactor退出事件处理器
class ExitHandler : public ACE_Event_Handler
{
public:
	virtual int handle_exception (ACE_HANDLE) {
		reactor ()->end_reactor_event_loop ();
		return 0; // Trigger call to handle_close() method.
	}
};
ACE_THR_FUNC_RETURN tp_event_loop_0 (void *arg)
{
	ACE_TRACE(ACE_TEXT("tp_event_loop"));

	ACE_Reactor *pReactor = (ACE_Reactor *)arg;
	if (pReactor == 0)
	{
		ACE_ERROR ((LM_ERROR, "(%t) proactor create failed\n"));
		return 0;
	}
	pReactor->owner(ACE_Thread::self ());
	while ( pReactor->reactor_event_loop_done () == 0)
	{
		__i32 nRet = pReactor ->run_reactor_event_loop();
		ACE_ERROR ((LM_ERROR, "(%t) ACE_Reactor end event loop\n"));
		if(nRet >=0)
		{
			return 0;
		}
	}
	return 0;
}




IOSS_DLL_EXPORT __bool load(
							PDRIVER_INFO driverObj
							)
{
	if(g_ractor == NULL)
	g_ractor = new ACE_Reactor();

	__i32 nRet = 0;
	//启动Reactor，NicSys2000控制器只用一个reactor，最多带20个控制器
	nRet = ACE_Thread_Manager::instance ()->spawn_n(1, tp_event_loop_0,&g_ractor,0|0|0,THREAD_PRIORITY_ABOVE_NORMAL);


	if (nRet == -1)
	{
		return __false;
	}
	return __true;
}
IOSS_DLL_EXPORT __bool unload()
 {
	 //停止reactor的阻塞运行，关闭线程，得靠自身发送一个停止handler，让reactor自己关闭
	 ExitHandler *handleExit = new ExitHandler;
	 g_ractor->register_handler(handleExit +0);
	 g_ractor->notify(handleExit+0);
	 delete handleExit;
	 handleExit = NULL;
	 delete g_ractor;
	 g_ractor = NULL;
	return __true;
 }

IOSS_DLL_EXPORT __bool start_device(
									PDEVICE_INFO handle
									)
{

	CDPUDevice *dpu = new CDPUDevice();
	 if ((dpu))
	 {
		 dpu->init_device(handle,g_ractor);
	 }
	 else
	 {
		 return __false;
	 }
	handle->OwnerField = dpu;


	return __true;
 }

IOSS_DLL_EXPORT __bool stop_device(
								   PDEVICE_INFO handle
								   )
{



	CDPUDevice *dpu  =(CDPUDevice *)handle->OwnerField;
	if ((dpu))
	{
		dpu->uninit_device();
		delete dpu;
		handle->OwnerField = NULL;
	}
	else
	{
		 return __false;
	}
	return __true;
 }

IOSS_DLL_EXPORT __bool write_device(
									PRTK_TAG tte, 
									PCTAG_VALUE value
									)
 {
	 DPUDB_CURSOR *pCursor = NULL;
	 pmc_value_t vt;
	  ACE_Date_Time dt;
	 set_value_type(vt.Flags, get_value_type(tte->s.Flags));
	 vt.Value = *value;


	 pCursor = *((DPUDB_CURSOR**)tte->d.BinaryAddress);
	 if (!(pCursor))
	 {
		 return __false;
	 }
	 pCursor->set_value(vt);

	return __true;
 }

//DEVICE_API __i32 update_tag(nicIOSSBase_Ptr tte, DBData* new_value,nicTime_Ptr now)
IOSS_DLL_EXPORT void update_tag(PCRTK_TAG tte, struct pmc_value_t * pt, PCRTK_TIME now)
{
	
	__bool ret = __false;
	 
	DPUDB_CURSOR *pCursor = NULL;
	DBData val;
	pCursor = *((DPUDB_CURSOR**)tte->d.BinaryAddress);
	//tte->d.CachedTime = (*now);
	//memcpy(&tte->d.CachedTime, now, sizeof(RTK_TIME));
	//new_value->Time = *now;
	pt->Flags &= ~TF_Valid;
	if (!(pCursor))
	{
		return;
	}
	switch (pCursor->curType)
	{
	case DBCURSOR_iovalue:
		ret = pCursor->get_value(&val);//Zander 

		break;
	case DBCURSOR_dpustatus:
		ret = pCursor->get_dpu_status(&val);
		//小IP由备切到主过程中的延迟判断, Zander, 2013-9-22
		pCursor->delay_small_switch(&val);
		break;
	case DBCURSOR_iostatus:
		ret = pCursor->get_iocard_status(&val);
		break;
	case DBCURSOR_chstatus:
		ret = pCursor->get_channel_status(&val);
		break;
	case DBCURSOR_null:
	default:
		//new_value->Value.DataType = DBDataType_Empty;
		set_value_type(pt->Flags, dt_null);
		return;
	}
	//VariantTypeCast(tte->DataType,&val.Value,&new_value->Value);	
	
	// Zander  添加了if判断
	if (ret)
	{
		pmc_type_cast(&(val.value), pt);

		pt->Flags |= TF_Valid;
		//if(pCursor->_device->_pTask->_isLinked != __true)
		//{
		//		pt->Flags&=~TF_Valid;
		//}
	}

	 return;

 }


//将tag的地址字符串转换为缓冲区游标
 //冒号，短线，点号，分好，竖线,井号都可以作为分隔符
 //DPU状态格式为"DPU：0：0"
 //IO卡状态格式为"CARD：24：0"
 //通道状态格式为"CH：24：0"
 //标签点值格式为"VAL：24：245"
//z注:第一个标识符支持多种辨识
 __i32 parse_tag_address(char * address,DPUDB_CURSOR * pCursor)
{
	CArgs args_1;

	parse_arg_ex(address,&args_1,";-:|#");
	//如果格式不对返回错误
	if (args_1.argc != 3)
	{
		pCursor->curType = DBCURSOR_null;
		return __false;
	}
	//第一个标识符支持多种辨识
	else if (stricmp(args_1.argv[0],"DPU")== 0||
			  stricmp(args_1.argv[0],"CP")== 0||
			  stricmp(args_1.argv[0],"P")== 0 ||
			  stricmp(args_1.argv[0],"0")== 0 )
	{
		//DPU的选择只有两个 0和1
		int dpuNum= atoi(args_1.argv[1]);
		if (dpuNum !=0 && dpuNum !=1)
		{
			pCursor->curType = DBCURSOR_null;
			return __false;
		}
		pCursor->curType = DBCURSOR_dpustatus;
		pCursor->dpustatus_cur.dpu = dpuNum;
		pCursor->dpustatus_cur.statusNum = ACE_OS::atoi(args_1.argv[2]);	
		
	}
	else if (stricmp(args_1.argv[0],"CARD")== 0||
		stricmp(args_1.argv[0],"IO")== 0||
		stricmp(args_1.argv[0],"I")== 0 ||
		stricmp(args_1.argv[0],"1")== 0 )
	{
		//IO的地址只能在0~255之间
		int ioAddr= atoi(args_1.argv[1]);
		if (ioAddr < 0 ||ioAddr > 255 )
		{
			pCursor->curType = DBCURSOR_null;
			return __false;
		}
		pCursor->curType = DBCURSOR_iostatus;
		pCursor->iostatus_cur.ioAddr = ioAddr;
		pCursor->iostatus_cur.statusNum = ACE_OS::atoi(args_1.argv[2]);	
	}
	else if (stricmp(args_1.argv[0],"CH")== 0||
		stricmp(args_1.argv[0],"CL")== 0||
		stricmp(args_1.argv[0],"H")== 0 ||
		stricmp(args_1.argv[0],"2")== 0 )
	{

		//IO的地址只能在0~255之间
		int ioAddr= atoi(args_1.argv[1]);
		if (ioAddr < 0 ||ioAddr > 255 )
		{
			pCursor->curType = DBCURSOR_null;
			return __false;
		}
		pCursor->curType = DBCURSOR_chstatus;
		pCursor->chstatus_cur.ioAddr = ioAddr;
		pCursor->chstatus_cur.chAddr = ACE_OS::atoi(args_1.argv[2]);
	}
	else if (stricmp(args_1.argv[0],"VAL")== 0||
		stricmp(args_1.argv[0],"DATA")== 0||
		stricmp(args_1.argv[0],"V")== 0 ||
		stricmp(args_1.argv[0],"3")== 0 )
	{
		//IO的地址只能在0~255之间
		int ioAddr= atoi(args_1.argv[1]);
		if (ioAddr < 0 ||ioAddr > 255 )
		{
			pCursor->curType = DBCURSOR_null;
			return __false;
		}
		pCursor->curType = DBCURSOR_iovalue;
		pCursor->iovalue_cur.ioAddr = ioAddr;
		pCursor->iovalue_cur.chAddr = ACE_OS::atoi(args_1.argv[2]);
	}
	else
	{
		pCursor->curType = DBCURSOR_null;
		return __false;

	}
	return __true;

}

//DEVICE_API __i32 NICSYS_API dispatch(PDEVICE_INFO device,__u32 majorCode,__u32 param)
IOSS_DLL_EXPORT IOSS_STATUS PMC_API dispatch(
	PDEVICE_INFO device,
	int majorCode,
	int param
	)
 {
	 //标签的
	PRTK_TAG tte = NULL;
	DPUDB_CURSOR *pCursor = NULL;
	CDPUDevice * d =(CDPUDevice *) device->OwnerField;
	 switch(majorCode)
	 {
		 //支持添加标签点
	 case DISP_DB_ADD_TAG:
		 tte = (PRTK_TAG)param;
		pCursor  = new DPUDB_CURSOR(d);
		if(!(d))
		{
			pCursor->curType = DBCURSOR_null;
			break;
		}
		*((DPUDB_CURSOR**)tte->d.BinaryAddress) = pCursor;
		 parse_tag_address(tte->s.Address,pCursor);
		 break;
		 //支持主备切换事件
	 case DISP_POWER_STATE_CHANGED:

		 break;
		 //支持删除标签点

	 case DISP_DB_DROP_TAG:
	 default:
		 tte = (PRTK_TAG)param;
		 pCursor = *((DPUDB_CURSOR**)tte->d.BinaryAddress);
		 if ((tte) && (pCursor))
		 {
			 delete pCursor;
			 *((DPUDB_CURSOR**)tte->d.BinaryAddress) = 0;
		 }
		 break;

	 }
	return IOSS_SUCCESS;
 }
