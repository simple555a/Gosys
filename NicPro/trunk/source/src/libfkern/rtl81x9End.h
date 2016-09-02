/* rtl81x9End.h - RealTek RTL81x9 Fast Ethernet interface header */

/* Copyright 1984-2000 Wind River Systems, Inc. */
/*
modification history
--------------------
01b,26jan00,dgy  Set the Early Rx Threshold within
				 the RTL_RXCFG_CONFIG
01a,29oct99,dgy  written
*/

#ifndef __INCrtl81x9Endh
#define __INCrtl81x9Endh

#include "End.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DMA Definitions */

#define RTL_RXDMA_16BYTES		0x00000000
#define RTL_RXDMA_32BYTES		0x00000100
#define RTL_RXDMA_64BYTES		0x00000200
#define RTL_RXDMA_128BYTES		0x00000300
#define RTL_RXDMA_256BYTES		0x00000400
#define RTL_RXDMA_512BYTES		0x00000500
#define RTL_RXDMA_1024BYTES		0x00000600
#define RTL_RXDMA_UNLIMITED		0x00000700

#define RTL_RXFIFO_16BYTES		0x00000000
#define RTL_RXFIFO_32BYTES		0x00002000
#define RTL_RXFIFO_64BYTES		0x00004000
#define RTL_RXFIFO_128BYTES		0x00006000
#define RTL_RXFIFO_256BYTES		0x00008000
#define RTL_RXFIFO_512BYTES		0x0000A000
#define RTL_RXFIFO_1024BYTES	0x0000C000
#define RTL_RXFIFO_NOTHRESH		0x0000E000

#define RTL_TXDMA_16BYTES		0x00000000
#define RTL_TXDMA_32BYTES		0x00000100
#define RTL_TXDMA_64BYTES		0x00000200
#define RTL_TXDMA_128BYTES		0x00000300
#define RTL_TXDMA_256BYTES		0x00000400
#define RTL_TXDMA_512BYTES		0x00000500
#define RTL_TXDMA_1024BYTES		0x00000600
#define RTL_TXDMA_2048BYTES		0x00000700

/*
 * The RealTek doesn't use a fragment-based descriptor mechanism.
 * Instead, there are only four register sets, each or which represents
 * one 'descriptor.' Basically, each TX descriptor is just a contiguous
 * packet buffer (32-bit aligned!) and we place the buffer addresses in
 * the registers so the chip knows where they are.
 *
 * For reception, there's just one large buffer where the chip stores
 * all received packets.
 */

/* Configuration items */


#define RTL_BUFSIZ      (ETHERMTU + ENET_HDR_REAL_SIZ + 6)
#define RTL_RMD_RLEN   5       /* ring size as a power of 2 -- 32 RMD's */ /*vicadd*/
#define RTL_SPEED        10000000

/* Definitions for the drvCtrl flags field */
/* The format for the Flags field is as follows */

/* The first 16 bits are allocated as being used 	*/
/* for miscellaneous flags. The second 16 bits   	*/
/* are used for setting the Early Rx Threshold 		*/
/* from within the string passed in initialisation 	*/
/* This allows the user to taylor performance 		*/
/* depending on their board without the need 		*/
 /* recomplile the driver source code.				*/

#define RTL_FLG_PROMISCUOUS_FLAG     0x01
#define RTL_FLG_MEM_ALLOC_FLAG       0x02
#define RTL_FLG_MODE_MEM_IO_MAP	     0x04   /* device registers memory mapped */
#define RTL_FLG_POLLING              0x08	/* polling flag */

#define RTL_RXBUF_8			0x00000000
#define RTL_RXBUF_16			0x00000800
#define RTL_RXBUF_32			0x00001000
#define RTL_RXBUF_64			0x00001800

#define RTL_RX_BUF_SZ		RTL_RXBUF_64   
#define RTL_RXBUFLEN		(1 << ((RTL_RX_BUF_SZ >> 11) + 13))

#define RTL_RX_FIFOTHRESH	RTL_RXFIFO_NOTHRESH 
/*#define RTL_RX_FIFOTHRESH	RTL_RXFIFO_128BYTES*/
/*#define RTL_RX_MAXDMA		RTL_RXDMA_256BYTES*/
#define RTL_RX_MAXDMA		RTL_RXDMA_1024BYTES		/*/vicadd*/
#define RTL_TX_MAXDMA		RTL_TXDMA_1024BYTES		/*/vicadd*/

/* configuration items */

#define RTL_DEV_NAME 			"rtl"	/* device name */
#define RTL_DEV_NAME_LEN 		4	
/*#define RTL81x9_BUFSIZE			1536 */ 	/* aligned packet size */
#define RTL81x9_BUFSIZE			1536 
#define RTL_CRC_SIZE 			4
#define RTL_NUM_TX_DESC			4		/* Number of Tx descriptor registers. */


