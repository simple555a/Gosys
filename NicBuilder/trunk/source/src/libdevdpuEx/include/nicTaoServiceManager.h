#ifndef __NICSYS_TAOSERVICEMANGER_H__
#define __NICSYS_TAOSERVICEMANGER_H__

#include "nicRefACE.h"
#include "nicRefTAO.h"
#include "nicDefs.h"
#include <vector>
using std::vector;
//定义网络上五个事务，可以扩充
//通道在应用层的编号
#define  NICSYS_CHANNEL_MIN		0
#define  NICSYS_CHANNEL_CONFIG		1
#define  NICSYS_CHANNEL_IOREFRESH	2
#define  NICSYS_CHANNEL_DBREFRESH	3
#define  NICSYS_CHANNEL_ALARM		4
#define  NICSYS_CHANNEL_LOGBOOK	5
#define  NICSYS_CHANNEL_MAX		10

//定义无效的通道ID
#define   UNVALID_CHANNELID(ecid)  ((ecid <= NICSYS_CHANNEL_MIN) || (ecid >= NICSYS_CHANNEL_MAX))




//TAO的启动配置类型,可以组合搭配 STUB_NO_EC|NOTIFY_CONNECT_EC
#define  STUB_NO_EC			0x1   //没有任何事务，没有通道，只用bind_object_to_name和resolve_name_to_object两个接口
#define  NOTIFY_CREATE_ALLEC	0x2   //创建所有通知服务通道
#define  NOTIFY_CONNECT_EC		0x4   //连接通知服务通道，连接哪个通道由用户选择
#define  EVENT_CREATE_ALLEC		0x8   //创建所有事件服务通道，连接哪个通道由用户选择
#define  EVENT_CONNECT_EC		0x16   //连接事件服务通道，连接哪个通道由用户选择

class CTaoNotifySupplier;
class CTaoNotifyConsumer;

//TaoService不用nicdefs.h的定义。

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
	//初始化函数

	int init_i(int argc, char* argv[]);

	int fini_i(void);

	virtual  int init_tao(void);
	virtual  int fini_tao(void);

	//连接命名服务
	int init_name_service(int argc, char* argv[]);
	int fini_name_service();

	//连接通知服务
	int init_notify_service(void);
	int fini_notify_service(void);



	//服务器端创建Notify EventChannel,可以创建多个通道
	//channelID为NICSYS_CHANNEL_###相对应
	int notify_create_eventchannel(int channelID);	
	//服务器端销毁Notify EventChannel
	int notify_destroy_eventchannel(int channelID);	

	//客户端连接Notify EventChannel
	int notify_connect_eventchannel(int channelID);	
	//客户端断开Notify EventChannel
	int notify_disconnect_eventchannel(int channelID);

	//根据连接的通道，先连接通道，后创建此notifyserver后，然后调用send
	int notify_create_server(int channelID);
	//根据连接的通道，先连接通道，后创建此notifyclient后，然后调用recv
	int notify_create_client(int channelID);
	//根据连接的通道，先连接通道，后创建此notifyserver后，然后调用send
	int notify_destroy_server(int channelID);
	//根据连接的通道，先连接通道，后创建此notifyclient后，然后调用recv
	int notify_destroy_client(int channelID);


	//作为Supplier时Push字符串信息,
	//channelID指NICSYS_CHANNEL_####那几个
	int notify_send_eventstring( int channelID,char* domain_name, char * type_name, CORBA::Any & anyData);
	//作为Supplier时Push字符串信息
	int notify_send_eventstring(int channelID, char * domain_name,char *type_name, ACE_Message_Block * msgBlock);
	// 作为consumer添加订阅事件回调
	int notify_add_eventsubscription(int channelID,char * domain_name, char * type_name, NIC_EVENT_HANDLER_FUNC func);
	// 作为consumer删除订阅事件回调
	int notify_del_eventsubscription(int channelID,char * domain_name, char * type_name);



	//连接事件服务，暂不实现	
	int init_event_service(void);
	int fini_event_service(void);




	CORBA::ORB_var get_service_orb(void) const;	
	bool bind_object_to_name(CORBA::Object_ptr objref,char* strContextID,char*  strContextKind);
	bool unbind_object_to_name(char* strContextID,char*  strContextKind);
	CORBA::Object_ptr CTAO_Service_Manager::resolve_name_to_object(char*  strContextID,char*  strContextKind);
	///与105的HMI兼容
	///shuimujie 2012-11-12 start
	bool sim_bind_object_to_name(CORBA::Object_ptr objref,char* strContextID,char*  strContextKind);
	bool sim_unbind_object_to_name(char* strContextID,char*  strContextKind);
	CORBA::Object_ptr CTAO_Service_Manager::sim_resolve_name_to_object(char*  strContextID,char*  strContextKind);
	///与105的HMI兼容
	///shuimujie 2012-11-12 end
