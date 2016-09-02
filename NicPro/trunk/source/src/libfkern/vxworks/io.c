/*
	Linux and VxWorks F8 kernel IO module
	The F8 field bus IO protocol implementation
	
	2004/5/10 
	created panxh@actech-mc.com

	2004/5/11
	IO test mode added

	2006/5/25
	dual-8019 IO channel implemented
*/
#include "../precomp.h"

#if 1
#undef LOCAL
#define LOCAL 
#endif

#pragma hdrstop

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <vxworks.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <etherLib.h>
#include <end.h>
#include <semLib.h>

#define MAX_PKT_LEN 2048

#define CMD_NULL_TYPE 0x0
#define CMD_READ_DATA 0x1
#define CMD_WRITE_DATA 0x2
#define CMD_READ_CONFIG 0x3
#define CMD_WRITE_CONFIG 0x4
#define CMD_COMM_STAT 0x5
#define CMD_STATUS_BROADCAST 0x6

#define CMD_FLAG_ACK 0x80

/*
	message passed to/from rabbit board
*/
struct io_msg_t{
	f8_u8	addr; 	/* */
	f8_u8	flags;	/* low 4 bits is the msg type, high 4 bit the flags */
	f8_u16    seq;		/* sequential num */
	f8_u16	wParam;
	f8_u16	reserved;
	f8_u32	lParam;
	f8_u32	reserved2;
}PACKED;

struct io_cell_data{
	f8_uuid id;
	f8_u8 data[DEV_DATA_LEN];
}PACKED;

struct io_cell_config{
	f8_uuid id;
	f8_u8 data[DEV_CONFIG_LEN];
}PACKED;

#define IO_CHANNEL_OVERRUN 0x1

struct io_channel{
	struct ifnet * channel;
	int flags;
	int bufferLength;
	char buffer[2048];
	SEMAPHORE sem;
	int sempoll;
};

struct io_channel ioChannels[2];
int ioReadTimeout=5;
int ioWriteTimeout=5;
LOCAL long long ioBaseTime;

int activeRoute;
__bool hookDebug;
__bool semDebug;

/*
	check if there is IO activities originated by another controller
	are connected on a single IO bus.
*/
LOCAL __bool ioCheckCollision(struct ether_header *h, int length)
{
	struct io_msg_t * m;
	struct kstatus_t * st;

	//printf("got here 3\n");
	
	if(length == sizeof(*m)+sizeof(*st)+sizeof(*h)){
		m=(struct io_msg_t*)&h[1];
		if(m->flags == CMD_STATUS_BROADCAST){
			st=(struct kstatus_t*)&m[1];
			g_kernel.peer->eventProc(HSE_PEER_STATUS_BROADCAST,h,(int)st,0,0);
		}
	}
	
	return __true;
}

/*
	ethernet input hook procedure
	N.B., invoked in netTask's context, no floating point is allowed
*/
LOCAL STATUS inputHook(int cc, char * buffer, int length)
{
	struct ether_header * h=(struct ether_header *)buffer;
	struct io_channel * c;
	long long t2;

	// ** this function have been modified by cj @ 2010-1-17 for a stupid error.
	c=&ioChannels[cc];

	switch(h->ether_type){
	case ACTECH_ETHER_TYPE + ACTECH_IO_PROTOCOL:
		/* COPY it to buffer */
		if(length > sizeof(*h)){
			memcpy(c->buffer, buffer+sizeof(*h), length-sizeof(*h));
			if(c->bufferLength){
				c->flags |= IO_CHANNEL_OVERRUN;
			}
			c->bufferLength = length-sizeof(*h);
#ifdef LX800
#else
			semGive(&c->sem);
#endif
		}
		c->sempoll= 1;
		break;
	case ACTECH_ETHER_TYPE + ACTECH_STANDBY_PROTOCOL:
		ioCheckCollision(h,length);
		c->sempoll= 1;
		break;
	default:
		return ERROR;
	}
	return OK;
}

