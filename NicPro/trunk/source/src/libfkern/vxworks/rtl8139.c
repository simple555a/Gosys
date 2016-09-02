/*
	poll mode 8139 driver
	panxiaohui@actech-mc.com, 2005-9
*/
#include <taskLib.h>
#include <cacheLib.h>
#include <in.h>
#include <iv.h> /* interrupt vector <-> irq number */

#include "rtl8139.h"

#undef LOCAL
#define LOCAL

struct rtl81x9_device _channel3;
struct rtl81x9_device * channel3=&_channel3;

LOCAL void		read_eeprom	(struct rtl81x9_device *, UCHAR *, ULONG, ULONG, ULONG);
LOCAL void		eeprom_put_byte(struct rtl81x9_device *, ULONG);
LOCAL void		eeprom_get_word(struct rtl81x9_device *, ULONG, USHORT*);

LOCAL void		phy_write_reg (struct rtl81x9_device *, int, int);
LOCAL USHORT	phy_read_reg	(struct rtl81x9_device *, int);
LOCAL int		mii_write_reg (struct rtl81x9_device *, RTL_MII_FRAME *);
LOCAL void		mii_sync 	(struct rtl81x9_device *);
LOCAL void		mii_send 	(struct rtl81x9_device *, ULONG, int);
LOCAL int		mii_read_reg	(struct rtl81x9_device *, RTL_MII_FRAME *);
LOCAL void		auto_neg_tx	(struct rtl81x9_device *);
LOCAL int		mii_auto_neg	(struct rtl81x9_device *);
LOCAL int		media_set 	(struct rtl81x9_device *, int);
LOCAL int		media_config	(struct rtl81x9_device *);

/* Macro to enable the appropriate interrupt level */

LOCAL void rtl81x9_write2(struct rtl81x9_device * d, const void * buf, int len)
{
	char * pBuf;
	int tx_val;

	pBuf = (d->txmemspace + (d->nextDesc * RTL81x9_BUFSIZE));
	len = max (len, 60);	/* ETHERSMALL */
	len = min(len, 1518);
	memcpy(pBuf, buf, len);
	
	tx_val = len + (3 << 16);
	d->last_sent_packet = pBuf;
	d->last_sent_packet_length = len;
	csr_write_long (d, RTL_REGS_TX_ADDR0 + (d->nextDesc * 4), (int) pBuf, RTL_WIN_0); 
	csr_write_long (d, RTL_REGS_TX_STATUS0 + (d->nextDesc * 4), tx_val, RTL_WIN_0);
	d->counters[1]++;
	d->nextDesc++;
	if(d->nextDesc == RTL_NUM_TX_DESC){
		d->nextDesc = 0;
	}
	
	/* Flush the write pipe */
	
	CACHE_PIPE_FLUSH ();
}

void rtl81x9_purge(struct rtl81x9_device * d)
{
	char buf[1514];
	while(rtl81x9_read(d, buf, sizeof(buf), 0) > 0);
}

STATUS rtl81x9_write_ok(struct rtl81x9_device * d)
{
	u_long txstatus;
	int 		   txDesc;
	unsigned int tsad;
	int cnt = 3;

#if 0
	do{
		tsad = csr_read_word (d,RTL_REGS_TX_SUMMARY, NONE);
		taskDelay(1);
	} while ((tsad & 0xf000)==0 && (cnt-->0))	;

	if( (tsad & 0xf000)==0 ){
		return ERROR;
	}
#endif

	txDesc = d->nextDesc;
	txstatus = csr_read_long (d, RTL_REGS_TX_STATUS0 + (txDesc*4), NONE);			
	if (!(txstatus & RTL_TX_HOST_OWNS)){	/*4*/
		int expectedDelay;
		int retry;
		/* on 100Mbps net, the NIC should send 100 bits per microseconds, so ... */
		expectedDelay = (txstatus & 0xfff) * 8 / 100;
		retry = 0;
		while(!(txstatus & RTL_TX_HOST_OWNS) && retry++<10){
			sysStallExecutionProcessor(expectedDelay/10);
			txstatus = csr_read_long (d, RTL_REGS_TX_STATUS0 + (txDesc*4), NONE);	
		}
		if(!(txstatus & RTL_TX_HOST_OWNS)){
			return -1;
		}
	}	/*4*/

	if(!(txstatus & RTL_TX_STATUS_OK) && csr_read_long(d, RTL_REGS_TX_ADDR0 + (txDesc*4), NONE)){
		int expectedDelay;
		int retry;
		/* on 100Mbps net, the NIC should send 100 bits per microseconds, so ... */
		expectedDelay = (txstatus & 0xfff) * 8 / 100;
		retry = 0;
		while(!(txstatus & RTL_TX_STATUS_OK) && retry++<10){
			sysStallExecutionProcessor(expectedDelay/10);
			txstatus = csr_read_long (d, RTL_REGS_TX_STATUS0 + (txDesc*4), NONE);	
		}
		if(!(txstatus & RTL_TX_STATUS_OK)){
			return -2;
		}
	}

	return 0;
}

STATUS rtl81x9_write(struct rtl81x9_device * d, const void * buf, int len)
{
	if(rtl81x9_write_ok(d) == 0){
		rtl81x9_write2(d,buf,len);
	}else{
		return -1;
	}
	return OK;
}