/*
 * Initialization Block.
 * Specifies addresses of receive and transmit descriptor rings.
 */				  

typedef struct rtlIB
    {
    ULONG       rtlIBMode;		 /* mode register */
    UCHAR       rtlIBPadr [8];   /* PADR: bswapd ethnt phys address */
    UCHAR       rtlIBLadrf [8];  /* logical address filter */
    USHORT      rtlIBRdraLow;    /* RDRA: read ring lower address */
    USHORT      rtlIBRdraHigh;   /* RDRA: read ring upper address */
    } rtl_ib;

#define RTL_ADDR_TO_IB_RMD(addr, ib, rsize) \
	ib->rtlIBRdraLow = (u_long) addr; \
	ib->rtlIBRdraHigh = (((u_long) addr >> 16) & 0xff) | (rsize << 13);

/* Receive Message Descriptor Entry.*/
#if 0
typedef struct rtlRxData
    {
    USHORT      rxStatusFlags;
    USHORT      rxPktLen;
    UCHAR	   	*pktData;
    } RTL_RX_DATA;
#endif

typedef struct rtlRMD
    {
    USHORT      rxStatusFlags;
    USHORT      rxPktLen;
    UCHAR	   	*pktData;
    } RTL_RMD;

/* clear the logical address filter */
    
#define RTL_ADDRF_CLEAR(pIb) (bzero(&(pIb)->rtlIBLadrf[0], 8))

/* set the logical address filter to accept a new multicast address */
    
#define RTL_ADDRF_SET(pIb, crc) \
    (((pIb)->rtlIBLadrf[((crc) & 0x0000003f) >> 3]) |= (1 << ((crc) & 0x7)))


/* This driver supports the following four variants of the RTL Fast */
/* Ethernet Controller Chip. 8129, 8139 - Single Chip Fast	*/
/* Ethernet Controller. 8139A=8139 with power saving. 8139B=8139A + Cardbus*/
/* PCI/CardBus support												*/
 
#define	RTL_DEV_8129	1
#define	RTL_DEV_8139A	2
#define	RTL_DEV_8139B	3
#define	RTL_DEV_8139C	4		/* almost like  8139C */


/*
 * RealTek PCI vendor ID
 */
#define	RTL_VENDORID				0x10EC

/*
 * RealTek chip device IDs.
 */
#define	RTL_DEVICEID_8129			0x8129
#define	RTL_DEVICEID_8139			0x8139

/*
 * Accton PCI vendor ID
 */
#define ACCTON_VENDORID				0x1113

/*
 * Accton MPX 5030/5038 device ID.
 */
#define ACCTON_DEVICEID_5030		0x1211

/*
 * Delta Electronics Vendor ID.
 */
#define DELTA_VENDORID				0x1500

/*
 * Delta device IDs.
 */
#define DELTA_DEVICEID_8139			0x1360

/*
 * Addtron vendor ID.
 */
#define ADDTRON_VENDORID			0x4033

/*
 * Addtron device IDs.
 */
#define ADDTRON_DEVICEID_8139		0x1360


#if _BYTE_ORDER==_BIG_ENDIAN
#define PCI_SWAP(x)		LONGSWAP((int)(x))
#define PCI_WORD_SWAP(x)	((LSB((USHORT)(x)) << 8) | MSB((USHORT)(x)))
#else
#define PCI_SWAP(x)		(x)
#define PCI_WORD_SWAP(x)	(x)
#endif


/*
 * Generic shared memory read and write macros.
 */

#ifndef SHMEM_RD
#    define SHMEM_RD(x)	*(x)
#endif

#ifndef SHMEM_WR
#    define SHMEM_WR(x,y)	(*(x) = y)
#endif

/* RTL81x9 Registers */