LOCAL volatile int ioThreads;
LOCAL volatile int ioCmd;
LOCAL volatile f8_u8 ioCmdFlag;

#define lockChannel(c) sysIoChannelDisable(c)
#define unlockChannel(c) sysIoChannelEnable(c)

LOCAL f8_status flushChannel(int c)
{
#ifdef LX800
	do{
		ioChannels[c].sempoll=0;
		ne2kInt(c);
	}while(ioChannels[c].sempoll!=0);
#endif	
	ioChannels[c].bufferLength = 0;
	ioChannels[c].flags = 0;
	return F8_SUCCESS;
}

LOCAL f8_status writeChannel(int c, struct io_msg_t * data, int len, int protocol)
{
	struct ether_header h;
	STATUS st;
	char buf[MAX_PKT_LEN];

#ifdef	LX800
	flushChannel(c);
#else
	lockChannel(c);
	if(ioChannels[c].bufferLength)
		semTake(&ioChannels[c].sem,0);
	flushChannel(c);
	unlockChannel(c);
#endif

	memcpy(h.ether_dhost, ACTECH_ETHER_ID, 6);
	h.ether_dhost[5] = data->addr;
	h.ether_type = protocol;
	if(!ioChannels[0].channel){
		init_io();	
	}

	memcpy(h.ether_shost, ACTECH_ETHER_ID, 6);
	h.ether_shost[5]=0x8A+(ke_get_flag(FKERN_KEY_A_B)?1:0);
	// h.ether_shost[5]=0x80;
	memcpy(buf, &h, sizeof(struct ether_header));
	memcpy(buf+sizeof(struct ether_header), data, len);

	st = sysWriteNe2000(c, buf, sizeof(struct ether_header)+len,3);

#ifdef	LX800
	ne2kInt(c);
#endif

	return st==OK? F8_SUCCESS : F8_PATH_NOT_FOUND;
}

LOCAL f8_status expectChannel(int c, void * data, int *len, int timeout)
{	
#ifdef LX800
	while(timeout-- > 0){
		miliSleep(1);
		ne2kInt(c);
		if(ioChannels[c].bufferLength>0){
			if(*len >= ioChannels[c].bufferLength){
				*len=ioChannels[c].bufferLength;
				memcpy(data, ioChannels[c].buffer, *len);
			}
			flushChannel(c);
			return F8_SUCCESS;
		}
	}
#else
	if(semTake(&ioChannels[c].sem,timeout) == OK){
		lockChannel(c);
		if(*len >= ioChannels[c].bufferLength){
			*len=ioChannels[c].bufferLength;
			memcpy(data, ioChannels[c].buffer, *len);
		}
		flushChannel(c);
		unlockChannel(c);
		return F8_SUCCESS;
	}
#endif
	return F8_TIMEOUT;
}

/* read/write time statistics buffer */
struct average_array_t rab[IO_MAX_STATIONS], wab[IO_MAX_STATIONS];