/*
	poll 8139 for packets
*/
int rtl81x9_read(struct rtl81x9_device * d, char * buf, int len, int microseconds)
{
	int retry;
	int wrapSize;
	RTL_RMD *rxData;
	USHORT		cur_rx;
	USHORT		limit;
	USHORT		max_bytes = 0, rx_bytes = 0;
	char		*readPtr;
	int t2;
	u_short stat;

	
	/* In case of an RxFIFOOver we must also clear the RxOverflow
   bit to avoid dropping frames for ever.
	*/
	stat = csr_read_word(d, RTL_REGS_INTR_STATUS, 0);
	if(stat){
		csr_write_word(d, RTL_REGS_INTR_STATUS, 
			(stat & RTL_IPT_RX_FIFO_OVER)? stat | RTL_IPT_RX_OVERFLOW : stat, NONE);
	}
	cur_rx = ((csr_read_word (d, RTL_REGS_RX_BUF_PTR, NONE) + 16) % RTL_RXBUFLEN);
		
	t2 = microseconds/10;
	retry = 0;
	while ((csr_read_byte (d, RTL_REGS_CHIP_CMD, NONE) & RTL_CMD_RX_BUF_EMPTY) == 1 && retry<10){
		sysStallExecutionProcessor(t2);
		retry++;
	}
	
	if((csr_read_byte (d, RTL_REGS_CHIP_CMD, NONE) & RTL_CMD_RX_BUF_EMPTY) == 1){
		return -1;
	}
	
	limit  = (csr_read_word (d, RTL_REGS_RX_BUF_ADDR, NONE) % RTL_RXBUFLEN);
	
	if (limit < cur_rx)
		max_bytes = (RTL_RXBUFLEN - cur_rx) + limit;
	else
		max_bytes = limit - cur_rx;
	
	readPtr = (d->ptrRxBufSpace + cur_rx);
	rxData = (RTL_RMD *) readPtr;
				
	if ((rxData->rxPktLen >> 16) == RTL_RX_UNFINISHED){
		return -2;
	}
		
	if (!(rxData->rxStatusFlags & RTL_RX_STATUS_OK)){
		if (rxData->rxStatusFlags & (RTL_RX_BAD_SYMBOL|
			RTL_RX_RUNT|
			RTL_RX_TOO_LONG|
			RTL_RX_CRC_ERROR|
			RTL_RX_BAD_ALIGN))
		{
			csr_write_byte(d, RTL_REGS_CHIP_CMD, RTL_CMD_TX_ENB, RTL_WIN_0);
			csr_write_byte(d, RTL_REGS_CHIP_CMD, RTL_CMD_TX_ENB|
				RTL_CMD_RX_ENB, RTL_WIN_0);
			csr_write_long(d, RTL_REGS_RX_BUF, (ULONG) d->ptrRxBufSpace, RTL_WIN_0);
			csr_write_word(d, RTL_REGS_RX_BUF_PTR, cur_rx - 16, RTL_WIN_0);
			cur_rx = 0;
		}
		return -3;
	}
		
		
	/* No errors; receive the packet. */

	/* get packet length */
	if(rxData->rxPktLen < len){
		len = rxData->rxPktLen;
	}else if(rxData->rxPktLen > len){
		/* buffer too small */
		return -4;
	}
	
	rx_bytes += len + 4;
	
	/*
	*/
	len -= RTL_CRC_SIZE;
	
	/*
	* Avoid trying to read more bytes than we know
	* the chip has prepared for us.
	*/
	if (rx_bytes > max_bytes){
		/*??? the chip cheats us? */
#if 0
#define _(x) printf(#x "=%08x\n", x)
		_(rx_bytes);
		_(max_bytes);
		_(rxData->rxPktLen);
		_(len);
		_(cur_rx);
		_(limit);
		return -5;
#endif
	}
		
	/* Copy the data found into the new cluster, we have (+4) to get us past the */
	/* data that the rtl chip places at the start of the message and we remove	 */
	/* the CRC from the end of the message	*/
	
	if ((readPtr + len) >= (d->ptrRxBufSpace + RTL_RXBUFLEN)){
		wrapSize = (int) ((readPtr + len) - (d->ptrRxBufSpace + RTL_RXBUFLEN));
		
		/* Copy in first section of message as stored */
		/* at the end of the ring buffer			  */
		
		memcpy (buf, readPtr + 4, len - wrapSize);
		
		/* Copy in end of message as stored */
		/* at the start of the ring buffer	*/
		
		/* memcpy (buf+len - wrapSize, d->ptrRxBufSpace, wrapSize); */
		memcpy (buf+len - wrapSize -4, d->ptrRxBufSpace, wrapSize+0x04);
		cur_rx = (wrapSize + RTL_CRC_SIZE + 4 + 3) & ~3;
	}else{
		memcpy (buf, readPtr + 4, len);
		cur_rx = (cur_rx + len + RTL_CRC_SIZE + 4 + 3) & ~3;
	}
		
	/* We then write the CAPR as the next ptr we will read minus 0x10 which 	*/
	/* give us a little leeway to ensure that there is no overflow				*/
	csr_write_word (d, RTL_REGS_RX_BUF_PTR, (cur_rx - 16), RTL_WIN_0);

	d->counters[0]++;
	
	return len;	
}

STATUS rtl81x9_init(struct rtl81x9_device * d)
{
	UINT	sz = 0; 			/* temporary size holder */
	int rtlRsize = 5;
	int i, speed;
	USHORT rtl_dev_id;

	if(d->pShMem){
		/* already initialized */
		return OK;
	}
	
	/***** Establish size of shared memory region we require *****/
	/* Add it all up */
	sz = (((1 << rtlRsize) + 1) * RMD_SIZ) + IB_SIZ + 24 + 8;
	
	/*RTL81X9_LOG (RTL81X9_DEBUG_LOAD, "sx - %d\n",sz, 0, 0, 0, 0, 0); */
	
	
	/***** Establish a region of shared memory *****/
	
	/* OK. We now know how much shared memory we need.	If the caller
	* provides a specific memory region, we check to see if the provided
	* region is large enough for our needs.  If the caller did not
	* provide a specific region, then we attempt to allocate the memory
	* from the system, using the cache aware allocation system call.
	*/
	
	/* Because the structures that are shared between the device
	* and the driver may share cache lines, the possibility exists
	* that the driver could flush a cache line for a structure and
	* wipe out an asynchronous change by the device to a neighboring
	* structure. Therefore, this driver cannot operate with memory
	* that is not write coherent.  We check for the availability of
	* such memory here, and abort if the system did not give us what
	* we need.
	*/
	
	if (!CACHE_DMA_IS_WRITE_COHERENT ()){
		return ERROR;
	}
	
	d->pShMem = (char *) cacheDmaMalloc (sz);
	
	if ((int)d->pShMem == 0){
		return ERROR;
	}
		
	/*						  Turkey Carving
	*						 --------------
	*
	*						   LOW MEMORY
	*
	*			  |----------------------------------------|
	*			  | 	  The initialization block		   |
	*			  | 		(sizeof (rtl_ib))			   |
	*			  |----------------------------------------|
	*			  | 		The Rx descriptors			   |
	*			  | ((1 << rtlRsize) + 1)*sizeof (RTL_RMD) |
	*			  |----------------------------------------|
	*/
	
	/* align */
	d->memBase = (char *) ( ( (int)d->pShMem + 3) & ~3);
	/* Save some things */
	d->memBase  = (char *)((ULONG)d->pShMem & 0xff000000);	
	/* first let's clear memory */	
	bzero ((char *) d->pShMem, (int) sz); 
	/* setup Rx memory pointers */
	d->pRring	  = (RTL_RMD *) ((int)d->pShMem + IB_SIZ);
	d->rringLen	  = rtlRsize;
	d->rringSize   = 1 << rtlRsize;
	/* Allocate a chunk of memory for the Chip to place the Rx Buffers in */
	d->ptrRxBufSpace = malloc (RTL_RXBUFLEN + 64);

	d->rmdNext = 0;
	for(i=0; i<6; i++)
		d->macAddress[i] = csr_read_byte(d, i, NONE);

	/* align send buffer at 16-byte boundary */
	d->txmemspace = (char *)(((int)d->_txmemspace + 0xf) & ~0xf);
	d->phyAddr = 0;

	rtl81x9_reset(d);
	media_config (d);
	
	/*
	* Now read the exact device type from the EEPROM to find
	* out if it's an 8129 or 8139.
	*/
	read_eeprom (d, (UCHAR *) &rtl_dev_id, RTL_EE_PCI_DID, 1, 0);
	if (rtl_dev_id == RTL_DEVICEID_8139 || rtl_dev_id == ACCTON_DEVICEID_5030 ||
		rtl_dev_id == DELTA_DEVICEID_8139 || rtl_dev_id == ADDTRON_DEVICEID_8139)
	{
		if ((csr_read_long(d, RTL_REGS_TX_CONFIG, 0) & RTL_TXCG_BLID))/*vicadd*/
			d->devType = RTL_DEV_8139A;
		else
			d->devType = RTL_DEV_8139B;
	}else if (rtl_dev_id == RTL_DEVICEID_8129){
		d->devType = RTL_DEV_8129;
	}else{
		/* generic 8139 assumed */
		d->devType = RTL_DEV_8139A;
	}
}

