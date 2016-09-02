/*
	2006-1-13 
	jhunter@tsinghua.org.cn
*/

#include "precomp.h"

/*
	outout debug message
*/
void net_debug()
{
	if(vbus_system){
		utils_debug("system bus dump:\n");
		((CRtkVBus*)vbus_system)->dbg_dump();
		utils_debug("Transaction queue length: %d\n",  ((CRtkVBus*)vbus_system)->broker.tlist.size());
	}
	if(vbus_rtdb){
		utils_debug("rtdb bus dump:\n");
		((CRtkVBus*)vbus_rtdb)->dbg_dump();
		utils_debug("Transaction queue length: %d\n",  ((CRtkVBus*)vbus_rtdb)->broker.tlist.size());
	}
	if(vbus_config){
		utils_debug("config bus dump:\n");
		((CRtkVBus*)vbus_config)->dbg_dump();
		utils_debug("Transaction queue length: %d\n",  ((CRtkVBus*)vbus_config)->broker.tlist.size());
	}
	if(vbus_operation){
		utils_debug("operation bus dump:\n");
		((CRtkVBus*)vbus_operation)->dbg_dump();
		utils_debug("Transaction queue length: %d\n",  ((CRtkVBus*)vbus_operation)->broker.tlist.size());
	}
}