LOCAL f8_status readStation(int s, BOOL force)
{
	struct io_msg_t *m;
	struct io_cell_data * a;
	char buf[MAX_PKT_LEN];
	int len;
	struct f8_u8 * ioMem;
	struct io_device * dev;
	int i,k,j;
	BOOL stationConfigured=FALSE;
	struct station_status_t * st=&g_kernel.stationStatus[s];
	struct module_status_t * mst;
	f8_u8 route=st->route;
	f8_r8 t, t2;
	f8_u16 seq;
	//** added by cj, get io module status @ 2009-6-19
	static unsigned int rsFail[IO_MAX_STATIONS];
	//**
	
	dev=s*IO_DEVICE_PER_STATION+g_kernel.devices;
	if(!force){
		for(i=0;i<IO_DEVICE_PER_STATION;i++,dev++){
			//** modified by cj @ 2009-6-19
			//if((dev->features & IO_DEVICE_PRESENT) && (dev->features & IO_DEVICE_INPUT)){
			if((dev->features & IO_DEVICE_PRESENT)){
			//**
				stationConfigured=TRUE;
				break;
			}			
		}
		if(!stationConfigured){
			return F8_SUCCESS;
		}
	}
	
	/* issue read-data command */
	m=(struct io_msg_t*)buf;
	m->addr = s;
	m->flags = CMD_READ_DATA;
	seq=(f8_u16)pentiumTscGet32();
	m->seq = seq;

	st->readCount=st->readCount+1;
	t=sysTimeMark();	
	if(writeChannel(route, m, sizeof(*m),ACTECH_ETHER_TYPE+ACTECH_IO_PROTOCOL) == F8_SUCCESS){
		len=sizeof(buf);
		if(expectChannel(route, buf, &len, ioReadTimeout) == F8_SUCCESS){		
			if(m->seq == seq && 
				m->flags==(CMD_FLAG_ACK|CMD_READ_DATA) &&
				len==sizeof(struct io_cell_data)*9+sizeof(*m) &&
				m->addr==s
				){
				/* read successful */
				rab[s].M1=ioReadTimeout;
				t2=sysTimeMark()-t;
				st->readTime=ki_average(&rab[s], t2);
				st->readMaxTime=rab[s].M;
				st->readMinTime=rab[s].m;
				k=s*IO_DEVICE_PER_STATION;
				dev=k+g_kernel.devices;
				//** added by cj, get io module status @ 2009-6-19
				rsFail[s]=0;				
				//**
				
				a=(struct io_cell_data*)&m[1];
				
				// update station status 
				st->dev.id=a->id;
				memcpy(st->dev.data,a->data,DEV_DATA_LEN);				
				a++;

				mst=&g_kernel.moduleStatus[s*IO_DEVICE_PER_STATION];
				for(j=0;j<IO_DEVICE_PER_STATION;j++,a++,dev++,mst++,k++){
					/* update device status */
					memcpy(
						mst->dev.data,
						a->data,
						DEV_DATA_LEN
						);
					mst->dev.id=a->id;
					/* import data from device */
					if(a->id == dev->clsid && 
						(dev->features & IO_DEVICE_INPUT) &&
						(dev->features & IO_DEVICE_PRESENT)
						){
						memcpy(
							g_kernel.io_memory[k],
							a->data,
							DEV_DATA_LEN
							);
					}else{
						//**removed by cj @ 2009-6-19
						//memset(g_kernel.io_memory[k],0,DEV_DATA_LEN);
						//**
					}
				}
				return F8_SUCCESS;
			}else{
				/*
				if(semDebug)
					printf("IORead[%d],Failed,Route[%d], code 1\n", s,route);
				*/
				st->readFailCount=st->readFailCount+1;
			}
		}else{
			/*
			if(semDebug)
				printf("IORead[%d],Failed,Route[%d], code 2\n", s,route);
			*/
			//** added by cj, get io module status @ 2009-6-19
			rsFail[s]++;
			if(rsFail[s]>10){
				rsFail[s]=10;
				//**added by cj @ 2009-6-19
				mst=&g_kernel.moduleStatus[s*IO_DEVICE_PER_STATION];
				for(j=0;j<IO_DEVICE_PER_STATION;j++,mst++){
					memset(mst,0,sizeof(struct module_status_t));
				}
				//**
			}
			//**
			st->readFailCount=st->readFailCount+1;
		}
	}else{
		st->readFailCount=st->readFailCount+1;
	}

	// com failed, switch to another route
	st->route=1-st->route;
	
	return F8_PATH_NOT_FOUND;
}