void rtl81x9_uninit(struct rtl81x9_device * d)
{
	if(d->ptrRxBufSpace){
		free(d->ptrRxBufSpace);
		d->ptrRxBufSpace = 0;
	}
	if(d->pShMem){
		cacheDmaFree(d->pShMem);
		d->pShMem = 0;
	}
}

LOCAL void rtl81x9_reap_packets(struct rtl81x9_device  *d)
{
	int len;
	d->counters[4]++;
	while((len = rtl81x9_read(d, d->last_packet, sizeof(d->last_packet), 0)) > 0){
		d->last_packet_length = len;
		if(d->on_packet){
			d->counters[3]++;
			d->on_packet(d->last_packet, len);
		}
	}
}

LOCAL void rtl81x9_int(struct rtl81x9_device  *d)
{
	u_short 	   stat;
	int i;
	
	/* Disable controller interrupts. */
	csr_write_word (d, RTL_REGS_INTR_MASK, 0x00, NONE);
	
	for (;;){
		/* Read the interrupt status register */
		stat = csr_read_word (d, RTL_REGS_INTR_STATUS, NONE);
		
		/* clear interrupts, */
		/* 
		jht, no use as the datasheet states that write to ISR has no effect 
		just read ISR will clear it
		jht 2005-9-6 
		yes it's useful, the datasheet is wrong, writing with 1 will clear the bits
		respectfully, see the p-guide for detail.
		*/
		if (stat){
			/* In case of an RxFIFOOver we must also clear the RxOverflow
		   	bit to avoid dropping frames for ever.
			*/
#if 0
			logMsg("rtl81x9_int : statue is %04x\n", stat);
#endif
			csr_write_word (d, RTL_REGS_INTR_STATUS, 
				(stat & RTL_IPT_RX_FIFO_OVER)? stat | RTL_IPT_RX_OVERFLOW : stat, NONE);
			d->counters[2]++;
		}else{
		}
		
		/* Check if a valid Interrupt has been set */
		if ((stat & RTL_VALID_INTERRUPTS) == 0){
			/* spurious interrupt */
			break;
		}
		
		if (stat & RTL_IPT_PCI_ERR)	{
			rtl81x9_start(d);
		}
		
		if (stat & RTL_IPT_PCS_TIMEOUT){
		}
		
		if (stat & RTL_IPT_CABLE_LEN_CHG){
		}
		
		if (stat & RTL_IPT_RX_FIFO_OVER){
		}
		
		if (stat & RTL_IPT_RX_UNDERUN){
		}
		
		if (stat & RTL_IPT_RX_OVERFLOW){
		}
		
		if (stat & RTL_IPT_TX_ERR){
		}
		
		/* Check for transmit Interrupt */
		if (stat & RTL_IPT_TX_OK){
		}
		
		if (stat & RTL_IPT_RX_ERR){
		}
		
		if (stat & (RTL_IPT_RX_OK | RTL_IPT_RX_OVERFLOW | RTL_IPT_RX_FIFO_OVER | RTL_IPT_RX_UNDERUN)){
			/* netJobAdd((FUNCPTR)rtl81x9_reap_packets, d,2,3,4,5); */
			rtl81x9_reap_packets(d);
		}		
	}
	
	csr_write_word (d, RTL_REGS_INTR_MASK, RTL_VALID_INTERRUPTS, NONE);
}

