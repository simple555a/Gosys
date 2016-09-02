#ifndef __NOTIFY_CONSUMER_EXPORT_H__
#define __NOTIFY_CONSUMER_EXPORT_H__

#include <tao/AnyTypeCode/Any.h>

#ifdef NOTIFYCONSUMER_EXPORTS
#define NOTIFYCONSUMER_API __declspec(dllexport)
#else
#define NOTIFYCONSUMER_API __declspec(dllimport)
#endif

typedef void (*EVENT_HANDLER_FUNC)(CORBA::Any);	//CORBA::AnyΪʵ������

// ��ʼ������
NOTIFYCONSUMER_API void init_notify_consumer_service(int argc, char *argv[]);

// ��Ӷ���
NOTIFYCONSUMER_API void add_event_subscription(char* domain_name, char* type_name, EVENT_HANDLER_FUNC func);

// ���ٶ���
NOTIFYCONSUMER_API void del_event_subscription(char* domain_name, char* type_name);

// ��������
NOTIFYCONSUMER_API void start_consumer_service();

// ֹͣ����
NOTIFYCONSUMER_API void stop_consumer_service();

#endif