LOCAL f8_status writeStation(int s, BOOL force)
{
	/* build a write command packet */
	struct io_msg_t * m;
	struct io_cell_data * c;
	struct io_device * d;
	int i;
	char dbuf[MAX_PKT_LEN];
	int len;
	BOOL stationConfigured=FALSE;
	f8_u8 route=g_kernel.stationStatus[s].route;
	f8_r8 t,t2;
	struct station_status_t * st=&g_kernel.stationStatus[s];
	struct module_status_t * mst;
	f8_u16 seq;

	
	d=s*IO_DEVICE_PER_STATION + g_kernel.devices;
	if(!force){
		for(i=0;i<IO_DEVICE_PER_STATION;i++,d++){
			if((d->features & IO_DEVICE_PRESENT) && (d->features & IO_DEVICE_OUTPUT)){
				stationConfigured=TRUE;
				break;
			}
		}
		if(!stationConfigured){
			return F8_SUCCESS;
		}
	}
	
	m = (struct io_msg_t*)dbuf;
	c = (struct io_cell_data*)&m[1];
	memset(dbuf, 0, sizeof(dbuf));
	m->addr = s;
	m->flags = CMD_WRITE_DATA;
	seq=(f8_u16)pentiumTscGet32();
	m->seq = seq;
	
	/* prepare data */
	c++; /* skip data for comm card */
	d=s*IO_DEVICE_PER_STATION + g_kernel.devices;
	for(i=0; i<IO_DEVICE_PER_STATION; i++,d++,c++){
		if((d->features & IO_DEVICE_PRESENT) && (d->features & IO_DEVICE_OUTPUT)){
			c->id = d->clsid;
			memcpy(c->data, g_kernel.io_memory[d->addr], 24);
		}
	}
	
	st->writeCount=st->writeCount+1;
	t=sysTimeMark();
	
	if(writeChannel(route, m, sizeof(*m)+sizeof(*c)*9,ACTECH_ETHER_TYPE+ACTECH_IO_PROTOCOL) == F8_SUCCESS){
		len=sizeof(dbuf);
		if(expectChannel(route, dbuf, &len, ioWriteTimeout) == F8_SUCCESS){		
		if(m->seq == seq && m->flags==(CMD_FLAG_ACK|CMD_WRITE_DATA) && m->addr==s){
				wab[s].M1=ioWriteTimeout;
				t2=sysTimeMark()-t;
				/*
				if(semDebug)
					printf("Add write average entry:%d,%f\n",s,t2);
				*/
				st->writeTime=ki_average(&wab[s], t2);
				st->writeMaxTime=wab[s].M;
				st->writeMinTime=wab[s].m;
				return F8_SUCCESS;
			}else{
				st->route=1-st->route;
				st->writeFailCount=st->writeFailCount+1;
			}
		}else{
			/* one route to this station failes, switch to another route */
			st->route=1-st->route;
			st->writeFailCount=st->writeFailCount+1;
		}
	}else{
		/* one route to this station failes, switch to another route */
		st->route=1-st->route;
		st->writeFailCount=st->writeFailCount+1;
	}
	
	return F8_PATH_NOT_FOUND;
}
	
LOCAL int busInput(struct kernel_t * kernel)
{
	int i;
	static int station;
	
	if(kernelState==KERN_S_RUNNING){	
		for(i=0;i<16;i++){
			readStation(i, FALSE);
		}
	}

	io_read_config(station);
	station++;
	if(station >= IO_MAX_STATIONS){
		station=0;
	}
	
	return 0;
}

LOCAL int busOutput(struct kernel_t * kernel)
{
	int i;
	static int d;

	if(kernelState==KERN_S_RUNNING){
		for(i=0;i<16;i++){
			writeStation(i, FALSE);
		}

		// after 128 base cycles, all devices config will written
		// this keeps device config synchronized with controller
		io_write_config(&g_kernel.devices[d]);
		d++;
		if(d>=IO_MAX_DEVICES){
			d=0;
		}
	}
	
	return 0;
}