STATUS rtl81x9_start(struct rtl81x9_device * d)
{
	int rxcfg;
	int i;
	/* IMPORT STATUS pciIntConnect(); */

	rtl81x9_reset(d);
	
	/* Init the RX buffer pointer register. */
	csr_write_long(d, RTL_REGS_RX_BUF, (ULONG) d->ptrRxBufSpace, RTL_WIN_0);

	/* Must enable Tx/Rx before setting transfer thresholds! */
	csr_write_byte(d, RTL_REGS_CHIP_CMD, RTL_CMD_RX_ENB + RTL_CMD_TX_ENB, RTL_WIN_0);
	/*
	* Set the initial TX and RX configuration.
	* Set the buffer size and set the wrap register
	*/
	csr_write_long (d, RTL_REGS_TX_CONFIG, RTL_TXCFG_CONFIG, RTL_WIN_0); 
	csr_write_long (d, RTL_REGS_RX_CONFIG, RTL_RXCFG_CONFIG, RTL_WIN_0); 
	
	rxcfg = csr_read_long(d, RTL_REGS_RX_CONFIG, RTL_WIN_0);
	
	/* Set the Early Threshold bits depending on flags read */	/*vicadd*/
	/* from initialisation string							*/
	rxcfg |= (10<< 24); /*RTL81X9_EARX_THRESH */
	
	/* Set the individual bit to receive frames for this host only. */
	rxcfg |= RTL_RXCG_APM;
	
	/*
	* Set capture broadcast bit to capture broadcast frames.
	*/
	rxcfg |= RTL_RXCG_AB;	
	
	/*	RTL81X9_LOG (RTL81X9_DEBUG_ALL, "rtl81x9-RCR - %x: \n", rxcfg, 2, 3, 4, 5, 6);*/
	
	d->reg_rcr =rxcfg;/* save it */
	csr_write_long (d, RTL_REGS_RX_CONFIG, rxcfg, RTL_WIN_0); 
	
	/* We now need to update the Multicast Registers */
	/* These values need to be finalised and written */
	csr_write_long (d, RTL_REGS_MAR0, 0xffff, RTL_WIN_0); 
	csr_write_long (d, RTL_REGS_MAR0 + 4, 0xffff, RTL_WIN_0); 

	/* enable receive interrupts */
	if(d->flags & RTL_FLG_POLLING){
		csr_write_word (d, RTL_REGS_INTR_MASK, 0, 0);
	}else{
		sysIntEnablePIC(d->ilevel);
		pciIntConnect ((VOIDFUNCPTR *)INUM_TO_IVEC (d->ivec), rtl81x9_int, (int)d);
		csr_write_word (d, RTL_REGS_INTR_MASK, RTL_VALID_INTERRUPTS, 0);
	}
	
	/* Start RX/TX process. */
	csr_write_long (d, RTL_REGS_RX_MISSED, 0, RTL_WIN_0); 
	
	/* Enable Tx and RX */
	csr_write_byte(d, RTL_REGS_CHIP_CMD, RTL_CMD_RX_ENB + RTL_CMD_TX_ENB, RTL_WIN_0);

	d->nextDesc = 0;
	for(i=0; i<RTL_NUM_TX_DESC; i++){
		csr_write_long (d, RTL_REGS_TX_ADDR0 + (i* 4), 0, RTL_WIN_0); 
	}

	memset(d->counters, 0, sizeof(d->counters));
	
	return OK;
}

void rtl81x9_stop(struct rtl81x9_device * d)
{
	/* disable interrupts */
	csr_write_word (d, RTL_REGS_INTR_MASK, 0, 0);
	if(!(d->flags & RTL_FLG_POLLING)){
		pciIntDisconnect ((VOIDFUNCPTR *)INUM_TO_IVEC (d->ivec), rtl81x9_int);
	}
}

/*******************************************************************************
*
* rtl81x9Wait - wait for the command completion
*
* This function waits for the command completion
* It is possible the chip can wedge and the 'command in progress' bit may
* never clear. Hence, we wait only a finite amount of time to avoid getting
* caught in an infinite loop. Normally this delay routine would be a macro,
* but it isn't called during normal operation so we can afford to make it a
* function.
*
* RETURNS: N/A
*/

void rtl81x9_wait(struct rtl81x9_device * d)
{
	register int	ix;
	
	for (ix = 0; ix < RTL_TIMEOUT; ix++){
		/*if ((csr_read_word (d, RTL_REGS_CHIP_CMD, NONE) & 0x10) == 0)*/
		if ((csr_read_byte(d, RTL_REGS_CHIP_CMD, NONE) & 0x10) == 0)
			break;			/*vicadd*/
	}
		
	return;
}

STATUS rtl81x9_reset(struct rtl81x9_device * d)
{
	csr_write_byte(d, RTL_REGS_CHIP_CMD, RTL_CMD_RESET, RTL_WIN_0);
	rtl81x9_wait(d);
	/* wait for a while */
	sysStallExecutionProcessor(200);	/* wait for the command to complete */
	return OK;
}

void rtl81x9_enable_intr(struct rtl81x9_device * d, int mode)
{
	if(!(d->flags & RTL_FLG_POLLING)){
		if(mode){
			csr_write_word (d, RTL_REGS_INTR_MASK, RTL_VALID_INTERRUPTS, 0);
		}
	}
}

int rtl81x9_disable_intr(struct rtl81x9_device * d)
{
	int mode = csr_read_word(d, RTL_REGS_INTR_MASK, 0);
	if(!(d->flags & RTL_FLG_POLLING)){
		csr_write_word (d, RTL_REGS_INTR_MASK, 0, 0);
	}
	return mode & RTL_VALID_INTERRUPTS;
}

LOCAL void dumpBuf(void * buf, int len)
{
	int i;
	unsigned char *p=(unsigned char*)buf;
	if (len > 64){
		len = 64;
	}
	i=0;
	while(i < len){
		printf("%02x ", *p++);
		if((i % 8)==7){
			printf("-- ");
		}
		if((i % 16)==15){
			unsigned char * k;
			k=(unsigned char*)buf;
			while(k < p){
				if(isgraph(*k)){
					printf("%c", *k);
				}else{
					printf(".");
				}
				k++;
			}
			buf = (char*)buf+16;
			putchar('\n');
		}
		i++;
	}
	putchar('\n');
}

void rtl81x9_show(struct rtl81x9_device * d)
{
	int i;
	printf("nextDesc=%d,RxBufSpace=%08x,memBase=%08x,ShMem=%08x\n",
		d->nextDesc, d->ptrRxBufSpace, d->memBase, d->pShMem);
	printf("txmemspace=%08x,type=%d,ivec=%d,ilevel=%d,on_packet=%08x\n", d->txmemspace, d->devType, 
		d->ivec, d->ilevel, d->on_packet);
	printf("counters:\n");
	for(i=0; i<sizeof(d->counters)/sizeof(d->counters[0]); i++){
		printf("%d=%d,", i, d->counters[i]);
	}
	printf("\nLast packet at %08x, %d bytes:\n", d->last_packet, d->last_packet_length);
	dumpBuf(d->last_packet, d->last_packet_length);
}

/*
* MII access routines are provided for the 8129, which
* doesn't have a built-in PHY. For the 8139, we fake things
* up by diverting rl_phy_readreg()/rl_phy_writereg() to the
* direct access PHY registers.
*/
#define _mii_set(x)					\
	csr_write_byte (d, RTL_REGS_MII,				\
	csr_read_byte(d, RTL_REGS_MII, RTL_WIN_0) | x, RTL_WIN_0 )