#define RTL_REGS_IDR0				0x00
#define RTL_REGS_IDR1				0x01
#define RTL_REGS_IDR2				0x02
#define RTL_REGS_IDR3				0x03
#define RTL_REGS_IDR4				0x04
#define RTL_REGS_IDR5				0x05
#define RTL_REGS_MAR0				0x08	/* Multicast filter. */
#define RTL_REGS_TX_STATUS0			0x10	
#define RTL_REGS_TX_STATUS1			0x14	
#define RTL_REGS_TX_STATUS2			0x18	
#define RTL_REGS_TX_STATUS3			0x1C	
#define RTL_REGS_TX_ADDR0			0x20
#define RTL_REGS_TX_ADDR1			0x24
#define RTL_REGS_TX_ADDR2			0x28
#define RTL_REGS_TX_ADDR3			0x29
#define RTL_REGS_RX_BUF				0x30
#define RTL_REGS_RX_EARLY_CNT		0x34
#define RTL_REGS_RX_EARLY_STATUS	0x36
#define RTL_REGS_CHIP_CMD			0x37
#define RTL_REGS_RX_BUF_PTR			0x38
#define RTL_REGS_RX_BUF_ADDR		0x3A
#define RTL_REGS_INTR_MASK			0x3C
#define RTL_REGS_INTR_STATUS		0x3E
#define RTL_REGS_TX_CONFIG			0x40
#define RTL_REGS_RX_CONFIG			0x44
#define RTL_REGS_TIMER				0x48	/* A general-purpose counter. */
#define RTL_REGS_RX_MISSED			0x4C	/* 24 bits valid, write clears. */
#define RTL_REGS_CFG_9346			0x50
#define RTL_REGS_CONFIG_0			0x51
#define RTL_REGS_CONFIG_1			0x52
#define RTL_REGS_TIMERINT			0x54
#define RTL_REGS_MSR				0x58
#define RTL_REGS_CONFIG3			0x59
#define RTL_REGS_MII				0x5A		/* 8129 chip only */
#define RTL_REGS_MULTI_INTR			0x5C
#define RTL_REGS_PCI_RERID			0x5e
#define RTL_REGS_TX_SUMMARY			0x60
#define RTL_REGS_MII_BMCR			0x62
#define RTL_REGS_MII_BMSR			0x64
#define RTL_REGS_NWAY_ADVERT		0x66
#define RTL_REGS_NWAY_LPAR			0x68
#define RTL_REGS_NWAY_EXPANSION		0x6A
#define RTL_REGS_DIS				0x6c
#define	RTL_REGS_FCSC				0x6e
#define RTL_REGS_NWAYTR				0x70	/* N-Way Test Register */
#define RTL_REGS_CSCR				0x74	/* Chip Status and Configuration Register. */
#define RTL_REGS_PARA_78			0x78
#define RTL_REGS_PARA_7c			0x7c	/* Magic transceiver parameter register. */


/* Rx Status Bits - From Rx Packet Header */

#define	RTL_RX_MULTICAST		0x8000
#define	RTL_RX_PHYSICAL			0x4000
#define	RTL_RX_BROADCAST		0x2000
#define	RTL_RX_BAD_SYMBOL		0x0020
#define	RTL_RX_RUNT				0x0010
#define	RTL_RX_TOO_LONG			0x0008
#define	RTL_RX_CRC_ERROR		0x0004
#define	RTL_RX_BAD_ALIGN		0x0002
#define	RTL_RX_STATUS_OK		0x0001

#define RTL_RX_UNFINISHED		0xfff0		/* DMA still in progress */


/* Tx Status Bits */
/* TSD0-3 - Offset 0010h - 001Fh - R/W */

#define	RTL_TX_HOST_OWNS		0x2000
#define	RTL_TX_UNDERRUN			0x4000
#define RTL_TX_STATUS_OK		0x8000
#define	RTL_TX_OUT_OF_WINDOW	0x20000000
#define	RTL_TX_ABORTED			0x40000000
#define	RTL_TX_CARRIER_LOST		0x80000000

/* Chip Command Bits */
/* Offset 0037h - R/W */

#define RTL_CMD_RESET			0x10
#define RTL_CMD_RX_ENB			0x08
#define RTL_CMD_TX_ENB			0x04
#define RTL_CMD_RX_BUF_EMPTY	0x01


/* Interrupt Mask & Status register bits */
/* Mask 	- Offset 003Ch-003Dh - R/W */
/* Status 	- Offset 003Eh-003Fh - R/W */

#define RTL_IPT_PCI_ERR			0x8000
#define RTL_IPT_PCS_TIMEOUT		0x4000		/* 8129 only */
#define RTL_IPT_CABLE_LEN_CHG	0x2000
#define RTL_IPT_RX_FIFO_OVER	0x40		/* 8139 only */
#define RTL_IPT_RX_UNDERUN		0x20
#define RTL_IPT_RX_OVERFLOW		0x10
#define RTL_IPT_TX_ERR			0x08
#define RTL_IPT_TX_OK			0x04
#define RTL_IPT_RX_ERR			0x02
#define RTL_IPT_RX_OK			0x01

/*#define RTL_VALID_INTERRUPTS	0xe07f*/	/* All Interrupts above */
#define RTL_VALID_INTERRUPTS	0x003f	/* All Interrupts above  vicadd */

/*#define RTL_ERROR_INTERRUPTS	0x807a*/	/* All bar OK's, TOUT and LEN_CHG */
#define RTL_ERROR_INTERRUPTS	0x007a  /*vicadd*/

#define RTL_RX_INTERRUPTS		0x73
#define RTL_TX_INTERRUPTS		0x0c
#define RTL_CONFIG_INTERRUPTS	0xe000 /* PCI,PCS,CABLE */
								

/* Transmit Config Regs */
/* Offset 0040-0043 */

#define RTL_TXCG_HWVER			0x80000000 /* 7 for RTL8139 (A and B) */
										   /* 6 for RTL8139 */

