 /*
	This module manages configurations related to blocks.
	by J.Hunter, 2003/9/18
*/
#include "precomp.h"

static int _dump_devices(int addr)
{
	int i;
	struct io_device * d;
	devlib_ent_t * dd;

	d = ke_get_device_list(proxy_adapter->kernel);

	if(addr == -1){
		for(i=0; i<IO_MAX_DEVICES; i++, d++){
			if(!(d->features & IO_DEVICE_PRESENT)){
				continue;
			}
			dd = blklib_get_dev_by_id(&d->clsid);
			if(!dd){
				char uid[128];
				f8_uuid_to_string(&d->clsid, uid, sizeof(uid));
				utils_trace("Slot %03d:\tUnknown device %s\n", i, uid);
			}else if(strcmp(dd->name, "NONE")){
				utils_trace("Slot %03d:\t%s\t%s\t%04x\n", i, dd->name, dd->description, d->features);
			}
		}
	}else{
		if(addr <0 || addr > IO_MAX_DEVICES){
			utils_error("Invalid device NO.:%d\n", addr);
			return F8_INVALID_DATA;
		}
		d += addr;
		dd = blklib_get_dev_by_id(&d->clsid);
		if(!(d->features & IO_DEVICE_PRESENT)){
			// utils_trace("Slot %d:\tNONE\n", addr);
		}else if(!dd){
			char uid[128];
			f8_uuid_to_string(&d->clsid, uid, sizeof(uid));
			utils_trace("Slot %03d:\tUnknown device %s\n", addr, uid);
		}else{
			utils_trace("Slot %03d:\t%s\t%s\t%04x\n", addr, dd->name, dd->description, d->features);
		}
	}
	return 0;
}

/*
	usage :

	- device
	list all configured devices

	- device <addr> <Type>
	config a device at an address

	- device <addr>
	show device info at specified address
*/
int on_device(char *v, void * k)
{
	CArgs args;
	f8_u16 no;
	io_device d;
	devlib_ent_t * dd;
	f8_uuid did;

	_shproc_prolog_();

	memset(&d, 0, sizeof(d));
	
	parse_arg_ex(v, &args, FS);

	if(args.argc == 1){
		return _dump_devices(-1);
	}else if(args.argc == 2){
		return _dump_devices(atoi(args.argv[1]));
	}

	no = atoi(args.argv[1]);
	if(no >= IO_MAX_DEVICES){
		utils_error("Invalid device number : %s\n", args.argv[1]);
		return F8_INVALID_DATA;
	}
	
	if(!strcmpi(args.argv[2], "none")){
		d.features = 0;
	}else{
		dd = blklib_get_dev_by_name(args.argv[2]);
		if(!dd){
			f8_uuid_from_string(&did, args.argv[2]);
			dd = blklib_get_dev_by_id(&did);
		}
		if(!dd){
			utils_error("Unknown device type : %s\n", args.argv[2]);
			return F8_INVALID_DATA;
		}
		d = dd->dev;
		d.addr = no;
		d.features = dd->dev.features | IO_DEVICE_PRESENT;
	}

	if(args.argc >= 4){
		int configLen=DEV_CONFIG_LEN;
		byte_array_by_hex_string(args.argv[3], d.configData, &configLen);
	}
	
	no = 1;
	__vcall__(s, put, (s, &no, sizeof(no)));
	__vcall__(s, put, (s, &d, sizeof(d)));

	cmd.cmd = F8_SET_DEVICE;
	cmd.isize = ms.pointer;
	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));

	if(!successful){
		return cmd.retcode;
	}

	return F8_SUCCESS;
}


static bool isStationConfigured(int s)
{
	struct io_device * dev;
	int i;
	dev=s*IO_DEVICE_PER_STATION+ke_get_device_list(proxy_adapter->kernel);
	for(i=0;i<IO_DEVICE_PER_STATION;i++,dev++){
		if(dev->features & IO_DEVICE_PRESENT){
			return true;
		}
	}
	return false;
}