#define _mii_clr(x)					\
	csr_write_byte (d, RTL_REGS_MII,				\
	csr_read_byte(d, RTL_REGS_MII, RTL_WIN_0) & ~x, RTL_WIN_0 )

/* EEPROM Macros */

#define _ee_set(d, x)					\
	csr_write_byte (d, RTL_REGS_CFG_9346,			\
	csr_read_byte(d, RTL_REGS_CFG_9346, RTL_WIN_0) | x, RTL_WIN_0)

#define _ee_clr(d, x)					\
	csr_write_byte(d, RTL_REGS_CFG_9346,			\
	csr_read_byte(d, RTL_REGS_CFG_9346, RTL_WIN_0) & ~x, RTL_WIN_0)

/*******************************************************************************
*
* read_eeprom - read a value from the EEPROM
*
* This function reads a sequence of words from the EEPROM at the address passed in the params
*
* RETURNS: N/A
*/

LOCAL void read_eeprom
	(
	struct rtl81x9_device * 	d,	/* device control structure */
	UCHAR					*dest,
	ULONG					off,
	ULONG					cnt,
	ULONG					swap
	)
	
{
	int 		i;
	USHORT	word = 0, *ptr;

	for (i = 0; i < cnt; i++)	{
		eeprom_get_word(d, off + i, &word);
		ptr = (USHORT *)(dest + (i * 2));
		if (swap)
			*ptr = ntohs(word);
		else
			*ptr = word;
	}
	
	return;
}

/*******************************************************************************
*
* eeprom_get_word - read a value from the EEPROM
*
* Read a word of data stored in the EEPROM at address 'addr.'
*
* RETURNS: N/A
*/

LOCAL void eeprom_get_word
	(
	struct rtl81x9_device * 	d,	/* device control structure */
	ULONG					addr,
	USHORT					*dest
	)
	
{
	register int		i;
	USHORT				word = 0;

	/* Enter EEPROM access mode. */
	csr_write_byte (d, RTL_REGS_CFG_9346, RTL_EEMODE_PROGRAM|RTL_EE_SEL, RTL_WIN_0);
	
	/*
	* Send address of word we want to read.
	*/
	eeprom_put_byte (d, addr);
	
	csr_write_byte (d, RTL_REGS_CFG_9346, RTL_EEMODE_PROGRAM|RTL_EE_SEL, RTL_WIN_0);
	
	/*
	* Start reading bits from EEPROM.
	*/
	for (i = 0x8000; i; i >>= 1) {
		_ee_set(d, RTL_EE_CLK);
		sysStallExecutionProcessor(10);
		if (csr_read_byte (d, RTL_REGS_CFG_9346, RTL_WIN_0) & RTL_EE_DATAOUT)
			word |= i;
		_ee_clr(d, RTL_EE_CLK);
		sysStallExecutionProcessor(10);
	}
	
	/* Turn off EEPROM access mode. */
	csr_write_byte(d, RTL_REGS_CFG_9346, RTL_EEMODE_OFF, RTL_WIN_0);
	
	*dest = word;
	
	return;
}

/*******************************************************************************
*
* eeprom_put_byte - read a value from the EEPROM
*
* Send a read command and address to the EEPROM, check for ACK
*
* RETURNS: N/A
*/

LOCAL void eeprom_put_byte
	(
	struct rtl81x9_device * 	_d,	/* device control structure */
	ULONG					addr
	)
	
{
	register int		d, i;

	d = addr | RTL_EECMD_READ;
	
	/*
	* Feed in each bit and stobe the clock.
	*/
	for (i = 0x400; i; i >>= 1) {
		if (d & i) {
			_ee_set(_d, RTL_EE_DATAIN);
		} else {
			_ee_clr(_d, RTL_EE_DATAIN);
		}
		sysStallExecutionProcessor(10);
		_ee_set(_d, RTL_EE_CLK);
		sysStallExecutionProcessor(10);
		_ee_clr(_d, RTL_EE_CLK);
		sysStallExecutionProcessor(10);
	}
	
	return;
}

/*******************************************************************************
*
* mii_sync - Sync the PHY's
*
* Sync the PHYs by setting data bit and strobing the clock 32 times.
*
* RETURNS: N/A
*/

LOCAL void mii_sync
	(
	struct rtl81x9_device * 	d	/* device control structure */
	)
	
{
	register int		i;

	_mii_set(RTL_MII_DIR|RTL_MII_DATAOUT);
	
	for (i = 0; i < 32; i++) {
		_mii_set(RTL_MII_CLK);
		sysStallExecutionProcessor(10);
		_mii_clr(RTL_MII_CLK);
		sysStallExecutionProcessor(10);
	}
	
	return;
}


/*******************************************************************************
*
* mii_send
*
* Clock a series of bits through the MII.
*
* RETURNS: N/A
*/

LOCAL void mii_send
	(
	struct rtl81x9_device * 	d,	/* device control structure */
	ULONG					bits,
	int 					cnt
	)
	
{
	int 		i;

	_mii_clr(RTL_MII_CLK);
	
	for (i = (0x1 << (cnt - 1)); i; i >>= 1) {
		if (bits & i) {
			_mii_set(RTL_MII_DATAOUT);
		} else {
			_mii_clr(RTL_MII_DATAOUT);
		}
		sysStallExecutionProcessor(10);
		_mii_clr(RTL_MII_CLK);
		sysStallExecutionProcessor(10);
		_mii_set(RTL_MII_CLK);
	}

	return;
}

/*******************************************************************************
*
* mii_read_reg
*
* Read an PHY register through the MII.
*
* RETURNS: N/A
*/

LOCAL int mii_read_reg 
	(
	struct rtl81x9_device * 	d,	/* device control structure */
	RTL_MII_FRAME * 		frame
	)
	