LOCAL int ioBusProcessor()
{
	while(ioCmd != 3){
		switch(ioCmd){
		case 0:
			/* no command */
			break;
			
		case 1:
			/* phase 0, read command issued for this bus */
			if(!ioCmdFlag){
				busInput(&g_kernel);
				ioCmdFlag=1;
			}
			break;
			
		case 2:
			/* phase 2, write command issued for this bus */
			if(!ioCmdFlag){
				busOutput(&g_kernel);
				ioCmdFlag= 1;
			}
			break;
			
		case 3:
			/* exit command issued for this bus */
			break;
		}
		miliSleep(1);
	}
	ioCmdFlag = 1;
	ioThreads = -1;
}

/*
	synchronizing io memory with device memory
	for each configured device, issue a read command,
	wait for ack, and copy returned data.
*/
void process_input(struct kernel_t * kernel)
{
	if(kernel->status.flags & KERN_F_SERIALIZED_IO){
		busInput(kernel);
	}else{
		int volatile * pFlag;
		ioCmd = 1;
		ioCmdFlag = 0;
		while(ioCmdFlag != 0x1 && (taskFlag != TSK_STOPPING)){
			miliSleep(1);
		}
		ioCmd = 0;
	}
}

/*
	synchronizing device memory with io memory
*/
void process_output(struct kernel_t * kernel)
{
	if(kernel->status.flags & KERN_F_SERIALIZED_IO){
		busOutput(kernel);
	}else{
		int volatile * pFlag;
		ioCmd = 2;
		ioCmdFlag = 0;
		while(ioCmdFlag != 0x1 && (taskFlag != TSK_STOPPING)){
			miliSleep(1);
		}
		ioCmd = 0;
	}
}

void io_dump_memory(char *buffer, int base, int len)
{
	int i, j;
	unsigned char * p = (unsigned char*)buffer;
	for(i=0; i<len - 16; i+=16, p+=16){
		printf("%08x: ", base+i); 
		for(j=0; j<16; j++){
			printf("%02x ", p[j]);
		}
		for(j=0; j<16; j++){
			if(isgraph(p[j])){
				putchar(p[j]);
			}else{
				putchar('.');
			}
		}
		putchar('\n');
	}
	printf("%08x: ", base+i);
	for(j=i; j<len; j++){
		printf("%02x ", p[j]);
	}
	for(j=i; j<len; j++){
		if(isgraph(p[j])){
			putchar(p[j]);
		}else{
			putchar('.');
		}
	}
	putchar('\n');
}

f8_status io_write_config(const struct io_device * d)
{
	/* build a write command packet */
	struct io_msg_t * m;
	struct io_cell_config * c;
	int i;
	char dbuf[MAX_PKT_LEN];
	int len;
	f8_u16 seq=pentiumTscGet32();

	if(!(d->features & IO_DEVICE_PRESENT))
		return F8_ACCESS;
	
	m = (struct io_msg_t*)dbuf;
	c = (struct io_cell_config*)&m[1];
	memset(dbuf, 0, sizeof(dbuf));
	m->addr = d->addr >> 3;
	m->flags = CMD_WRITE_CONFIG;
	m->seq = seq;
	m->wParam=d->addr & 0x7;

	/* prepare data */
	c->id = d->clsid;
	assert(DEV_CONFIG_LEN==sizeof(c->data));
	memcpy(c->data, d->configData, DEV_CONFIG_LEN);
	writeChannel(activeRoute, m, sizeof(*m)+sizeof(*c),ACTECH_ETHER_TYPE+ACTECH_IO_PROTOCOL);
	len=sizeof(dbuf);
	if(expectChannel(activeRoute, dbuf, &len, ioWriteTimeout) == F8_SUCCESS){
		if(m->seq == seq && m->flags==(CMD_FLAG_ACK|CMD_WRITE_CONFIG)){
			return F8_SUCCESS;
		}
	}
	return F8_GENERAL_CONFIG_ERR;
}