int on_station(char *v, void * k)
{
	int i;
	struct module_status_t buf[IO_MAX_DEVICES+IO_MAX_STATIONS];
	struct station_status_t * sst;

	_shproc_prolog_();

	if(!(proxy_adapter->flags & ADAPTER_F_ONLINE)){
		return F8_ADAPTER_OFFLINE;
	}

	cmd.cmd=F8_GET_DEV_STATUS;
	cmd.osize=sizeof(buf);
	cmd.obuffer=buf;
	__vcall__(proxy_adapter, exec, (proxy_adapter, &cmd, psp->timeout));
	if(!successful){
		return cmd.retcode;
	}
	
	sst=(struct station_status_t*)buf;
	utils_trace("NUM   RD    OK  FAIL   PCT       TM(MAX/MIN)    WRT    OK  FAIL PCT     TM(MAX/MIN)\n");
	for(i=0;i<IO_MAX_STATIONS;i++,sst++){
		char idName[32];
		if(isStationConfigured(i)){
			f8_uuid_to_string(&sst->dev.id, idName, sizeof(idName));
			utils_trace("%02d ", i, idName);
			utils_trace(
				"%5.0f %5.0f %5.0f %4.0f%% %.2fms(%5.2f/%.2f) ",
				sst->readCount,
				sst->readCount-sst->readFailCount,
				sst->readFailCount,
				sst->readCount>1? (sst->readCount-sst->readFailCount)*100/sst->readCount : 0,
				sst->readTime,
				sst->readMaxTime,
				sst->readMinTime
				);
			utils_trace(
				"%5.0f %5.0f %5.0f %4.0f%% %.2f(%5.2f/%.2f))\n",
				sst->writeCount,
				sst->writeCount-sst->writeFailCount,
				sst->writeFailCount,
				sst->writeCount>1? (sst->writeCount-sst->writeFailCount)*100/sst->writeCount : 0,
				sst->writeTime,
				sst->writeMaxTime,
				sst->writeMinTime
				);
		}
	}

	return F8_SUCCESS;
}

int on_cpu(char *v, void * k)
{
	struct kstatus_t st;

	_shproc_prolog_();

	if(!(proxy_adapter->flags & ADAPTER_F_ONLINE)){
		return F8_ADAPTER_OFFLINE;
	}
	cmd.cmd = F8_GET_STATUS;	
	cmd.isize = 0;	
	cmd.obuffer = (char *)&st;	
	cmd.osize = sizeof(st);	
	__vcall__(proxy_adapter, exec_remote, (proxy_adapter, &cmd, 10000));	
	if(!F8_SUCCESSED(cmd.retcode)){		
		return cmd.retcode;	
	}

	char buf[128];
	ke_time_to_string(st.upTime,buf,sizeof(buf));
	utils_trace("System started on %s\n", buf);
	ke_time_to_string(st.curr_time,buf,sizeof(buf));
	utils_trace("Current time is %s\n", buf);
	ke_reltime_to_string(st.curr_time-st.upTime,buf,sizeof(buf));
	utils_trace("System has been up for %s\n", buf);
	utils_trace(
		"TIMES: %.0f cycles, %.1f per BC(%.1f+%.1f+%.1f+%.1f), timeunit=millisecond\n", 
		st.n_cycles,
		st.n_phaseTimes[4]/10.,
		st.n_phaseTimes[0]/10.,
		st.n_phaseTimes[1]/10.,
		st.n_phaseTimes[2]/10.,
		st.n_phaseTimes[3]/10.
		);

	f8_uuid_to_string(&st.prog_id, buf, sizeof(buf));
	utils_trace("ProgId:%s\n", buf);
	utils_trace("State:");
	switch(st.state){
	case KERN_S_RUNNING:
		utils_trace("Run\n");
		break;
	case KERN_S_STOPPED:
		utils_trace("Stop\n");
		break;
	case KERN_S_STANDBY:
		utils_trace("Standby\n");
		break;
	case KERN_S_ARBITRATING:
		utils_trace("Arbitrating\n");
		break;
	case KERN_S_HALTED:
		utils_trace("Halt\n");
		break;
	}
	utils_trace("Enabled features:");
	if(st.flags & KERN_F_ENABLE_INPUT)
		utils_trace("IN ");
	if(st.flags & KERN_F_ENABLE_OUTPUT)
		utils_trace("OUT ");
	if(st.flags & KERN_F_ENABLE_STANDBY)
		utils_trace("STANDBY ");
	if(st.flags & KERN_F_ENABLE_SCHEDULER)
		utils_trace("SCHED ");
	if(st.flags & KERN_F_ENABLE_AGENT)
		utils_trace("AGENT ");
	if(st.flags & KERN_F_SERIALIZED_IO)
		utils_trace("SERIALIZED-IO ");
	if(st.flags & KERN_F_ENABLE_WATCHDOG)
		utils_trace("WATCHDOG ");
	utils_trace("\n");
	if(st.flags & (1<<FKERN_KEY_LOCK))
		utils_trace("Controller is Locked\n");
	utils_trace("StandbyID:");
	if(st.flags & (1<<FKERN_KEY_A_B))
		utils_trace("B\n");
	else
		utils_trace("A\n");
	return F8_SUCCESS;
}