#define RTL_TXCG_BLID			0x40000000 /* DevType 0 - A 1 - B) */

#define RTL_TXCG_LBK1			0x40000		/* Loopback Test */
#define RTL_TXCG_LBK0			0x20000		/* 00 - Normal */
											/* 01 - MAC */
											/* 10 - PHY */
											/* 11 - Twister */

#define RTL_TXCG_CRC			0x10000		/* Append CRC */

#define RTL_TXCG_MXDMA2			0x400		/* Max DMA Burst Size */
#define RTL_TXCG_MXDMA1			0x200		/* 000 - 16   001 - 32   */
#define RTL_TXCG_MXDMA0			0x100		/* 010 - 64	  011 - 128  */
											/* 100 - 256  101 - 512  */
											/* 110 - 1024 111 - 2048 */

#define RTL_TXCG_TXRR			0x80		/* Retry Count - 16 * TXRR */

#define RTL_TXCG_CLRABT			0x01		/* Re transmit */

#define RTL_TXCG_MAXDMA			0x00000700	/* max DMA burst size */
#define RTL_TXCG_LOOPBKTST		0x00060000	/* loopback test */
#define RTL_TXCG_IFG			0x03000000	/* interframe gap */


/* Receive Configuration Register */
/* Offset 0044-0047h */

/* Note the Early Threshold bits are set by modifying the sysRtl81x9.c 			*/
/* file stored within BSP directory. Altering the define RTL81X9_EARX_THRESH 	*/
/* sets the Ealry threshold required in the passed endload string and then		*/
/* passed correctly into the configuration routine within the rtl81x9Start 		*/
/* routine.																		*/

#define RTL_RXCG_ERTH			0x40000		/* Early Rx Threshold Multiplier */
											/* 0 - None 1 - 1/16 2 - 2/16	 */
											/* etc. etc. 					 */

#define RTL_RXCG_MULERINT		0x20000		/* Multiple Early interrupt */
#define RTL_RXCG_RER8			0x10000		/* Rx Packet Length Error	*/

#define RTL_RXCG_RXFTH2			0x8000		/* Rx FIFO Threshold in Bytes */
#define RTL_RXCG_RXFTH1			0x4000		/* 000 - 16   , 001 - 32   */
#define RTL_RXCG_RXFTH0			0x2000		/* 010 - 64   , 011 - 128  */
											/* 100 - 256  , 101 - 512  */
											/* 110 - 1024 , 111 - none */

#define RTL_RXCG_RBLEN1			0x1000		/* Rx Buffer Length			*/
#define RTL_RXCG_RBLEN0			0x800		/* 00 - 8k  + 16bytes */
											/* 01 - 16k + 16bytes */
											/* 10 - 32k + 16bytes */
											/* 11 - 64k + 16bytes */

#define RTL_RXCG_MXDMA2			0x400		/* Max DMA Burst size per RX DMA */
#define RTL_RXCG_MXDMA1			0x200		/* 000 - 16   , 001 - 32   */
#define RTL_RXCG_MXDMA0			0x100		/* 010 - 64   , 011 - 128  */								      
											/* 100 - 256  , 101 - 512  */
											/* 110 - 1024 , 111 - unlimited */

#define RTL_RXCG_WRAP			0x80			
#define RTL_RXCG_9356SEL		0x40		/* Selects EEPROM 1 - 9356 0 - 9346 */			
#define RTL_RXCG_AER			0x20		/* Accept Error Packet */			
#define RTL_RXCG_AR				0x10		/* Accept RUNT */			
#define RTL_RXCG_AB				0x08		/* Accept Broadcast */			
#define RTL_RXCG_AM				0x04		/* Accept Multicast */			
#define RTL_RXCG_APM			0x02		/* Accept Physical match */			
#define RTL_RXCG_AAP			0x01		/* Accept all Phy */			

/* The following are used as the initial Tx and Rx Configs */

#define RTL_RXCFG_CONFIG 	(RTL_RX_BUF_SZ|RTL_RX_FIFOTHRESH|RTL_RX_MAXDMA)
#define RTL_TXCFG_CONFIG	(RTL_TXCG_IFG|RTL_TX_MAXDMA)


/* 9346CR Command Register */
/* Offset 0050h - R/W */

#define RTL_EEMODE_PROGRAM		0x80		/* 8139B Operating Mode */
#define RTL_EEMODE_AUTOLOAD		0x40		/* 00 - Normal */
											/* 01 - Auto Load */
											/* 10 - 93c46 Prog Mode */
											/* 11 - Cfg Reg Write Mode */
#define RTL_EEMODE_WRITECFG		(0x80|0x40)
#define RTL_EEMODE_OFF			0x00

/* MII register (8129 only) */
/* Offset 005Ah - R/W */

#define RTL_MII_CLK			0x01
#define RTL_MII_DATAIN		0x02
#define RTL_MII_DATAOUT		0x04
#define RTL_MII_DIR			0x80	/* 0 == input, 1 == output */

