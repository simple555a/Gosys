#ifndef __NICSYS_TAOSERVICEMANGER_H__
#define __NICSYS_TAOSERVICEMANGER_H__

#include "nicRefACE.h"
#include "nicRefTAO.h"
#include "nicDefs.h"
#include <vector>
using std::vector;
//����������������񣬿�������
//ͨ����Ӧ�ò�ı��
#define  NICSYS_CHANNEL_MIN		0
#define  NICSYS_CHANNEL_CONFIG		1
#define  NICSYS_CHANNEL_IOREFRESH	2
#define  NICSYS_CHANNEL_DBREFRESH	3
#define  NICSYS_CHANNEL_ALARM		4
#define  NICSYS_CHANNEL_LOGBOOK	5
#define  NICSYS_CHANNEL_MAX		10

//������Ч��ͨ��ID
#define   UNVALID_CHANNELID(ecid)  ((ecid <= NICSYS_CHANNEL_MIN) || (ecid >= NICSYS_CHANNEL_MAX))




//TAO��������������,������ϴ��� STUB_NO_EC|NOTIFY_CONNECT_EC
#define  STUB_NO_EC			0x1   //û���κ�����û��ͨ����ֻ��bind_object_to_name��resolve_name_to_object�����ӿ�
#define  NOTIFY_CREATE_ALLEC	0x2   //��������֪ͨ����ͨ��
#define  NOTIFY_CONNECT_EC		0x4   //����֪ͨ����ͨ���������ĸ�ͨ�����û�ѡ��
#define  EVENT_CREATE_ALLEC		0x8   //���������¼�����ͨ���������ĸ�ͨ�����û�ѡ��
#define  EVENT_CONNECT_EC		0x16   //�����¼�����ͨ���������ĸ�ͨ�����û�ѡ��

class CTaoNotifySupplier;
class CTaoNotifyConsumer;

//TaoService����nicdefs.h�Ķ��塣

#ifdef _WIN32
#if defined  CLASS_TAOMANAGER_EXPORTS
#define CLASS_TAOMANAGER_API	  __declspec(dllexport)
#else
#define CLASS_TAOMANAGER_API	  __declspec(dllimport)
#endif
#else
#define CLASS_TAOMANAGER_API
#endif


class  CLASS_TAOMANAGER_API CTAO_Service_Manager:public ACE_Task<ACE_MT_SYNCH>
{
public:
	CTAO_Service_Manager(char* path,int unflag);
	CTAO_Service_Manager(void);
	~CTAO_Service_Manager(void);
	//��ʼ������

	int init_i(int argc, char* argv[]);

	int fini_i(void);

	virtual  int init_tao(void);
	virtual  int fini_tao(void);

	//������������
	int init_name_service(int argc, char* argv[]);
	int fini_name_service();

	//����֪ͨ����
	int init_notify_service(void);
	int fini_notify_service(void);



	//�������˴���Notify EventChannel,���Դ������ͨ��
	//channelIDΪNICSYS_CHANNEL_###���Ӧ
	int notify_create_eventchannel(int channelID);	
	//������������Notify EventChannel
	int notify_destroy_eventchannel(int channelID);	

	//�ͻ�������Notify EventChannel
	int notify_connect_eventchannel(int channelID);	
	//�ͻ��˶Ͽ�Notify EventChannel
	int notify_disconnect_eventchannel(int channelID);

	//�������ӵ�ͨ����������ͨ�����󴴽���notifyserver��Ȼ�����send
	int notify_create_server(int channelID);
	//�������ӵ�ͨ����������ͨ�����󴴽���notifyclient��Ȼ�����recv
	int notify_create_client(int channelID);
	//�������ӵ�ͨ����������ͨ�����󴴽���notifyserver��Ȼ�����send
	int notify_destroy_server(int channelID);
	//�������ӵ�ͨ����������ͨ�����󴴽���notifyclient��Ȼ�����recv
	int notify_destroy_client(int channelID);


	//��ΪSupplierʱPush�ַ�����Ϣ,
	//channelIDָNICSYS_CHANNEL_####�Ǽ���
	int notify_send_eventstring( int channelID,char* domain_name, char * type_name, CORBA::Any & anyData);
	//��ΪSupplierʱPush�ַ�����Ϣ
	int notify_send_eventstring(int channelID, char * domain_name,char *type_name, ACE_Message_Block * msgBlock);
	// ��Ϊconsumer��Ӷ����¼��ص�
	int notify_add_eventsubscription(int channelID,char * domain_name, char * type_name, NIC_EVENT_HANDLER_FUNC func);
	// ��Ϊconsumerɾ�������¼��ص�
	int notify_del_eventsubscription(int channelID,char * domain_name, char * type_name);



