#ifndef __NICSYS_DEV_ONETTYPE_H_
#define __NICSYS_DEV_ONETTYPE_H_
#include "ONetConst.h"
#include "convert_type.h"
//#include "RW_Thread_Mutex.h"

#include <pshpack1.h>

/** ͨ��Э����һ��֡�����ݰ��ĸ�ʽ */
typedef struct FD_PACKET_st {
	__u8 sync_header;		/**< ͬ��ͷ */
	__u8 dest_addr;		/**< Ŀ�� */
	__u8 source_addr;		/**< Դ */
	__u8 length_low;		/**< ���ȵ� */
	__u8 length_high;		/**< ���ȸߣ���ΪЭ��汾�� */
	__u8 control_area_1;	/**< ������1 */
	__u8 control_area_2;	/**< ������2 */
	__u8 control_area_3;	/**< ������3 */
	__u8 protocol_type;	/**< Э������ */
	__u8 info_code;		/**< ��Ϣ�� */
	__u8 description_code;	/**< ������ */
	__u8 * data;			/**< ���� */
	__u16 crc16;			/**< CRC16У�� */
}FD_PACKET_ONet;





/** ����IED���ľ�̬������Ϣ */
struct IED_CARD_ATTR {
	__u8 ioid;				/**< �����ַ���ۺ� */
	__u8 ioaddr;			/**< IED���߼���ַ */
	__u8 status;			/**< ��ʼ̬0x01; ����̬0x02; ����̬0x11; ��̬0x4F; ����̬0x41; ͨѶ�ж�0x42; ���ô���0x44 */
	__u8 type;				/**< IEDģ������ */
	__u16 iotype;			/**< IED�忨���ͣ����ֽ�->���ֽ� */
};

/** ����IED��״̬��Ϣ��Ӧ��DPBUSЭ��ȷ������ṹ */
struct IED_CARD_VDI {
	__u8 vdilen;		/**< ���ݳ��� */
	__u8 * vdidata;		/**< ������������Ϊvdilen�ֽ� */
};

/** ����IED���͵����ݺ������룬ͬʱҲ�����IED�������������� */
struct IED_CARD_VALUE {
	__u8 qlenHigh;		/**< �����볤�ȣ���λ */
	__u8 qlenLow;		/**< �����볤�ȣ���λ */
	__u8 * quality;		/**< ������������������Ϊqlen�ֽ� */
	__u8 vlenHigh;		/**< ���������ȣ���λ */
	__u8 vlenLow;		/**< ���������ȣ���λ */
	__u8 * vvalue;		/**< ������������Ϊvlen�ֽ� */
};


/** ����VPT���ľ�̬������Ϣ */
struct VPT_CARD_ATTR {
	__u8 ioid;				/**< �����ַ���ۺ� */
	__u8 ioaddr;			/**< VPT���߼���ַ */
	__u8 status;			/**< ��ʼ̬0x01; ����̬0x02; ����̬0x11; ��̬0x4F; ����̬0x41; ͨѶ�ж�0x42; ���ô���0x44 */
	__u8 type;				/**< VPTģ������ */
	__u8 linesHigh;			/**< VPT�ĵ�������(��λ) */
	__u8 linesLow;			/**< VPT�ĵ�������(��λ) */
};

/** ����VPT�������볤�ȣ�Ϊ"0x0000" */
struct VPT_CARD_QUALIFY {
	__u16 qualify;		/**< �����볤�� */
};

/** ����VPT���͵����� */
struct VPT_CARD_VALUE {
	__u8 type;			/**< VPT�������� */
	__u8 vlenHigh;		/**< ���������ȣ���λ */
	__u8 vlenLow;		/**< ���������ȣ���λ */
	__u8 * vvalue;		/**< ������������Ϊvlen�ֽ� */
};


/** ����IO���ľ�̬������Ϣ */
struct IO_CARD_ATTR
{
	__u8 ioID;				/**< �����ַ���ۺ� */
	__u8 ioAddr;			/**< IO���߼���ַ */
	__u8 Status;			/**< ��ʼ̬0x01; ����̬0x02; ����̬0x11; ��̬0x4F; ����̬0x41; ͨѶ�ж�0x42; ���ô���0x44 */
	__u8 Type;				/**< IOģ������ */
	__u16 ioType;			/**< IO�忨���ͣ����ֽ�->���ֽ� */
	__u8 ioSoftVer[7];		/**< IO������汾������汾��(���ֽ�)->����汾��(���ֽ�)->���д������:��->���д������:��
								* ->���д������:��(ʵ������2000�Ĳ�ֵ)->CRCУ�����ֵ->CRCУ�����ֵ */
	__u8 ctrlDpu;			/**<  */
};								