/*
 * EEPROM control register
 */

#define RTL_EE_DATAOUT			0x01		/* Data out */
#define RTL_EE_DATAIN			0x02		/* Data in */
#define RTL_EE_CLK				0x04		/* clock */
#define RTL_EE_SEL				0x08		/* chip select */
#define RTL_EE_MODE				(0x40|0x80)

/* 9346 EEPROM commands */
#define RTL_EECMD_WRITE			0x140
#define RTL_EECMD_READ			0x180
#define RTL_EECMD_ERASE			0x1c0

#define RTL_EE_ID				0x00
#define RTL_EE_PCI_VID			0x01
#define RTL_EE_PCI_DID			0x02

/* Location of station address inside EEPROM */
#define RTL_EE_EADDR			0x07




/* Configuration Register 0 */
/* Offset 0051h - R/W */

#define RTL_CONFIG0_BS2			0x04		/* Boot ROM Size */
#define RTL_CONFIG0_BS1			0x02		/* 000 - No  Boot Rom */
#define RTL_CONFIG0_BS0			0x01		/* 001 - 8k  Boot Rom */
											/* 010 - 16k Boot Rom */
											/* 011 - 32k Boot Rom */
											/* 100 - 64k Boot Rom */
											/* 101 - 128 Boot Rom */
											/* 110 - unused */
											/* 111 - unused */

/* Configuration Register 1 */
/* Offset 0052h - R/W */

#define RTL_CONFIG1_LEDS1		0x80		/* LED's */
#define RTL_CONFIG1_LEDS0		0x40		/* Refer to LED IFace */
#define RTL_CONFIG1_DVRLOAD		0x20		/* Driver loaded status bit */
#define RTL_CONFIG1_LWACT		0x10		/* LWAKE Active mode */
#define RTL_CONFIG1_MEMMAP		0x08		/* Registers Mapped to PCI Memory */
#define RTL_CONFIG1_IOMAP		0x04		/* Registers Mapped to PCI I/O space */
#define RTL_CONFIG1_VPD			0x02		/* Vital Product Data */
#define RTL_CONFIG1_PMEN		0x01		/* Power Management Enable */

/* Media Status Bits 	*/
/* Offset 0058h - R/W 	*/

#define RTL_MED_RXPF				0x01	/* Rx Pause Flag */
#define RTL_MED_TXPF				0x02	/* Tx Pause Flag */
#define	RTL_MED_LINKB				0x04	/* Inverse of Link Status */
											/* 0 - Link OK */
#define	RTL_MED_SPEED				0x08    /* 1 - 10Mbps, 0 - 100Mbps */
#define RTL_MED_AUX					0x10	/* Aux power Status */
#define	RTL_MED_RXFCE				0x40	/* Rx Flow Control enable */
#define	RTL_MED_TXFCE				0x80	/* Tx Flow control enable */

/* Configuration Register 3	*/
/* Offset 0059h - R/W 		*/

#define RTL_CONFIG3_GNTSEL			0x80	/* Grant Select */
#define RTL_CONFIG3_PARM_EN			0x40	/* Paramater Enable - 100Mbps only */
#define RTL_CONFIG3_MAGIC			0x20	/* Magic Packet */
#define RTL_CONFIG3_LINKUP			0x10
#define RTL_CONFIG3_CARDB_EN		0x08	/* Card Bus Enable */
#define RTL_CONFIG3_CLKRUN_EN		0x04
#define RTL_CONFIG3_FUNCREG_EN		0x02	/* Functions registers enable */
#define RTL_CONFIG3_FBTBEN			0x01	/* Fast back to back enable */

/* Configuration Register 4 */
/* Offset 005Ah - R/W 		*/

#define RTL_CONFIG4_RXFIFO_CLR		0x80	/* Automatically clear RX FIFO OFlow */
#define RTL_CONFIG4_ANAOFF			0x40	/* Analogue Power Off */
#define RTL_CONFIG4_LONGWF			0x20	/* Long Wake up Frame */
#define RTL_CONFIG4_LWPME			0x10	/* LANWAKE vs PMEB */
#define RTL_CONFIG4_MSWFB			0x08	/* Microsoft Wake up frame */
#define RTL_CONFIG4_LWPTN			0x04	/* LWAKE Pattern */
#define RTL_CONFIG4_PARM_EN2		0x02	/* Parameter Enable 2  */
#define RTL_CONFIG4_RD_AUX			0x01	/* Rev d - Aux Power Status */


/* Transmit Status of all Descriptors */
/* Offset 0060 - 0061h - R/W 		  */
/* RTL_REGS_TX_SUMMARY			      */