/*
	read station config data(and that of pertaining modules)
*/
f8_status io_read_config(int s)
{
	/* build a write command packet */
	struct io_msg_t * m;
	struct io_cell_config * c;
	int i;
	char dbuf[MAX_PKT_LEN];
	int len;
	int route;
	struct module_status_t * t;
	f8_u16 seq=pentiumTscGet32();
	
	m = (struct io_msg_t*)dbuf;
	memset(dbuf, 0, sizeof(dbuf));
	m->addr = s;
	m->flags = CMD_READ_CONFIG;
	m->seq = seq;
	route=g_kernel.stationStatus[s].route;
	
	writeChannel(route, m, sizeof(*m),ACTECH_ETHER_TYPE+ACTECH_IO_PROTOCOL);
	len=sizeof(dbuf);
	if(expectChannel(route, dbuf, &len, ioReadTimeout) == F8_SUCCESS){
		if(m->seq == seq && m->flags==(CMD_FLAG_ACK|CMD_READ_CONFIG) &&
			len==sizeof(*c)*9 + sizeof(*m)
			){
			c=(struct io_cell_config*)&m[1];
			g_kernel.stationStatus[s].dev.id=c->id;
			memcpy(g_kernel.stationStatus[s].dev.configData,c->data, DEV_CONFIG_LEN);
			//** removed by cj @ 2009-6-19
			//t=&g_kernel.moduleStatus[s*IO_DEVICE_PER_STATION];
			//for(i=0;i<IO_DEVICE_PER_STATION;i++,c++,t++){
			//	t->dev.id=c->id;
			//	memcpy(t->dev.configData, c->data, DEV_CONFIG_LEN);
			//}
			//**
			return F8_SUCCESS;
		}
	}
	return F8_GENERAL_CONFIG_ERR;
}

void start_io()
{
	// clear device status
	memset(&g_kernel.moduleStatus,0,sizeof(g_kernel.moduleStatus));
	memset(&g_kernel.stationStatus,0,sizeof(g_kernel.stationStatus));
	/*
		issue start IO command to IO processor
	*/
	ioCmd = 0;
	if(g_kernel.status.flags & KERN_F_SERIALIZED_IO){
		ioThreads = -1;
	}else{
		char taskName[16];
		sprintf(taskName, "IOThread");
		ioThreads = taskSpawn(
			taskName,
			255,			/* priority */
			VX_FP_TASK,		/* options */
			12*1024,		/* stack size */
			ioBusProcessor,		/* entry point */
			0, 0, 0,0,0,0,0,0,0,0	/* arg 1~10 */
		);
	}

	//printf("IO layer started.\n");
}

void stop_io()
{
	/*
		halt the IO processor
	*/
	int i;
	ioCmd = 3;

	if(!(g_kernel.status.flags & KERN_F_SERIALIZED_IO)){
		while(ioThreads != -1){
			miliSleep(1);
		}
	}
	ioCmdFlag = 0;
}

f8_int init_io()
{
	f8_u8 ab;
	char addr[32];
	int i;
	static f8_r8 rBuf[IO_MAX_STATIONS][20],wBuf[IO_MAX_STATIONS][20];

	ioChannels[0].channel=(struct ifnet*)1;
	sysIoChannelHook(0, inputHook);
	sysIoChannelHook(1, inputHook);	
#ifdef LX800
	sysIoChannelDisable(0);
	sysIoChannelDisable(1);
	ioChannels[0].sempoll=0;
	ioChannels[1].sempoll=0;
#else
	semBInit(&ioChannels[0].sem, SEM_Q_PRIORITY, SEM_EMPTY);
	semBInit(&ioChannels[1].sem, SEM_Q_PRIORITY, SEM_EMPTY);
#endif

// initialize time statistics stuff
	for(i=0;i<IO_MAX_STATIONS;i++)
		ki_init_average(&rab[i],&rBuf[i][0],20,ioReadTimeout,1);
	for(i=0;i<IO_MAX_STATIONS;i++)
		ki_init_average(&wab[i],&wBuf[i][0],20,ioWriteTimeout,1);
	
	pentiumTscGet64(&ioBaseTime);
	
	return F8_SUCCESS;
}