{
	int 		i, ack, s;

	s = splimp();
	
	/*
	* Set up frame for RX.
	*/
	frame->mii_stdelim = RTL_MII_STARTDELIM;
	frame->mii_opcode = RTL_MII_READOP;
	frame->mii_turnaround = 0;
	frame->mii_data = 0;
	
	csr_write_word (d, RTL_REGS_MII, 0, RTL_WIN_0);
	
	/*
	* Turn on data xmit.
	*/
	_mii_set(RTL_MII_DIR);
	
	mii_sync(d);
	
	/*
	* Send command/address info.
	*/
	mii_send(d, frame->mii_stdelim, 2);
	mii_send(d, frame->mii_opcode,  2);
	mii_send(d, frame->mii_phyaddr, 5);
	mii_send(d, frame->mii_regaddr, 5);
	
	/* Idle bit */
	_mii_clr((RTL_MII_CLK|RTL_MII_DATAOUT));
	sysStallExecutionProcessor(10);
	_mii_set(RTL_MII_CLK);
	sysStallExecutionProcessor(10);
	
	/* Turn off xmit. */
	_mii_clr(RTL_MII_DIR);
	
	/* Check for ack */
	_mii_clr(RTL_MII_CLK);
	sysStallExecutionProcessor(10);
	_mii_set(RTL_MII_CLK);
	sysStallExecutionProcessor(10);
	ack = csr_read_word(d, RTL_REGS_MII, RTL_WIN_0) & RTL_MII_DATAIN;
	
	/*
	* Now try reading data bits. If the ack failed, we still
	* need to clock through 16 cycles to keep the PHY(s) in sync.
	*/
	if (ack) {
		for(i = 0; i < 16; i++) {
			_mii_clr(RTL_MII_CLK);
			sysStallExecutionProcessor(1);
			_mii_set(RTL_MII_CLK);
			sysStallExecutionProcessor(1);
		}
		goto fail;
	}
	
	for (i = 0x8000; i; i >>= 1) {
		_mii_clr(RTL_MII_CLK);
		sysStallExecutionProcessor(1);
		if (!ack) {
			if (csr_read_word(d, RTL_REGS_MII, RTL_WIN_0) & RTL_MII_DATAIN)
				frame->mii_data |= i;
			sysStallExecutionProcessor(1);
		}
		_mii_set(RTL_MII_CLK);
		sysStallExecutionProcessor(1);
	}
	
fail:
	
	_mii_clr(RTL_MII_CLK);
	sysStallExecutionProcessor(1);
	_mii_set(RTL_MII_CLK);
	sysStallExecutionProcessor(1);
	
	splx(s);
	
	if (ack){
		return 1;
	}
	return 0;
}



/*******************************************************************************
*
* mii_write_reg
*
* Write an PHY register through the MII.
*
* RETURNS: N/A
*/
	
LOCAL int mii_write_reg 
	(
	struct rtl81x9_device * 	d,	/* device control structure */
	RTL_MII_FRAME * 		frame
	)
{
	int 		s;

	s = splimp();
	
	/*
	* Set up frame for TX.
	*/
	
	frame->mii_stdelim = RTL_MII_STARTDELIM;
	frame->mii_opcode = RTL_MII_WRITEOP;
	frame->mii_turnaround = RTL_MII_TURNAROUND;
	
	/*
	* Turn on data output.
	*/
	
	_mii_set(RTL_MII_DIR);
	
	mii_sync(d);
	
	mii_send(d, frame->mii_stdelim, 2);
	mii_send(d, frame->mii_opcode, 2);
	mii_send(d, frame->mii_phyaddr, 5);
	mii_send(d, frame->mii_regaddr, 5);
	mii_send(d, frame->mii_turnaround, 2);
	mii_send(d, frame->mii_data, 16);
	
	/* Idle bit. */
	_mii_set(RTL_MII_CLK);
	sysStallExecutionProcessor(1);
	_mii_clr(RTL_MII_CLK);
	sysStallExecutionProcessor(1);
	
	/*
	* Turn off xmit.
	*/
	_mii_clr(RTL_MII_DIR);
	
	splx(s);
	
	return 0;
}
		
/*******************************************************************************
*
* phy_read_reg
*
* Read an PHY register.
*
* RETURNS: N/A
*/

LOCAL USHORT phy_read_reg 
	(
	struct rtl81x9_device * 	d,	/* device control structure */
	int 					reg
	)
	
{
	RTL_MII_FRAME			frame;
	USHORT					rval = 0;
	USHORT					rtl8139_reg = 0;

	if ((d->devType == RTL_DEV_8139A) || (d->devType == RTL_DEV_8139B)){
		switch(reg) {
		case RTL_PHY_BMCR:
			rtl8139_reg = RTL_REGS_MII_BMCR;
			break;
		case RTL_PHY_BMSR:
			rtl8139_reg = RTL_REGS_MII_BMSR;
			break;
		case RTL_PHY_ANAR:
			rtl8139_reg = RTL_REGS_NWAY_ADVERT;
			break;
		case RTL_PHY_LPAR:
			rtl8139_reg = RTL_REGS_NWAY_LPAR;
			break;
		default:
			return 0;
		}
		rval = csr_read_word(d, rtl8139_reg, RTL_WIN_0);
		
		return rval;
	}
	
	bzero((char *)&frame, sizeof(frame));
	
	frame.mii_phyaddr = d->phyAddr;
	frame.mii_regaddr = reg;
	mii_read_reg(d, &frame);
	
	return frame.mii_data;
}
		
		
/*******************************************************************************
*
* phy_write_reg
*
* Read an PHY register.
*
* RETURNS: N/A
*/

LOCAL void phy_write_reg 
	(
	struct rtl81x9_device * 	d,	/* device control structure */
	int 					reg,
	int 					data
	)
{
	RTL_MII_FRAME			frame;
	USHORT					rtl8139_reg = 0;
	
	if ((d->devType == RTL_DEV_8139A) || (d->devType == RTL_DEV_8139B)){
		switch(reg) {
		case RTL_PHY_BMCR:
			rtl8139_reg = RTL_REGS_MII_BMCR;
			break;
		case RTL_PHY_BMSR:
			rtl8139_reg = RTL_REGS_MII_BMSR;
			break;
		case RTL_PHY_ANAR:
			rtl8139_reg = RTL_REGS_NWAY_ADVERT;
			break;
		case RTL_PHY_LPAR:
			rtl8139_reg = RTL_REGS_NWAY_LPAR;
			break;
		default:
			return;
		}
		csr_write_word (d, rtl8139_reg, data, RTL_WIN_0);
		return;
	}
	
	bzero((char *)&frame, sizeof(frame));
	
	frame.mii_phyaddr = d->phyAddr;
	frame.mii_regaddr = reg;
	frame.mii_data = data;
	
	mii_write_reg(d, &frame);
	
	return;
}


/*******************************************************************************
*
* auto_neg_tx
*
* Initiate an autonegotiation session.
*
* RETURNS: N/A
*/