#define RTL_TSAD_TOK_MSK			0xf000	/* Tx Ok Descs          */
#define RTL_TSAD_TUN_MSK			0x0f00	/* Tx Underrun Descs    */
#define RTL_TSAD_TABT_MSK			0x00f0	/* Transmit Abort Descs */
#define RTL_TSAD_OWN_MSK			0x000f	/* OWN Bit Descriptors  */

/* Basic Mode Control Register 	*/
/* Offset 0062-0063h - R/W 		*/
/* Note : All Default are 0 	*/

#define	RTL_BMCR_RESET				0x8000	/* Sets Status and Control Regs */
											/* 1=Software reset 0=normal op */

#define	RTL_BMCR_SPD_SET			0x2000	/* Network Speed */
											/* 1=100Mbps, 0=10Mbps */

#define	RTL_BMCR_ANE				0x1000	/* Auto Negotiation Enable */
#define	RTL_BMCR_RAN				0x0200	/* Restart Auto Negotiation */
#define	RTL_BMCR_DUPX				0x0100	/* Duplex mode */
											/* 1=full duplex, 0=normal */
/* Basic Mode Status Register 	*/
/* Offset 0064-0065h - R/W 		*/

#define	RTL_BMSR_100BASE_T4			0x8000	/* 1=enable, default = 0 */
#define	RTL_BMSR_100BASE_TX_FDUPX	0x4000	/* 1=enable, default = 1 */
#define	RTL_BMSR_100BASE_TX_HDUPX	0x2000	/* 1=enable, default = 1 */
#define	RTL_BMSR_10BASE_T_FDUPX		0x1000	/* 1=enable, default = 1 */
#define	RTL_BMSR_10BASE_T_HDUPX		0x0800	/* 1=enable, default = 1 */

#define	RTL_BMSR_AUTO_NEG_COMP		0x0020	/* 1=completed,   default = 0 */
#define	RTL_BMSR_REMOTE_FAULT		0x0010	/* 1=detected,    default = 0 */
#define	RTL_BMSR_AUTO_NEG_FAIL		0x0008	/* 1=failed,      default = 1 */
#define	RTL_BMSR_LINK_STATUS		0x0004	/* 1=established, default = 0 */
#define	RTL_BMSR_JABBER_DETECT		0x0002	/* 1=detected,    default = 0 */
#define	RTL_BMSR_EXT_CAPABILITY		0x0001	/* 1=extended,    default = 1 */
											/* 0=basic regs */

/* Auto Negotiation Advertisement Register 	*/
/* Offset 0066-0067h - R/W 		*/

#define	RTL_ANAR_NP			0x8000	/* Next Page Bit */
									/* 0=primary capability data page 	*/
									/* 1=protocol specific data page	*/
									/* default = 0 */

#define	RTL_ANAR_ACK		0x4000	/* 1=acknowledge, default = 0 */

#define	RTL_ANAR_RF			0x2000	/* 1=advertise remote fault detection	*/
									/*  default = 0 */

#define	RTL_ANAR_PAUSE		0x0400	/* 1=flow control supported locally */
									/* default comes from EEPROM */

#define	RTL_ANAR_T4			0x0200	/* 1=100Base-T4 supported */
									/* default = 0 */

#define	RTL_ANAR_TXFD		0x0100	/* 1=100Base-Tx full duplex supported */
									/* default = 1 */

#define	RTL_ANAR_TX			0x0080	/* 1=100Base-Tx supported */
									/* default = 1 */

#define	RTL_ANAR_10FD		0x0040	/* 1=10Base-T full duplex supported */
									/* default = 1 */

#define	RTL_ANAR_10			0x0020	/* 1=10Base-T supported */
									/* default = 1 */

#define	RTL_ANAR_SELECTOR	0x0001	/* Binary Encoded Selector */
									/* Currently only CSMA/CD <00001> */
									/* Supported */
									/* default = 00001 */

/*  EEPROM_Ctrl bits. */

/* eeprom contents */
    
#define RTL_EE_OEM_ADR0		0x0E
#define	RTL_EE_OEM_ADR1		0x11
#define RTL_EE_OEM_ADR2		0x13

#define	RTL_VENDOR_ID		0x10ec
#define	RTL_VENDOR_ID_MIS	0x1113
#define	RTL_8129_DEV_ID		0x8129
#define	RTL_8139_DEV_ID 	0x8139
#define	RTL_MIS_DEV_ID 		0x1211

#define	RTL_DEV_MASK		0xffff
#define	RTL_IO_SIZE			0x80

#define RTL_TIMEOUT			1000

#define RTL_WIN_0    		0

#define RTL_MIN_FBUF     100    /* Minimum size of the first buffer in a */
                               /* chain. */

typedef struct rtl_mii_frame {
	UINT8			mii_stdelim;
	UINT8			mii_opcode;
	UINT8			mii_phyaddr;
	UINT8			mii_regaddr;
	UINT8			mii_turnaround;
	USHORT			mii_data;
} RTL_MII_FRAME ;

