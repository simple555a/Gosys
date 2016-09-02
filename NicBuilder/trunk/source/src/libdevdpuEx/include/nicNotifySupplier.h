#ifndef __NOTIFY_SUPPLIER_EXPORT_H__
#define __NOTIFY_SUPPLIER_EXPORT_H__

#ifdef NOTIFYSUPPLIER_EXPORTS
#define NOTIFYSUPPLIER_API __declspec(dllexport)
#else
#define NOTIFYSUPPLIER_API __declspec(dllimport)
#endif

// 初始化函数
NOTIFYSUPPLIER_API void init_notify_supplier_service(int argc, char *argv[]);

// 发送一个包含一个字符串的事件，domain_name为组件名称，type_name为事件标志
NOTIFYSUPPLIER_API void send_event_string(char *domain_name, char *type_name, char *str);

#endif