LOCAL void auto_neg_tx 
	(
	struct rtl81x9_device * 	d	/* device control structure */
	)
{
	
	USHORT		phy_sts;
	
	phy_write_reg (d, RTL_PHY_BMCR, RTL_PHY_BMCR_RESET);
	
	sysStallExecutionProcessor(500);
	while(phy_read_reg(d, RTL_PHY_BMCR) & RTL_PHY_BMCR_RESET);
	
	phy_sts = phy_read_reg(d, RTL_PHY_BMCR);
	phy_sts |= RTL_PHY_BMCR_AUTONEGENBL|RTL_PHY_BMCR_AUTONEGRSTR;
	phy_write_reg(d, RTL_PHY_BMCR, phy_sts);
	
	sysStallExecutionProcessor(500);
	
	return;
}


/*******************************************************************************
*
* mii_auto_neg
*
* Invoke autonegotiation on a PHY. Also used with the 8139 internal
* transceiver.
*
* RETURNS: N/A
*/

LOCAL int mii_auto_neg 
	(
	struct rtl81x9_device * 	d	/* device control structure */
	)
	
{
	UINT16		phyStatus = 0;
	UINT16		media;
	UINT16		advert;
	UINT16		ability;
	int 		speed = 10000000;
	
	/*
	* The 100baseT4 PHY sometimes has the 'autoneg supported'
	* bit cleared in the status register, but has the 'autoneg enabled'
	* bit set in the control register. This is a contradiction, and
	* I'm not sure how to handle it. If you want to force an attempt
	* to autoneg for 100baseT4 PHYs, #define FORCE_AUTONEG_TFOUR
	* and see what happens.
	*/
#ifndef FORCE_AUTONEG_TFOUR
	/*
	* First, see if autoneg is supported. If not, there's
	* no point in continuing.
	*/
	phyStatus = phy_read_reg(d, RTL_PHY_BMSR);
	if (!(phyStatus & RTL_PHY_BMSR_CANAUTONEG)) {
		media = phy_read_reg (d, RTL_PHY_BMCR);
		media &= ~RTL_PHY_BMCR_SPEEDSEL;
		media &= ~RTL_PHY_BMCR_DUPLEX;
		
		phy_write_reg (d, RTL_PHY_BMCR, media);
		
		return speed;
	}
#endif
	
	
	auto_neg_tx (d);
	
	sysStallExecutionProcessor (5000);
	
	if (phy_read_reg (d, RTL_PHY_BMSR) & RTL_PHY_BMSR_AUTONEGCOMP){
		phyStatus = phy_read_reg(d, RTL_PHY_BMSR);
	}
	
	media = phy_read_reg(d, RTL_PHY_BMCR);
	
	/* Link is good. Report modes and set duplex mode. */
	if (phy_read_reg(d, RTL_PHY_BMSR) & RTL_PHY_BMSR_LINKSTAT){
		advert = phy_read_reg(d, RTL_PHY_ANAR);
		ability = phy_read_reg(d, RTL_PHY_LPAR);
		
		if (advert & RTL_PHY_ANAR_100BT4 && ability & RTL_PHY_ANAR_100BT4){
			media |= RTL_PHY_BMCR_SPEEDSEL;
			media &= ~RTL_PHY_BMCR_DUPLEX;
			speed = 100000000;
		} else if (advert & RTL_PHY_ANAR_100BTXFULL &&	ability & RTL_PHY_ANAR_100BTXFULL){
			media |= RTL_PHY_BMCR_SPEEDSEL;
			media |= RTL_PHY_BMCR_DUPLEX;
			speed = 100000000;
		}else if (advert & RTL_PHY_ANAR_100BTXHALF && ability & RTL_PHY_ANAR_100BTXHALF){
			media |= RTL_PHY_BMCR_SPEEDSEL;
			media &= ~RTL_PHY_BMCR_DUPLEX;
			speed = 100000000;
		} else if (advert & RTL_PHY_ANAR_10BTFULL && ability & RTL_PHY_ANAR_10BTFULL) {
			media &= ~RTL_PHY_BMCR_SPEEDSEL;
			media |= RTL_PHY_BMCR_DUPLEX;
			speed = 10000000;
		} else {
			media &= ~RTL_PHY_BMCR_SPEEDSEL;
			media &= ~RTL_PHY_BMCR_DUPLEX;
			speed = 10000000;
		}
		
		/* Set ASIC's duplex mode to match the PHY. */
		phy_write_reg(d, RTL_PHY_BMCR, media);
	} 	else {
		media &= ~RTL_PHY_BMCR_SPEEDSEL;
		media &= ~RTL_PHY_BMCR_DUPLEX;
		phy_write_reg (d, RTL_PHY_BMCR, media);
	}
	
	return speed;
}

/*******************************************************************************
*
* media_set
*
* Set speed and duplex mode.
*
* RETURNS: N/A
*/

LOCAL int media_set 
(
	struct rtl81x9_device *	d,	/* device control structure */
	int 					media
	)
	
{
	USHORT		bmcr;
	USHORT		bmsr;
	int 			speed = 10000000;
	
	bmsr = phy_read_reg(d, RTL_PHY_BMSR);
	bmcr = phy_read_reg(d, RTL_PHY_BMCR);
	
	bmcr &= ~(RTL_PHY_BMCR_AUTONEGENBL|RTL_PHY_BMCR_SPEEDSEL|
		RTL_PHY_BMCR_DUPLEX|RTL_PHY_BMCR_LOOPBK);
	
	if (bmsr & RTL_PHY_BMSR_CANAUTONEG)	{
		return 0;
	}
	
	
	if (bmsr & RTL_PHY_BMSR_10BTHALF)	{
		if (IFM_SUBTYPE(media) == IFM_10_T) {
			bmcr &= ~RTL_PHY_BMCR_SPEEDSEL;
		}
	}
	
	if (bmsr & RTL_PHY_BMSR_10BTFULL){
		if (IFM_SUBTYPE(media) == IFM_10_T) {			
			bmcr &= ~RTL_PHY_BMCR_SPEEDSEL;
		}
	}
	
	if (bmsr & RTL_PHY_BMSR_100BTXHALF){
		speed = 100000000;
		if (IFM_SUBTYPE(media) == IFM_100_TX) {			
			bmcr |= RTL_PHY_BMCR_SPEEDSEL;
		}
	}
	
	if (bmsr & RTL_PHY_BMSR_100BTXFULL){
		speed = 100000000;
		if (IFM_SUBTYPE(media) == IFM_100_TX){			
			bmcr |= RTL_PHY_BMCR_SPEEDSEL;
		}
	}
	
	/* Some also support 100BaseT4 */
	if (bmsr & RTL_PHY_BMSR_100BT4){
		speed = 100000000;
		if (IFM_SUBTYPE(media) == IFM_100_T4){			
			bmcr |= RTL_PHY_BMCR_SPEEDSEL;
			bmcr &= ~RTL_PHY_BMCR_DUPLEX;
		}
	}
	if ((media & IFM_GMASK) == IFM_FDX){
		bmcr |= RTL_PHY_BMCR_DUPLEX;
	}else{
		bmcr &= ~RTL_PHY_BMCR_DUPLEX;
	}
	
	phy_write_reg(d, RTL_PHY_BMCR, bmcr);
	
	return speed;
	
}