/*
 * MII constants
 */

#define RTL_MII_STARTDELIM	0x01
#define RTL_MII_READOP		0x02
#define RTL_MII_WRITEOP		0x01
#define RTL_MII_TURNAROUND	0x02

/*
 * Texas Instruments PHY identifiers
 */
#define TI_PHY_VENDORID		0x4000
#define TI_PHY_10BT			0x501F
#define TI_PHY_100VGPMI		0x502F

/*
 * These ID values are for the NS DP83840A 10/100 PHY
 */
#define NS_PHY_VENDORID		0x2000
#define NS_PHY_83840A		0x5C0F

/*
 * Level 1 10/100 PHY
 */
#define LEVEL1_PHY_VENDORID	0x7810
#define LEVEL1_PHY_LXT970	0x000F

/*
 * Intel 82555 10/100 PHY
 */
#define INTEL_PHY_VENDORID	0x0A28
#define INTEL_PHY_82555		0x015F

/*
 * SEEQ 80220 10/100 PHY
 */
#define SEEQ_PHY_VENDORID	0x0016
#define SEEQ_PHY_80220		0xF83F


#define RTL_PHY_UNKNOWN			6

#define RTL_PHYADDR_MIN			0x00
#define RTL_PHYADDR_MAX			0x1F

#define RTL_PHY_BMCR			0x00
#define RTL_PHY_BMSR			0x01
#define RTL_PHY_VENID			0x02
#define RTL_PHY_DEVID			0x03
#define RTL_PHY_ANAR			0x04
#define RTL_PHY_LPAR			0x05
#define RTL_PHY_ANEXP			0x06

#define RTL_PHY_ANAR_NEXTPAGE	0x8000
#define RTL_PHY_ANAR_RSVD0		0x4000
#define RTL_PHY_ANAR_TLRFLT		0x2000
#define RTL_PHY_ANAR_RSVD1		0x1000
#define RTL_PHY_ANAR_RSVD2		0x0800
#define RTL_PHY_ANAR_RSVD3		0x0400
#define RTL_PHY_ANAR_100BT4		0x0200
#define RTL_PHY_ANAR_100BTXFULL	0x0100
#define RTL_PHY_ANAR_100BTXHALF	0x0080
#define RTL_PHY_ANAR_10BTFULL	0x0040
#define RTL_PHY_ANAR_10BTHALF	0x0020
#define RTL_PHY_ANAR_PROTO4		0x0010
#define RTL_PHY_ANAR_PROTO3		0x0008
#define RTL_PHY_ANAR_PROTO2		0x0004
#define RTL_PHY_ANAR_PROTO1		0x0002
#define RTL_PHY_ANAR_PROTO0		0x0001

/*
 * These are the register definitions for the PHY (physical layer
 * interface chip).
 */
/*
 * PHY BMCR Basic Mode Control Register
 */

#define RTL_PHY_BMCR_RESET			0x8000
#define RTL_PHY_BMCR_LOOPBK			0x4000
#define RTL_PHY_BMCR_SPEEDSEL		0x2000
#define RTL_PHY_BMCR_AUTONEGENBL		0x1000
#define RTL_PHY_BMCR_RSVD0			0x0800	/* write as zero */
#define RTL_PHY_BMCR_ISOLATE		0x0400
#define RTL_PHY_BMCR_AUTONEGRSTR		0x0200
#define RTL_PHY_BMCR_DUPLEX			0x0100
#define RTL_PHY_BMCR_COLLTEST		0x0080
#define RTL_PHY_BMCR_RSVD1			0x0040	/* write as zero, don't care */
#define RTL_PHY_BMCR_RSVD2			0x0020	/* write as zero, don't care */
#define RTL_PHY_BMCR_RSVD3			0x0010	/* write as zero, don't care */
#define RTL_PHY_BMCR_RSVD4			0x0008	/* write as zero, don't care */
#define RTL_PHY_BMCR_RSVD5			0x0004	/* write as zero, don't care */
#define RTL_PHY_BMCR_RSVD6			0x0002	/* write as zero, don't care */
#define RTL_PHY_BMCR_RSVD7			0x0001	/* write as zero, don't care */

/*
 * RESET: 1 == software reset, 0 == normal operation
 * Resets status and control registers to default values.
 * Relatches all hardware config values.
 *
 * LOOPBK: 1 == loopback operation enabled, 0 == normal operation
 *
 * SPEEDSEL: 1 == 100Mb/s, 0 == 10Mb/s
 * Link speed is selected byt his bit or if auto-negotiation if bit
 * 12 (AUTONEGENBL) is set (in which case the value of this register
 * is ignored).
 *
 * AUTONEGENBL: 1 == Autonegotiation enabled, 0 == Autonegotiation disabled
 * Bits 8 and 13 are ignored when autoneg is set, otherwise bits 8 and 13
 * determine speed and mode. Should be cleared and then set if PHY configured
 * for no autoneg on startup.
 *
 * ISOLATE: 1 == isolate PHY from MII, 0 == normal operation
 *
 * AUTONEGRSTR: 1 == restart autonegotiation, 0 = normal operation
 *
 * DUPLEX: 1 == full duplex mode, 0 == half duplex mode
 *
 * COLLTEST: 1 == collision test enabled, 0 == normal operation
 */