/** ����IO���Ķ�̬������Ϣ */
struct IO_DYNA_ATTR
{
	__u8 ioId;				/**< �����ַ���ۺ� */
	__u8 ioAddr;			/**< IO���߼���ַ */
	__u8 status;			/**< ��ʼ̬0x01; ����̬0x02; ����̬0x11; ��̬0x4F; ����̬0x41; ͨѶ�ж�0x42; ���ô���0x44 */
	__u8 Type;				/**< IOģ������ */
};

/** ����IO��״̬��Ϣ��Ӧ��DPBUSЭ��ȷ������ṹ */
struct IO_VDI {
	__u8 vdilen;				/**< ���ݳ��� */
	__u8 vdidata[14];	/**< ������������Ϊvdilen�ֽ� */
};

/** ����IO�����������ͺͷ������� */
struct IO_DATA_ATTR
{
	__u8 Type;					/**< �����ͣ�BITλ0x1F; ���ֽڷ���0x24; ���ֽڷ���0x23; ���ֽڷ���0x22; ���ֽ��޷�������0x34;
									* ���ֽ��޷�������0x32; ���ֽ��޷�������0x31; ���ֽڷ�������0x44; ���ֽڷ�������0x42; ���ֽڷ�������0x41 */
	__u8 Mode;					/**< r=01;w=02; ��дȨ�� */
	__u16 num;					/**< ����Ŀ */
};

/** ����IO���͵����ݺ������룬ͬʱҲ�����IO�������������� */
struct IO_VALUE
{
	__u8 qLen;					/**< �����볤�� */
	__u8 *quaData;					/**< ������������������Ϊqlen�ֽ� */
	__u8 vLen;					/**< ���������� */
	__u8 *valData;				/**< ������������Ϊvlen�ֽ� */
	__u16 ptNum;				/**< �����Ŀ */
	IO_DATA_ATTR attr[4];			/**< �����������������֧��4������ */
};

struct IO_DYNA_VALUE
{
	__u8 qLen;					/**< �����볤�� */
	__u8 *quaData;					/**< ������������������Ϊqlen�ֽ� */
	__u8 vLen;					/**< ���������� */
	__u8 *valData;				/**< ������������Ϊvlen�ֽ� */
};									

struct DPU_ATTR
{
	__u8 dpuId;			//ID =255
	__u8 dpuAddr;		//�߼���ַ 1-99
	__u16 dpuStatus;	// Byte0:Ĭ����CM��DPU״̬; Bit0:����; Bit1:����; Bit2:����̬; Bit3:����ʹ��\n
										// Byte1:Ĭ�ϱ�CM��DPU״̬; Bit0:����; Bit1:����; Bit2:����̬; Bit3:����ʹ�� */
	__u8 dpuType;		//����
	__u8 dpuIOCap;		//���IO��Ŀ
	__u8 dpuIOSize;		//���ʵIO��Ŀ
	__u8 dpuVirIOSize;	//�����IO��Ŀ	
	__u8 dpuRunIONum;	//����ʵIO��Ŀ
	__u8 dpuRunVirIONum;//������IO��Ŀ
	char dpuVersion[32];	//DPU����汾��
	char	plcVersion[32];	//PLC����汾��
	__u32 oNetIpAddr1;	//IP��ַ1
	__u32 oNetIpAddr2;	//IP��ַ2
	__u32 oNetIpAddr3;	//IP��ַ3
	__u32 oNetIpAddr4;	//IP��ַ4
	__u32 syncIpAddr;	//ͬ��IP��ַ
};
struct DPU_STATUS
{
	__u8 temp;			//�¶�
	__u8 cpu;			//CPU�����ʣ�%��
	__u8 mem;			//�ڴ�ʹ��[MByte]
	__u8 hardware;		//Ӳ��״̬
	__u8 dpBus1;			//DpBus1״̬
	__u8 dpBus2;			//DpBus2״̬
	__u8 comm;			//comm״̬
	__u8 heartbeat;		//ͬ��������״̬	
	__u8 dpbus_state;	//DP��վ״̬
	__u8 baud_ratestatus;//dpbus��վ������
	__u32 HostSendCount;							//��վ�����ܴ���
	__u32 HostRecvCount;							//��վ�����ܴ���
	__u32 HostErrorRecvCount;						//��վ���յ���Ч��δ��Ӧ֡����ͳ��
	__u32 PrimaryOverRun_Count;						//��ͨ�������е�OverRun�������ͳ��
	__u32 SecondaryOverRun_Count;					//��ͨ�������е�OverRun�������ͳ��
	__u32 PrimaryParity_Count;						//��ͨ�������е�Parity�������ͳ��
	__u32 SecondaryParity_Count;					//��ͨ�������е�Parity�������ͳ��
	__u32 PrimaryFrameClosed_Count;					//��ͨ�������е�Frame Closed�������ͳ��
	__u32 SecondaryFrameClosed_Count;				//��ͨ�������е�Frame Closed�������ͳ��
	__u16 Send_MaxTimeSpend;						//CPU����һ֡�������ʱ�䳤��
	__u16 Recv_MaxTimeSpend;						//CPU����һ֡�������ʱ�䳤��
};