/*******************************************************************************
*
* media_config - configure the media and do auto negotiation
*
* This function configures the media and does the auto negotioation.
*
* RETURNS: speed or 0
*
*/

LOCAL int media_config(struct rtl81x9_device *	d	)
{
	int speed;

	if ((speed = media_set (d, IFM_10_T)) == 0){
		speed = mii_auto_neg (d);
	}
	
	return speed;
}


enum TwisterParamVals {
	PARA78_default	= 0x78fa8388,
	PARA7c_default	= 0xcb38de43,	/* param[0][3] */
	PARA7c_xxx	= 0xcb38de43,
};

static const unsigned long param[4][4] = {
	{0xcb39de43, 0xcb39ce43, 0xfb38de03, 0xcb38de43},
	{0xcb39de43, 0xcb39ce43, 0xcb39ce83, 0xcb39ce83},
	{0xcb39de43, 0xcb39ce43, 0xcb39ce83, 0xcb39ce83},
	{0xbb39de43, 0xbb39ce43, 0xbb39ce83, 0xbb39ce83}
};

enum RTL8139_registers{
	/* Undocumented registers, but required for proper operation. */
	FIFOTMS = 0x70,		/* FIFO Control and test. */
	CSCR = 0x74,		/* Chip Status and Configuration Register. */
	PARA78 = 0x78,
	PARA7c = 0x7c,		/* Magic transceiver parameter register. */
};
	
/* Twister tuning parameters from RealTek.
   Completely undocumented, but required to tune bad links on some boards. */
enum CSCRBits {
	CSCR_LinkOKBit = 0x0400,
	CSCR_LinkChangeBit = 0x0800,
	CSCR_LinkStatusBits = 0x0f000,
	CSCR_LinkDownOffCmd = 0x003c0,
	CSCR_LinkDownCmd = 0x0f3c0,
};

LOCAL int next_tick;

LOCAL int rtl81x9_tune_twister_1(struct rtl81x9_device * tp)
{
	int linkcase;
	int HZ=1000;

	/* This is a complicated state machine to configure the "twister" for
	   impedance/echos based on the cable length.
	   All of this is magic and undocumented.
	 */
	switch (tp->twistie) {
	case 1:
		if (csr_read_word(tp, CSCR, 0) & CSCR_LinkOKBit) {
			/* We have link beat, let us tune the twister. */
			csr_write_word(tp, CSCR, CSCR_LinkDownOffCmd, 0);
			tp->twistie = 2;	/* Change to state 2. */
			next_tick = HZ / 10;
		} else {
			/* Just put in some reasonable defaults for when beat returns. */
			csr_write_word(tp, CSCR, CSCR_LinkDownCmd, 0);
			csr_write_long(tp, FIFOTMS, 0x20, 0);	/* Turn on cable test mode. */
			csr_write_long(tp, PARA78, PARA78_default, 0);
			csr_write_long(tp, PARA7c, PARA7c_default, 0);
			tp->twistie = 0;	/* Bail from future actions. */
		}
		break;
	case 2:
		/* Read how long it took to hear the echo. */
		linkcase = csr_read_word(tp, CSCR, 0) & CSCR_LinkStatusBits;
		if (linkcase == 0x7000)
			tp->twist_row = 3;
		else if (linkcase == 0x3000)
			tp->twist_row = 2;
		else if (linkcase == 0x1000)
			tp->twist_row = 1;
		else
			tp->twist_row = 0;
		tp->twist_col = 0;
		tp->twistie = 3;	/* Change to state 2. */
		next_tick = HZ / 10;
		break;
	case 3:
		/* Put out four tuning parameters, one per 100msec. */
		if (tp->twist_col == 0)
			csr_write_word(tp, FIFOTMS, 0, 0);
		csr_write_long(tp, PARA7c, param[(int) tp->twist_row][(int) tp->twist_col], 0);
		next_tick = HZ / 10;
		if (++tp->twist_col >= 4) {
			/* For short cables we are done.
			   For long cables (row == 3) check for mistune. */
			tp->twistie =
			    (tp->twist_row == 3) ? 4 : 0;
		}
		break;
	case 4:
		/* Special case for long cables: check for mistune. */
		if ((csr_read_word(tp,  CSCR, 0) & CSCR_LinkStatusBits) == 0x7000) {
			tp->twistie = 0;
			break;
		} else {
			csr_write_long(tp, PARA7c, 0xfb38de03, 0);
			tp->twistie = 5;
			next_tick = HZ / 10;
		}
		break;
	case 5:
		/* Retune for shorter cable (column 2). */
		csr_write_long(tp, FIFOTMS, 0x20, 0);
		csr_write_long(tp, PARA78, PARA78_default, 0);
		csr_write_long(tp, PARA7c, PARA7c_default, 0);
		csr_write_long(tp, FIFOTMS, 0x00, 0);
		tp->twist_row = 2;
		tp->twist_col = 0;
		tp->twistie = 3;
		next_tick = HZ / 10;
		break;

	default:
		/* do nothing */
		break;
	}

	return next_tick;
}

STATUS rtl81x9_tune_twister(struct rtl81x9_device * tp)
{
	int tick=0;
	int tw=0;
	tp->twistie = 1;
	while(tp->twistie){
		taskDelay(tick);
		if(1 || tp->twistie != tw){
			tw = tp->twistie;
			//printf("go to state %d.\n", tw);
		}
		tick = rtl81x9_tune_twister_1(tp);
	}
	return OK;
}

