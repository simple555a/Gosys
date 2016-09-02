

#ifndef _NICSYS_NETCBCONTAINER_H_
#define _NICSYS_NETCBCONTAINER_H_

#include "nicNet.h"

///
/// <summary> 
/// ȫ�ֵĻص�����ע����������������ģ��ע�������ĺ�����ַ
/// </summary> 
///
/// <remarks>
/// ������Ҫĳ���ص�����ʱ������ע��ΪNULL
///
/// </remarks>
///
class CallBackContainer
{
	//������Ԫ����ֹ�����������
	friend class ACE_Unmanaged_Singleton<CallBackContainer, ACE_Thread_Mutex>;

public:
	CallBackContainer();

	~CallBackContainer();

	//ע��ص�����
	nicInt32 registerCallBack(nicVoid * callBackFuncPtr,
										  nicUInt8 callBackFuncNo);

	//������ţ�ȡ�ûص�����ָ��
	nicVoid * getCallBackPtrByNo(nicUInt8 callBackFuncNo);

private:
	//�������캯����sʵ��Ϊ��
	CallBackContainer(const CallBackContainer& stChina);

	//��ֵ��������ʵ��Ϊ��
	CallBackContainer & operator = (const CallBackContainer& stChina);

	//�ص���������
	nicVoid * callBackPtrContainer[callBackFuncsCount];

	//�߳���
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
	//�漰���ڲ��������޸ģ��ȼ���
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