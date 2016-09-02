/**
 * @file
 * @brief 数据类型
 * @author Xiaosong Gao
 * @note 包含了FDCS-DPU通信协议中，需要传输的所有数据类型
 */

#ifndef __NICFDCS_DPU_TYPE_H__
#define __NICFDCS_DPU_TYPE_H__

#include <ace/Basic_Types.h>
#include "nicFDCS-DPU_const.h"

#pragma pack(push, 1)

/** 标签点的类型 */
struct IO_T {
	ACE_UINT8 dpu;				/**< 控制器逻辑地址 */
	ACE_UINT8 iocard;			/**< IO卡逻辑地址 */
	ACE_UINT16 channel;			/**< IO通道序号 */
	ACE_UINT8 subchannel;		/**< IO子通道序号，datatype为BOOL_1_BIT时有效 */
	ACE_UINT16 startchannel;	/**< IO卡的起始通道号(全局) */
	ACE_UINT16 endchannel;		/**< IO卡的终止通道号(全局) */
	ACE_UINT8 datatype;			/**< 数据区类型 */
	union {
		bool b;					/**< 1Bit的BOOL */
		ACE_INT8 c;				/**< 1字节整数 */
		ACE_UINT8 uc;			/**< 1字节无符号整数 */
		ACE_INT16 s;			/**< 2字节整数 */
		ACE_UINT16 us;			/**< 2字节无符号整数 */
		ACE_INT32 g;			/**< 4字节整数 */
		ACE_UINT32 ug;			/**< 4字节无符号整数 */
		float f;				/**< 4字节浮点(3字节浮点，2字节浮点) */
	} databuf;					/**< 数据区 */
	ACE_UINT16 qbuf;			/**< 质量码 */
	ACE_UINT64 timemark;		/**< 时标 */
};

/** 通信协议中一个帧的数据包的格式 */
struct FD_PACKET {
	ACE_UINT8 sync_header;		/**< 同步头 */
	ACE_UINT8 dest_addr;		/**< 目的 */
	ACE_UINT8 source_addr;		/**< 源 */
	ACE_UINT8 length_low;		/**< 长度低 */
	ACE_UINT8 length_high;		/**< 长度高，作为协议版本号 */
	ACE_UINT8 control_area_1;	/**< 控制域1 */
	ACE_UINT8 control_area_2;	/**< 控制域2 */
	ACE_UINT8 control_area_3;	/**< 控制域3 */
	ACE_UINT8 protocol_type;	/**< 协议类型 */
	ACE_UINT8 info_code;		/**< 信息码 */
	ACE_UINT8 description_code;	/**< 描述码 */
	ACE_UINT8 * data;			/**< 数据 */
	ACE_UINT16 crc16;			/**< CRC16校验 */
};

/** 时标 */
typedef ACE_UINT64 TIME_MARK;

/** IO卡类型 */
typedef enum {
	IO_CARD,	/**< IO */
	IED_CARD,	/**< IED */
	VPT_CARD	/**< VPT */
} IO_TYPE;

/** IO卡属性集合 */
struct IO_DATA_UNION {
	ACE_UINT8 ioaddr;		/**< IO板逻辑地址 */
	ACE_UINT8 type;			/**< IO模板类型 */
	IO_TYPE iotype;			/**< IO板卡总类型 */
	ACE_UINT16 qlen;		/**< 质量码长度 */
	ACE_UINT8 * quality;	/**< 质量码数据区，长度为qlen字节 */
	ACE_UINT16 vlen;		/**< 数据区长度 */
	ACE_UINT8 * vvalue;		/**< 数据区，长度为vlen字节 */
};

/** IO数据应答的返回结构 */
struct IO_DATA {
	ACE_UINT8 io_num;						/**< 实际包含的IO数目 */
	IO_DATA_UNION io_cards[MAX_IO_CARD];	/**< 所有IO的集合 */
};

#pragma pack(pop)

#endif	// __NICFDCS_DPU_TYPE_H__