/* 
 * PHY, BMSR Basic Mode Status Register 
 */   
#define RTL_PHY_BMSR_100BT4			0x8000
#define RTL_PHY_BMSR_100BTXFULL		0x4000
#define RTL_PHY_BMSR_100BTXHALF		0x2000
#define RTL_PHY_BMSR_10BTFULL		0x1000
#define RTL_PHY_BMSR_10BTHALF		0x0800
#define RTL_PHY_BMSR_RSVD1			0x0400	/* write as zero, don't care */
#define RTL_PHY_BMSR_RSVD2			0x0200	/* write as zero, don't care */
#define RTL_PHY_BMSR_RSVD3			0x0100	/* write as zero, don't care */
#define RTL_PHY_BMSR_RSVD4			0x0080	/* write as zero, don't care */
#define RTL_PHY_BMSR_MFPRESUP		0x0040
#define RTL_PHY_BMSR_AUTONEGCOMP	0x0020
#define RTL_PHY_BMSR_REMFAULT		0x0010
#define RTL_PHY_BMSR_CANAUTONEG		0x0008
#define RTL_PHY_BMSR_LINKSTAT		0x0004
#define RTL_PHY_BMSR_JABBER			0x0002
#define RTL_PHY_BMSR_EXTENDED		0x0001

/* ethernet media */

#define IFM_ETHER		0x00000020
#define	IFM_10_T		3		/* 10BaseT - RJ45 */
#define	IFM_10_2		4		/* 10Base2 - Thinnet */
#define	IFM_10_5		5		/* 10Base5 - AUI */
#define	IFM_100_TX		6		/* 100BaseTX - RJ45 */
#define	IFM_100_FX		7		/* 100BaseFX - Fiber */
#define	IFM_100_T4		8		/* 100BaseT4 - 4 pair cat 3 */
#define	IFM_100_VG		9		/* 100VG-AnyLAN */
#define	IFM_100_T2		10		/* 100BaseT2 */

/* Shared media sub-types */

#define	IFM_AUTO		0	/* Autoselect best media */
#define	IFM_MANUAL		1	/* Jumper/dipswitch selects media */
#define	IFM_NONE		2	/* Deselect all media */

/* Shared options */

#define IFM_FDX			0x00100000	/* Force full duplex */
#define	IFM_HDX			0x00200000	/* Force half duplex */
#define IFM_FLAG0		0x01000000	/* Driver defined flag */
#define IFM_FLAG1		0x02000000	/* Driver defined flag */
#define IFM_FLAG2		0x04000000	/* Driver defined flag */
#define	IFM_LOOP		0x08000000	/* Put hardware in loopback */

/* Masks */

#define	IFM_NMASK		0x000000e0	/* Network type */
#define	IFM_TMASK		0x0000000f	/* Media sub-type */
#define	IFM_IMASK		0xf0000000	/* Instance */
#define	IFM_ISHIFT		28		/* Instance shift */
#define	IFM_OMASK		0x0000ff00	/* Type specific options */
#define	IFM_GMASK		0x0ff00000	/* Global options */

/* Status bits */

#define	IFM_AVALID		0x00000001	/* Active bit valid */
#define	IFM_ACTIVE		0x00000002	/* Iface bound to working net */

/*
 * Macros to extract various bits of information from the media word.
 */
#define	IFM_TYPE(x)	((x) & IFM_NMASK)
#define	IFM_SUBTYPE(x)	((x) & IFM_TMASK)
#define	IFM_INST(x)	(((x) & IFM_IMASK) >> IFM_ISHIFT)

#define RTL_FLAG_FORCEDELAY	1
#define RTL_FLAG_SCHEDDELAY	2
#define RTL_FLAG_DELAYTIMEO	3	

/* Configuration items */

#define RMD_SIZ	sizeof(RTL_RMD)
#define IB_SIZ	sizeof(rtl_ib)

/* 
* load string format 
* <devMemAddr>:<devIoAddr>:<pciMemBase:<vecnum>:<intLvl>:<memAdrs>: 
* <memSize>:<memWidth>:<csr3b>:<offset>:<flags> 
*/
#define RTL_81X9_BUFF_LOAN_0      1
/* First entry loc */
#define RTL_81X9_LOAD_COUNT       4

IMPORT END_OBJ * sysRtl81x9EndLoad(char *, void *);

#ifdef __cplusplus
}
#endif

#endif /* __INCrtl81x9Endh */
