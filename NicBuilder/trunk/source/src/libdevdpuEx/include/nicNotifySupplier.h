#ifndef __NOTIFY_SUPPLIER_EXPORT_H__
#define __NOTIFY_SUPPLIER_EXPORT_H__

#ifdef NOTIFYSUPPLIER_EXPORTS
#define NOTIFYSUPPLIER_API __declspec(dllexport)
#else
#define NOTIFYSUPPLIER_API __declspec(dllimport)
#endif

// ��ʼ������
NOTIFYSUPPLIER_API void init_notify_supplier_service(int argc, char *argv[]);

// ����һ������һ���ַ������¼���domain_nameΪ������ƣ�type_nameΪ�¼���־
NOTIFYSUPPLIER_API void send_event_string(char *domain_name, char *type_name, char *str);

#endif