struct DPU_DYNA_ATTR
{
	__u8 dpuAddr;			//�߼���ַ
	__u8 dpuStatus;			//״̬
	__u8 dpuType;			//����
	__u8 Real_io_num;		//ʵ�����е�IO����
	__u8 vir_io_num;		//ʵ�����е�����IO����
};

/** �������͵��¼���һ��52���ֽ� */
struct IO_DYNA_EVENT {
	__u8 dpuaddr;			/**< DPU��ַ */
	__u8 ioaddr;			/**< io��ַ������ȫװ�ã�ΪFFH */
	__u8 time56[7];			/**< 7�ֽ�ʱ�� */
	__u8 dputype;			/**< DPU���� */
	__u8 eventtype;		/**< �¼����ͣ���Ϊ�澯���������������֣���"�¼����ͱ���" */
	__u8 eventcode;		/**< �¼����룬������ϸ�¼����롱������ */
	__u8 eventdesp[40];		/**< 40���ֽڵ��ı������¼�������������40����ʣ��ռ���0������������EVENT_DESP */
};

/**
* @name ͨ���ṹ��
* @{
*/
//ģ������ʵ�ʵ�ַ���Ը���startAddr+subCHnum *sizeof(Type)�ó���
//��������ʵ�ʵ�ַ���Ը���startAddr|subCHnum�ó�
typedef struct NICSYS_CHANNEL_st
{
	//��̬����
	__u8 dpuAddr;		//DPU��ַ
	__u8 ioAddr;		//IO���߼���ַ
	__u8 status;		//ͨ����״̬
	__u16 offAddr;		//VPT����ʱΪ�ڲ��ڴ��е�ƫ�Ƶ�ַ,IO����ʱΪIO����ͨ����,��0�����
	__u8  subOffAddr;	//�������ֽ���ƫ��ͨ���ţ���0 ��ʼ�� 7��ģ������Ϊ0
	__u8 ioModType;		//IO���Ͱ忨�ľ�������
	__u8  dataType;		//DPUʵ�ʶ������������
	//��̬����

	DBData   rData;		//ʵʱֵ�����а�������������Ϊ���㶨�����������
}nicONetChannel;



/**
* @name IO���ṹ��
* @{
*/
typedef struct NICSYS_IOCARD_st
{
	//��̬����
	//��̬����
	__u8 dpuAddr;			//DPU�߼���ַ
	__u8 ioId;				//IO�Ĳ�ۺ�
	__u8 ioAddr;			//IO���߼���ַ
	__u8 ioStatus;			//IO״̬	
	__bool isExist;			//�жϿ��Ƿ����
	__u8 ioModType;				/**< IOģ�����ͣ��������� AI_8 DI_16�� */
	__u16 ioCardType;			/**< IO���ͣ�VPT IED IOCard���ֽ�->���ֽ� */
	__i32 ioCurNum;				//��ǰIO����ͨ������
	nicONetChannel *channels ;	//����VPT���͵�IO��ͨ������������ֻ������Ϊ������
}nicONetIOCard;


/**
* @name ONetЭ�黺������
* @{
*/
//ȫ��Ψһ�����ݻ���
class ONet_DB
{
public:
	ONet_DB()
	{
		ACE_OS::memset(_staticAttr,0,sizeof(DPU_ATTR) *2);
		//��ʼ��dpu״̬�����еģ�������ڳ������ӵ����߼��ж�
		_staticAttr[0].dpuStatus = 0x0001;
		_staticAttr[1].dpuStatus = 0x0001;
		ACE_OS::memset(_dynaAttr,0,sizeof(DPU_DYNA_ATTR) *2);
		ACE_OS::memset(_status,0,sizeof(DPU_STATUS) *2);
		_IOCardsCount =0;
		ACE_OS::memset(ioCards,0,sizeof(nicONetIOCard) *IOCARD_MAX_SIZE);
		for (__u16 i = 0 ;i < IOCARD_MAX_SIZE;i++)
		{
			ioCards[i].isExist = __false;
		}
	}

	//��̬����
	DPU_ATTR   _staticAttr[2];
	//��̬����
	DPU_DYNA_ATTR _dynaAttr[2];

	//״̬
	DPU_STATUS  _status[2];
	__u16 _IOCardsCount;
	__u8   _netStatus0;//��CP��A��״̬
	__u8   _netStatus1;//��CP��B��״̬
	__u8   _netStatus2;//��CP��A��״̬
	__u8   _netStatus3;//��CP��B��״̬
	ACE_RW_Thread_Mutex _data_mutex;	/**< һ������������һ���������Ķ�д�� */
	nicONetIOCard ioCards[IOCARD_MAX_SIZE];//ȫ������������������ֻ��һ��IO��
};

#include <poppack.h>


#endif