private:


	//初始化ORB
	int init_ORB(int argc, char* argv[]);	
	//解析命名服务
	int resolve_naming_service();	
	//解析通知服务
	int resolve_notify_factory();	
	//初始化ORB
	int fini_ORB();	
	//断开命名服务
	int unresolve_naming_service();	
	//断开通知服务
	int unresolve_notify_factory();	

	//创建Supplier的管理员
	int notify_create_supplieradmin(int channelID);	
	//创建默认的Supplier
	int notify_create_suppliers(int channelID);	
	//创建Consumer的管理员
	int notify_create_consumeradmin(int channelID);	
	//创建默认的Consumer
	int notify_create_consumers(int channelID);
	//根据EventChannel创建Supplier的管理员
	int notify_destroy_supplieradmin(int channelID);	
	//创建默认的Supplier实例
	int notify_destroy_suppliers(int channelID);	
	//根据EventChannel创建Consumer的管理员
	int notify_destroy_consumeradmin(int channelID);	
	//创建默认的Consumer实例
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
	//Supplier管理类
	CTaoNotifySupplier* notify_structuredpushsupplier_[NICSYS_CHANNEL_MAX];
	//Consumer管理类
	CTaoNotifyConsumer* notify_structuredpushconsumer_[NICSYS_CHANNEL_MAX];

	char workDir_[MAX_FILE_PATH_LENGTH];

	int workFlag_;

	bool isNameServConnt_;
	bool isNotifyServConnt_;
	bool isEventServConnt_;
	bool bRun;
	//发送读写锁
	ACE_RW_Thread_Mutex  rsMutex_; 
	virtual int svc ();
	ACE_Event *pStopHandle_;
	virtual int open (void *args = 0);
	virtual int close (int flags = 0);
};

class  CTaoNotifyConsumer : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
	CTaoNotifyConsumer();	//构造函数

	~CTaoNotifyConsumer();	//析构函数

	void connect(CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);	//将Consumer连接到EventChannel上(建立一个新的代理Supplier)

	void disconnect();	//断开连接

	CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr get_proxy_supplier();	//获得代理Supplier

	virtual void offer_change(const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);

	virtual void push_structured_event(const CosNotification::StructuredEvent & notification);

	virtual void disconnect_structured_push_consumer();

	vector<nicEventHandler> funcs;	//消息的相关处理函数



protected:
	CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;	//代理Supplier

	CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;	//代理Supplier的ID
};

class  CTaoNotifySupplier : public POA_CosNotifyComm::StructuredPushSupplier
{
public:
	CTaoNotifySupplier();	//构造函数

	~CTaoNotifySupplier();	//析构函数

	int connect(CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);	//将Supplier连接到EventChannel上(建立一个新的代理Consumer)

	void disconnect();	//断开连接

	virtual void send_event(const CosNotification::StructuredEvent& event);	//发送一个通知

	virtual void subscription_change(const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed);	//订阅改变

	virtual void disconnect_structured_push_supplier();

protected:
	CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;	//代理Consumer的ID

	CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;	//代理Consumer
};

#endif