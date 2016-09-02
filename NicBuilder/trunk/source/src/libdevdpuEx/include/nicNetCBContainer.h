

#ifndef _NICSYS_NETCBCONTAINER_H_
#define _NICSYS_NETCBCONTAINER_H_

#include "nicNet.h"

///
/// <summary> 
/// 全局的回调函数注册容器，保留调度模块注册上来的函数地址
/// </summary> 
///
/// <remarks>
/// 当不需要某个回调函数时，将其注册为NULL
///
/// </remarks>
///
class CallBackContainer
{
	//申请友元，防止错误申请对象。
	friend class ACE_Unmanaged_Singleton<CallBackContainer, ACE_Thread_Mutex>;

public:
	CallBackContainer();

	~CallBackContainer();

	//注册回调函数
	nicInt32 registerCallBack(nicVoid * callBackFuncPtr,
										  nicUInt8 callBackFuncNo);

	//根据序号，取得回调函数指针
	nicVoid * getCallBackPtrByNo(nicUInt8 callBackFuncNo);

private:
	//拷贝构造函数，s实现为空
	CallBackContainer(const CallBackContainer& stChina);

	//赋值操作符，实现为空
	CallBackContainer & operator = (const CallBackContainer& stChina);

	//回调函数容器
	nicVoid * callBackPtrContainer[callBackFuncsCount];

	//线程锁
	ACE_Thread_Mutex m_ContMutex;
};

inline CallBackContainer::CallBackContainer()
{
	ACE_OS::memset(callBackPtrContainer, 
							   0, 
							   sizeof(nicVoid*)*callBackFuncsCount);
}

inline CallBackContainer::~CallBackContainer()
{
	ACE_OS::memset(callBackPtrContainer, 
							   0, 
							   sizeof(nicVoid*)*callBackFuncsCount);
}

inline  nicInt32 
CallBackContainer::registerCallBack(nicVoid * callBackFuncPtr,
													  nicUInt8 callBackFuncNo)
{
	//涉及到内部变量的修改，先加锁
	ACE_Guard<ACE_Thread_Mutex>locker(m_ContMutex);

	callBackPtrContainer[callBackFuncNo] = callBackFuncPtr;

	return nicRET_OK;
}

inline nicVoid * 
CallBackContainer::getCallBackPtrByNo(nicUInt8 callBackFuncNo)
{	return callBackPtrContainer[callBackFuncNo];}

inline CallBackContainer::CallBackContainer(const CallBackContainer &)
{
	//
}

inline CallBackContainer&
CallBackContainer::operator =(const CallBackContainer&)
{
	//
}

#endif