f8_int uninit_io()
{
	ioChannels[0].channel=0;
	ioChannels[1].channel=0;
	sysIoChannelUnhook(0);
	sysIoChannelUnhook(1);
	return F8_SUCCESS;
}

/*
* 
*/
f8_status io_write_status()
{
	char buf[sizeof(struct io_msg_t)+sizeof(struct kstatus_t)];
	struct io_msg_t * m=(struct io_msg_t*)buf;
	struct kstatus_t * s=(struct kstatus_t*)&m[1];
	memset(buf,0,sizeof(buf));
	m->flags=CMD_STATUS_BROADCAST;
	m->addr=0x8A+(ke_get_flag(FKERN_KEY_A_B)?0:1);;
	*s=g_kernel.status;
	writeChannel(0, m, sizeof(buf),ACTECH_ETHER_TYPE+ACTECH_STANDBY_PROTOCOL);
	writeChannel(1, m, sizeof(buf),ACTECH_ETHER_TYPE+ACTECH_STANDBY_PROTOCOL);
}

LOCAL f8_status writeDevice(struct io_device * d, char * buf)
{
	/* build a write command packet */
	struct io_msg_t * m;
	struct io_cell_data * c;
	int i;
	char dbuf[MAX_PKT_LEN];
	int len;
	long long t,t2;
	f8_status cod;
	f8_u16 seq=(f8_u16)pentiumTscGet32();

	m = (struct io_msg_t*)dbuf;
	c = (struct io_cell_data*)&m[1];
	memset(dbuf, 0, sizeof(dbuf));
	m->addr = d->addr >> 3;
	m->flags = CMD_WRITE_DATA;
	m->seq = seq;
	c += (d->addr & 7)+1;
	c->id = 0x12345678;
	memcpy(c->data, buf, 2);

	pentiumTscGet64(&t);
	//printf("%.3f ms, write channel ... ", (double)(t-ioBaseTime)/microsecondCount/1000);
	cod=writeChannel(activeRoute, m, sizeof(*m)+sizeof(*c)*9,ACTECH_ETHER_TYPE+ACTECH_IO_PROTOCOL);
	pentiumTscGet64(&t2);
	//printf("done, code=%d, t=%f ms.\n", cod, (double)(t2-t)/microsecondCount/1000);

	//printf("Awaiting ack...");
#if 1
	len=sizeof(dbuf);
	if((cod=expectChannel(activeRoute, dbuf, &len, ioWriteTimeout)) == F8_SUCCESS){
		//pentiumTscGet64(&t2);
		//printf("done, code=%d, t=%f ms.\n", cod, (double)(t2-t)/microsecondCount/1000);
		if(m->seq == seq && m->flags==(CMD_FLAG_ACK|CMD_WRITE_DATA)	){
			return F8_SUCCESS;
		}
	}else{
		//printf("Failed %d.\n", cod);
	}
#endif

	return F8_TIMEOUT;
}



/*
	digital output test
*/
int writeTest(char * para)
{
	int devid;
	int ret;
	int value;
	char * p;
	struct io_device dev;
	long long t;
	char buf[IO_SIZE_PER_SLOT];

	p = strtok(para, " \t\n");
	if(!p){
		return -1;
	}
	devid = atoi(p);
	p = strtok(NULL, " \t\n");
	if(!p){
		return -2;
	}
	value = strtoul(p, NULL, 16);

	memset(&dev, 0, sizeof(dev));
	dev.addr = devid;
	dev.n_do = 16;
	memset(buf, 0, sizeof(buf));
	*((f8_u16*)buf) = (f8_u16)value;
	ret = writeDevice(&dev, buf);
	return ret;
}

