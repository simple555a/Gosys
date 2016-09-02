#ifndef __NOTIFY_CONSUMER_EXPORT_H__
#define __NOTIFY_CONSUMER_EXPORT_H__

#include <tao/AnyTypeCode/Any.h>

#ifdef NOTIFYCONSUMER_EXPORTS
#define NOTIFYCONSUMER_API __declspec(dllexport)
#else
#define NOTIFYCONSUMER_API __declspec(dllimport)
#endif

typedef void (*EVENT_HANDLER_FUNC)(CORBA::Any);	//CORBA::Any为实际数据

// 初始化函数
NOTIFYCONSUMER_API void init_notify_consumer_service(int argc, char *argv[]);

// 添加订阅
NOTIFYCONSUMER_API void add_event_subscription(char* domain_name, char* type_name, EVENT_HANDLER_FUNC func);

// 减少订阅
NOTIFYCONSUMER_API void del_event_subscription(char* domain_name, char* type_name);

// 启动服务
NOTIFYCONSUMER_API void start_consumer_service();

// 停止服务
NOTIFYCONSUMER_API void stop_consumer_service();

#endif