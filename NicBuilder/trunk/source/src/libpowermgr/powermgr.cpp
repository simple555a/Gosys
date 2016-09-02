#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <powermgr.h>
#include <rtk_list_entry.h>

static int power_state = PWR_BACKUP;

struct CALLBACK_ITEM
{
	POWERMGR_CALLBACK	func;
	int context;
	RTK_LIST_ENTRY link;
};

static RTK_LIST_ENTRY cbs;

/*
功能：检查链表结构体连接，初始化电源状态和链表结构体
参数：无
返回：无
*/
static void _check_queue()
{
	if(!cbs.Flink){
		init_powermgr();
	}
}

/*
函数功能：初始化电源状态power_state,连接链表结构体cbs
参数说明：无
  返回值：0
*/
POWERMGR_API int PMC_API init_powermgr()
{
	power_state = PWR_BACKUP;
	RtkInitializeListHead(&cbs);
	return 0;
}

POWERMGR_API __bool PMC_API set_power_state(int newstate)
{
	CALLBACK_ITEM * ci;
	RTK_LIST_ENTRY * li;

	_check_queue();
	li = cbs.Flink;
	while(li != &cbs){
		ci = SAFE_CONTAINING_RECORD(li, CALLBACK_ITEM, link);
		if( !ci->func(newstate, ci->context) ){
			return __false;
		}
		li = li->Flink;
	}

	power_state = newstate;

	return __true;
}

/*
功能：取得电源状态，若未初始化，则将电源状态设为PWR_BACKUP，返回
参数：无
返回：电源状态
*/
POWERMGR_API int PMC_API get_power_state()
{
	_check_queue();
	return power_state;
}

/*
功能：初始化生成CALLBACK_ITEM
参数：
      cb        函数指针
	  context   
返回：0    分配CALLBACK_ITEM失败
      1    成功
*/
POWERMGR_API __bool PMC_API register_power_callback(
	POWERMGR_CALLBACK cb, 
	int context
	)
{
	CALLBACK_ITEM * ci;
	//1 检查链表结构体连接，初始化电源状态和链表结构体
	_check_queue();
	//2 将cb和context插入到CALLBACK型结构体ci中，将&ci->link链接到RTK_LIST_ENTRY型结构体cbs中。
	ci = new CALLBACK_ITEM;
	if(!ci){
		return __false;
	}
	ci->func = cb;
	ci->context = context;
	RtkInsertTailList(&cbs, &ci->link);
	return __true;
}

/*
参数：解除回调项CALLBACK_ITEM
功能：
      cb       待解除的回调项的函数指针成员
	  context  待解除的回调项的context成员
返回：0      没找到cb和context对应的回调项
      1      成功
*/
POWERMGR_API __bool PMC_API unregister_power_callback(
	POWERMGR_CALLBACK cb, 
	int context
	)
{
	CALLBACK_ITEM * ci;
	RTK_LIST_ENTRY * li;

	_check_queue();
	li = cbs.Flink;
	while(li != &cbs){
		//由li的地址和ci的成员link与ci的地址差，推算出ci的地址
		ci = SAFE_CONTAINING_RECORD(li, CALLBACK_ITEM, link);
		
		//找到与cb和context对应的ci，对其解除链接并delete
		if(ci->func == cb && ci->context == context){
			RtkRemoveEntryList(li);
			delete ci;
			break;
		}
		li = li->Flink;
	}
    
	if(li == &cbs){    //表示没找到要delete的ci
		return __false;
	}

	return __true;
}

/*
功能：
参数：
返回：
*/
static __bool PMC_API _power_state_changed(
	int ns, 
	int context
	)
{
	CPowerAware * pa;

	_check_queue();
	pa = reinterpret_cast<CPowerAware*>(context);//强制类型转换
	return pa->OnPowerStateChanged(ns);
}

CPowerAware::CPowerAware()
{
	_check_queue();
	register_power_callback(_power_state_changed, (int)this);
}

CPowerAware::~CPowerAware()
{
	unregister_power_callback(_power_state_changed, (int)this);
}
