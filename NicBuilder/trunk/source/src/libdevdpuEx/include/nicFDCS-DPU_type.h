/**
 * @file
 * @brief ��������
 * @author Xiaosong Gao
 * @note ������FDCS-DPUͨ��Э���У���Ҫ�����������������
 */

#ifndef __NICFDCS_DPU_TYPE_H__
#define __NICFDCS_DPU_TYPE_H__

#include <ace/Basic_Types.h>
#include "nicFDCS-DPU_const.h"

#pragma pack(push, 1)

/** ��ǩ������� */
struct IO_T {
	ACE_UINT8 dpu;				/**< �������߼���ַ */
	ACE_UINT8 iocard;			/**< IO���߼���ַ */
	ACE_UINT16 channel;			/**< IOͨ����� */
	ACE_UINT8 subchannel;		/**< IO��ͨ����ţ�datatypeΪBOOL_1_BITʱ��Ч */
	ACE_UINT16 startchannel;	/**< IO������ʼͨ����(ȫ��) */
	ACE_UINT16 endchannel;		/**< IO������ֹͨ����(ȫ��) */
	ACE_UINT8 datatype;			/**< ���������� */
	union {
		bool b;					/**< 1Bit��BOOL */
		ACE_INT8 c;				/**< 1�ֽ����� */
		ACE_UINT8 uc;			/**< 1�ֽ��޷������� */
		ACE_INT16 s;			/**< 2�ֽ����� */
		ACE_UINT16 us;			/**< 2�ֽ��޷������� */
		ACE_INT32 g;			/**< 4�ֽ����� */
		ACE_UINT32 ug;			/**< 4�ֽ��޷������� */
		float f;				/**< 4�ֽڸ���(3�ֽڸ��㣬2�ֽڸ���) */
	} databuf;					/**< ������ */
	ACE_UINT16 qbuf;			/**< ������ */
	ACE_UINT64 timemark;		/**< ʱ�� */
};

/** ͨ��Э����һ��֡�����ݰ��ĸ�ʽ */
struct FD_PACKET {
	ACE_UINT8 sync_header;		/**< ͬ��ͷ */
	ACE_UINT8 dest_addr;		/**< Ŀ�� */
	ACE_UINT8 source_addr;		/**< Դ */
	ACE_UINT8 length_low;		/**< ���ȵ� */
	ACE_UINT8 length_high;		/**< ���ȸߣ���ΪЭ��汾�� */
	ACE_UINT8 control_area_1;	/**< ������1 */
	ACE_UINT8 control_area_2;	/**< ������2 */
	ACE_UINT8 control_area_3;	/**< ������3 */
	ACE_UINT8 protocol_type;	/**< Э������ */
	ACE_UINT8 info_code;		/**< ��Ϣ�� */
	ACE_UINT8 description_code;	/**< ������ */
	ACE_UINT8 * data;			/**< ���� */
	ACE_UINT16 crc16;			/**< CRC16У�� */
};

/** ʱ�� */
typedef ACE_UINT64 TIME_MARK;

/** IO������ */
typedef enum {
	IO_CARD,	/**< IO */
	IED_CARD,	/**< IED */
	VPT_CARD	/**< VPT */
} IO_TYPE;

/** IO�����Լ��� */
struct IO_DATA_UNION {
	ACE_UINT8 ioaddr;		/**< IO���߼���ַ */
	ACE_UINT8 type;			/**< IOģ������ */
	IO_TYPE iotype;			/**< IO�忨������ */
	ACE_UINT16 qlen;		/**< �����볤�� */
	ACE_UINT8 * quality;	/**< ������������������Ϊqlen�ֽ� */
	ACE_UINT16 vlen;		/**< ���������� */
	ACE_UINT8 * vvalue;		/**< ������������Ϊvlen�ֽ� */
};

/** IO����Ӧ��ķ��ؽṹ */
struct IO_DATA {
	ACE_UINT8 io_num;						/**< ʵ�ʰ�����IO��Ŀ */
	IO_DATA_UNION io_cards[MAX_IO_CARD];	/**< ����IO�ļ��� */
};

#pragma pack(pop)

#endif	// __NICFDCS_DPU_TYPE_H__