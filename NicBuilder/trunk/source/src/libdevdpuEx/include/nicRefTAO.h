#ifndef _NICSYS_REFTAO_H__
#define _NICSYS_REFTAO_H__

#include <orbsvcs/CosEventCommS.h>
#include <orbsvcs/CosEventChannelAdminC.h>

#include <orbsvcs/CosNotifyCommS.h>
#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNamingS.h>
#include <orbsvcs/Notify/MonitorControlExt/MonitorEventChannelFactory.h>
#include <orbsvcs/CosNotifyCommC.h>
#include <orbsvcs/CosNotifyChannelAdminC.h>
#include <orbsvcs/CosNamingC.h>

#include <tao/AnyTypeCode/Any.h>



typedef CORBA::Long (*NIC_EVENT_HANDLER_FUNC)(CORBA::Any &);	//CORBA::Any为实际数据


typedef struct Event_Handler {
	char * domain_name;
	char * type_name;
	char * event_name;
	NIC_EVENT_HANDLER_FUNC func;
}nicEventHandler;

#endif 