int w2(int id, int dtime, int loop)
{
	int i;
	static int shift=0x5555;
	char buf[32];
	int ok, ret;
	ok = 0;
	if(loop <0){
		loop = 1024;
	}
	for(i=0; i<loop; i++){
		sprintf(buf, "%d %04x", id, shift & 0xffff);
		ret = writeTest(buf);
		if(ret > 0){
			ok++;
		}
		shift <<= 1;
		if(shift & 2){
			shift &= ~1;
		}else{
			shift |= 1;
		}
		if(dtime >0){
			miliSleep(dtime);
		}
	}
	// printf("%d out of %d ok.\n", ok, loop);
	return ok;
}

int readTest(int devId, const char * dt)
{
	f8_status e;
	g_kernel.stationStatus[devId>>3].route=activeRoute;
	if((e=readStation(devId >> 3,TRUE)) == F8_SUCCESS){
		int i;
		for(i=0;i<DEV_DATA_LEN;i++){
			printf("%02x", g_kernel.moduleStatus[devId].dev.data[i]);
		}
		putchar('\n');
	}else{
		printf("Failed %d.\n", e);
	}
	return 0;
}

int readTest2(int devId, int dlay, int loop)
{
	if(loop<0) loop=10;
	while(loop--){
		readTest(devId, 0);
		taskDelay(dlay);
	}
	return 0;
}

int readTest3(int s)
{
	int i;
	for(i=s*IO_DEVICE_PER_STATION;i<s*IO_DEVICE_PER_STATION+IO_DEVICE_PER_STATION;i++){
		readTest(i,0);
	}
	return 0;
}

int configTest(int devid, const char * dt)
{
	struct io_device d;
	memset(&d, 0, sizeof(d));
	d.addr = devid;
	strcpy(d.configData, dt);
	return io_write_config(&d);
}

int configTest2(int devId)
{
	int s;
	struct dev_status_t * t;
	struct module_status_t * mst;
	struct station_status_t * sst;
	if(devId < 0){
		s=-devId;
		sst=g_kernel.stationStatus+s;
		t=(struct dev_status_t*)sst;
	}else{
		s=devId>>3;
		mst=g_kernel.moduleStatus+devId;
		t=(struct dev_status_t*)mst;
	}
	g_kernel.stationStatus[s].route=activeRoute;
	if(io_read_config(s)==F8_SUCCESS){
		int i;
		f8_u16 * is;
		is = (f8_u16*)&t->id;
		printf("ID:%04x-%04x-%04x-%04x,CONFIG=", is[0], is[1], is[2], is[3]);
		for(i=0;i<DEV_CONFIG_LEN;i++){
			printf("%02x", t->configData[i]);
		}
		putchar('\n');
		if(devId < 0){
			printf("Read:%.0f(%.0f OK + %.0 Failure), average response time %.3f ms.\n",
				sst->readCount,
				sst->readFailCount,
				sst->readTime
				);
			printf("Write:%.0f(%.0f OK + %.0 Failure), average response time %.3f ms.\n",
				sst->writeCount,
				sst->writeFailCount,
				sst->writeTime
				);
		}
	}else{
		printf("Error reading device config.\n");
	}
	return 0;
}

void stationTest(int s)
{
	struct station_status_t * sst=g_kernel.stationStatus+s;
	printf("Station %d, ActiveChannel:%c\n", s,'A'+sst->route);
	printf("Read:%.0f(%.0f OK + %.0f Failure), average response time %.3f ms.\n",
		sst->readCount,
		sst->readCount-sst->readFailCount,
		sst->readFailCount,
		sst->readTime
		);
	printf("Write:%.0f(%.0f OK + %.0f Failure), average response time %.3f ms.\n",
		sst->writeCount,
		sst->writeCount-sst->writeFailCount,
		sst->writeFailCount,
		sst->writeTime
		);
}

void stationState()
{
	int i;
	for(i=0;i<8;i++){
		stationTest(i);
	}
}