	//�����¼������ݲ�ʵ��	
	int init_event_service(void);
	int fini_event_service(void);




	CORBA::ORB_var get_service_orb(void) const;	
	bool bind_object_to_name(CORBA::Object_ptr objref,char* strContextID,char*  strContextKind);
	bool unbind_object_to_name(char* strContextID,char*  strContextKind);
	CORBA::Object_ptr CTAO_Service_Manager::resolve_name_to_object(char*  strContextID,char*  strContextKind);
	///��105��HMI����
	///shuimujie 2012-11-12 start
	bool sim_bind_object_to_name(CORBA::Object_ptr objref,char* strContextID,char*  strContextKind);
	bool sim_unbind_object_to_name(char* strContextID,char*  strContextKind);
	CORBA::Object_ptr CTAO_Service_Manager::sim_resolve_name_to_object(char*  strContextID,char*  strContextKind);
	///��105��HMI����
	///shuimujie 2012-11-12 end
private:


	//��ʼ��ORB
	int init_ORB(int argc, char* argv[]);	
	//������������
	int resolve_naming_service();	
	//����֪ͨ����
	int resolve_notify_factory();	
	//��ʼ��ORB
	int fini_ORB();	
	//�Ͽ���������
	int unresolve_naming_service();	
	//�Ͽ�֪ͨ����
	int unresolve_notify_factory();	

	//����Supplier�Ĺ���Ա
	int notify_create_supplieradmin(int channelID);	
	//����Ĭ�ϵ�Supplier
	int notify_create_suppliers(int channelID);	
	//����Consumer�Ĺ���Ա
	int notify_create_consumeradmin(int channelID);	
	//����Ĭ�ϵ�Consumer
	int notify_create_consumers(int channelID);
	//����EventChannel����Supplier�Ĺ���Ա
	int notify_destroy_supplieradmin(int channelID);	
	//����Ĭ�ϵ�Supplierʵ��
	int notify_destroy_suppliers(int channelID);	
	//����EventChannel����Consumer�Ĺ���Ա
	int notify_destroy_consumeradmin(int channelID);	
	//����Ĭ�ϵ�Consumerʵ��
	int notify_destroy_consumers(int channelID);


protected:

	PortableServer::POA_var root_poa_;
private:


	CORBA::ORB_var orb_;

	CORBA::Object_var naming_obj_;

	CosNaming::NamingContext_var naming_context_;

	CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

	CosNotifyChannelAdmin::EventChannel_var ec_[NICSYS_CHANNEL_MAX];

	CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;

	CosNotification::QoSProperties initial_qos_;

	CosNotification::AdminProperties initial_admin_;

	CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_[NICSYS_CHANNEL_MAX];
	CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_[NICSYS_CHANNEL_MAX];
	//Supplier������
	CTaoNotifySupplier* notify_structuredpushsupplier_[NICSYS_CHANNEL_MAX];
	//Consumer������
	CTaoNotifyConsumer* notify_structuredpushconsumer_[NICSYS_CHANNEL_MAX];

	char workDir_[MAX_FILE_PATH_LENGTH];

	int workFlag_;

	bool isNameServConnt_;
	bool isNotifyServConnt_;
	bool isEventServConnt_;
	bool bRun;
	//���Ͷ�д��
	ACE_RW_Thread_Mutex  rsMutex_; 
	virtual int svc ();
	ACE_Event *pStopHandle_;
	virtual int open (void *args = 0);
	virtual int close (int flags = 0);
};

class  CTaoNotifyConsumer : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
	CTaoNotifyConsumer();	//���캯��

	~CTaoNotifyConsumer();	//��������

	void connect(CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);	//��Consumer���ӵ�EventChannel��(����һ���µĴ���Supplier)

	void disconnect();	//�Ͽ�����

	CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier();	//��ô���Supplier

	virtual void offer_change(const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);

	virtual void push_structured_event(const CosNotification::StructuredEvent & notification);

	virtual void disconnect_structured_push_consumer();

	vector<nicEventHandler> funcs;	//��Ϣ����ش�����



protected:
	CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;	//����Supplier

	CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;	//����Supplier��ID
};

class  CTaoNotifySupplier : public POA_CosNotifyComm::StructuredPushSupplier
{
public:
	CTaoNotifySupplier();	//���캯��

	~CTaoNotifySupplier();	//��������

	int connect(CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);	//��Supplier���ӵ�EventChannel��(����һ���µĴ���Consumer)

	void disconnect();	//�Ͽ�����

	virtual void send_event(const CosNotification::StructuredEvent& event);	//����һ��֪ͨ

	virtual void subscription_change(const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);	//���ĸı�

	virtual void disconnect_structured_push_supplier();

protected:
	CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;	//����Consumer��ID

	CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;	//����Consumer
};

#endif