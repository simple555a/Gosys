#include "precomp.h"

/*
	this is called when a collision on system bus is detected.
*/
void handleCollision(HVBUS bus, PCRTK_PACKET p)
{
	VBUS_DATA d;
	__bool shouldSwitch = __false;
	
	if(get_power_state() != PWR_RUNNING){
		return;
	}
	
	d.size = sizeof(d);
	get_vbus_data(bus, &d);
	assert(p->packet_type == PT_ServerClaim);
	if(!(d.serverGuid < *((RTK_GUID*)p->data))){
		shouldSwitch = __true;
	}
	
	if(shouldSwitch){
		utils_error(
			"Collision detected, peer=%s:%d\n",
			inet_ntoa(d.peerAddr.sin_addr),
			ntohs(d.peerAddr.sin_port)
			);
		utils_error("Initiating primary->backup switch....\n");
		switch_to_backup();
	}
}

/*
	determine if an incoming packet is of interest to
	PMC-node running at server mode.

	in server mode, all buses are treated the same.
*/
int PMC_API server_filter(HVBUS bus, int event, int _p)
{
	PCRTK_PACKET p = (PCRTK_PACKET)_p;
	switch(event){
		case VBUS_EVT_GOT_FRAGMENT:
			break;
		case VBUS_EVT_COLLISION:
			if(bus == vbus_system){
				handleCollision(bus, p);
			}
		default:
			return 0;
	}
	switch(get_power_state()){
	case PWR_RUNNING:
		return default_server_filter(bus, event, _p);
	case PWR_BACKUP:
		// only packets from master site are allowed to pass
		// fake as a client
		if(p->dest.port != get_vbus_port(bus) 
			&& p->dest.port != PORT_ALL_SLAVE
			&& p->dest.port != PORT_ALL
			){
			return __false;
		}

		if(!is_same_key(&p->src.host, &g_ThisNode->key)){
			return __false;
		}
		/* 2005-10-16 jht any primary-site generated packets will
		be a valid statement of primary site's healthy status */
		PrimaryWatchDogCounter = MaxPrimaryWatchDogCounter;
		return __true;		
	}

	return __false;
}

__bool PMC_API power_state_changed(int newState, int context)
{
	static int client_port = alloc_vbus_port();

	power_state_change_alarm(newState);
		
	switch(newState){
	case PWR_BACKUP:
		set_vbus_port(vbus_alarm, client_port);
		set_vbus_port(vbus_rtdb, client_port);
		set_vbus_port(vbus_operation, client_port);
		set_vbus_port(vbus_config, client_port);
		set_vbus_port(vbus_system, client_port);
		reset_vbus_flag(vbus_system, VBUS_ACTIVE_SERVER);
		break;
	case PWR_RUNNING:
		set_vbus_port(vbus_alarm, PORT_ALL_SERVER);
		set_vbus_port(vbus_rtdb, PORT_ALL_SERVER);
		set_vbus_port(vbus_operation, PORT_ALL_SERVER);
		set_vbus_port(vbus_config, PORT_ALL_SERVER);
		set_vbus_port(vbus_system, PORT_ALL_SERVER);
		set_vbus_flag(vbus_system, VBUS_ACTIVE_SERVER);
		break;
	default:
		return __false;
	}

{
	char buf[16];
	sprintf(buf,"%d",newState);
	WritePrivateProfileString("pmc","serverStatus",buf,get_config_file());
}
	
	return __true;
}
