/**
 * @file
 * @brief Э�鳣��
 * @author Xiaosong Gao
 * @note ������FDCS-DPU�У�����Э���ڲ�����
 */

#ifndef __NICFDCS_DPU_CONST_H__
#define __NICFDCS_DPU_CONST_H__

#include <ace/Basic_Types.h>

#define MAX_IO_CARD 256						/**< IO�������� */
#define NO_CRC_SEND							/**< ���͹����в�����CRC��Ϣ */
#define MAX_SIZE_BUF 4096					/**< �������ݰ���󳤶� */
#define MAX_SIZE_RECV_BUF MAX_SIZE_BUF*10	/**< ���ջ�������󳤶� */
#define SERV_PORT 2008						/**< �����DPU�Ķ˿ں� */
#define DEFAULT_TIMEOUT 500*1000			/**< �������ݵ�Ĭ�ϳ�ʱʱ��Ϊ30*1000us����30ms */
#define DEFAULT_T1 3						/**< Ĭ��T1������ʱʱ��Ϊ3s */
#define DEFAULT_PERIOD_TIME 500				/**< Ĭ�϶Կ������趨�Ķ�ʱ����ȫ����ʱ�� */
#define MAX_DPU_NUM 100						/**< DPU�߼���ַ���ޣ�ʵ��DPU�߼���ַΪ1~MAX_DPU_NUM-1 */

#define LOW_8_BIT(x) (char)(x & 0xFF)			/**< ���һ�����ĵ�8bitλ */
#define HIGH_8_BIT(x) (char)((x & 0xFF00) >> 8)	/**< ���һ��16λ���ĸ�8bitλ */

/**
 * @name ��ͷ
 * @{
 */
#define SYNC_HEADER 0x68				/**< ���ݰ�ͷ����һ���ֽڵ����� */
#define PROTOCOL_VERSION 0x00			/**< Ϊ����Э��ļ����ԣ����ȵĸ���λ��ΪЭ��İ汾�� */
#define LENGTH_HIGH_MASK 0x0F			/**< ���ȵĸ���λ������ */
#define NO_CRC_CHECK (ACE_INT16)0x1616	/**< �����ʹ��CRC�룬���������ֽ�Ϊ0x16, 0x16 */
/** @} */

/**
 * @name Э������
 * @{
 */
#define ONET_REAL_TRANSFER_PROTOCOL		0x08		/**< ONETʵʱ����Э�� */
#define ONET_DATA_EXCHANGE_PROTOCOL		0x09		/**< ONET���ݽ���Э��(����) */
#define CNET_REAL_TRANSFER_PROTOCOL		0x0A		/**< CNETʵʱ����Э�� */
#define CNET_TRANSFER_CONTROL_PROTOCOL	0x0B		/**< CNET�������Э��(���⻷����) */
#define HIGH_BYTE_ORDER					0x80		/**< Э���б�ʾ���ֽ��� */
/** @} */

/**
 * @name IO�忨ģ������
 * @{
 */
#define AI_8_1		0x01	/**< 8��ͨ����������Ч�Բ���AI_1�� */
#define AI_4_1		0x02	/**< 4��ͨ����������Ч�Բ���AI_1�� */
#define AI_10_2		0x08	/**< 6��ͨ����4��ģ̬��������Ч�Բ���AI_2�� */
#define LAI_16_1	0x09	/**< 16��ͨ����������Ч�Բ���LIO�� */
#define AI_19_2		0x0A	/**< 8��ͨ����������ʵ�����鲿����3��Ƶ�ʣ�������Ч�Բ���AI_3�� */
#define DI_16_1		0x10	/**< 16��ͨ����������Ч�Բ���DI_1�� */
#define PI_8_1		0x11	/**< 8��ͨ����������Ч�Բ���PI_1�� */
#define PI_12_1		0x12	/**< 2��ͨ����10��ģ̬��������Ч�Բ���PI_1�� */
#define LDI_32_1	0x13	/**< 32��ͨ����������Ч�Բ���LIO�� */
#define LDI_64_1	0x14	/**< 64��ͨ����������Ч�Բ���LIO�� */
#define LPI_8_1		0x15	/**< 8��ͨ����������Ч�Բ���LIO�� */
#define AM_8_1		0x20	/**< 8��ͨ����1~4ͨ��������Ч�Բ���AI_1�͡�5~8ͨ��������Ч�Բ���AO_1�� */
#define AO_8_1		0x40	/**< 8��ͨ����������Ч�Բ���AO_1�� */
#define DO_8_1		0x50	/**< 8��ͨ����������Ч�Բ���DO_1�� */
#define DO_9_1		0x51	/**< 9��ͨ����������Ч�Բ���DO_1�� */
#define LDO_8_1		0x52	/**< 8��ͨ����������Ч�Բ���LIO�� */
#define DO_16_1		0x53	/**< 16��ͨ����������Ч�Բ���DO_1�� */
#define SD_1		0x21	/**< 12��ͨ����1��3ͨ��������Ч�Բ���DO_1,4��9ͨ��������Ч�Բ���DI_1��10��12ͨ��������Ч�Բ���REV_1�� */
#define IEDMODEL	0xFD	/**< IEDģ�� */
#define VPTGROUP	0xFE	/**< VPT����� */
#define RETVPTGROUP	0xFF	/**< VPT����� */
/** @} */

/**
 * @name ��·����֡��ʽ(������1)
 * @{
 */
#define FRAME_FORMAT_MASK		0x03		/**< ����λ������ */
#define GENERAL_DATA_FRAME		0x00		/**< һ�������֡ */
#define T1_TIMEOUT_FRAME		0x01		/**< T1���ӣ���4λΪ��ʱʱ�� */
#define T2_TIMEOUT_FRAME		0x02		/**< T2���ӣ���4λΪ��ʱʱ�� */
#define LINK_FRAME				0x03		/**< ����֡��������·��ֹͣ��· */
#define T1_REQUEST_MASK			0x04		/**< T1��������λ������ */
#define T1_ACK_MASK				0x08		/**< T1����ȷ��λ������ */
#define T2_REQUEST_MASK			0x04		/**< T2��������λ������ */
#define T2_ACK_MASK				0x08		/**< T2����ȷ��λ������ */
#define CHECK_HEARTBEAT_MASK	0xC0		/**< CHECK HEARTBEAT�м���λ������ */
#define LINK_START_REQUEST		0x10		/**< ������·�������� */
#define LINK_START_ACK			0x20		/**< ������·ȷ������ */
#define LINK_STOP_REQUEST		0x04		/**< ֹͣ��·�������� */
#define LINK_STOP_ACK			0x08		/**< ֹͣ��·ȷ������ */
#define LINK_TEST_REQUEST		0x40		/**< ������·�������� */
#define LINK_TEST_ACK			0x80		/**< ������·ȷ������ */
/** @} */

/**
 * @name ��Ϣ���
 * @{
 */
#define CALL_FDCS_OBSERVATION_POINT_DATA				0x11		/**< �ٻ�FDCS������� */
#define RETURN_OPERATION_FAILED_MESSAGE					0x15		/**< ���ز���ʧ�ܱ��� */
#define ANSWER_FDCS_WHOLE_SYSTEM_OBSERVATION_POINT_DATA	0x21		/**< Ӧ��FDCSȫϵͳ������� */
#define DOWN_CONTROL_AO									0x33		/**< ���п���AO */
#define DOWN_CONTROL_DO									0x34		/**< ���п���DO */
#define HMI_IO_TRANSPARENT_DATA_DOWN					0x52		/**< HMI-IO͸���������� */
#define DOWN_CONTROL_VPT								0x3A		/**< ���п���VPT */
#define MULTI_PACKET									0x80		/**< ��֡���ݣ���Ϣ��ı�־ */
/** @} */

/**
 * @name ��Ϣ��/������ָ������
 * @{
 */
#define H11_DATA_CALL					0x03		/**< ��վ�յ�������󣬽����ݷ��͸�HMI��վ��һ�λỰ���� */
#define H11_DATA_SUBSCRIPTION			0x02		/**< ��վ���������͵�ͬʱ��Ϊ���������ʱ�����������趨�Ľ��������������� */
#define H11_CANCAL_DATA_SUBSCRIPTION	0x01		/**< ��վ���������͵�ͬʱ��ȡ�����ݶ��� */
#define H11_TIMER_MASK					0xFC		/**< ��λ�ĺ���λ����·�����ı�־ */
#define H21_TIME_MARK					0x02		/**< ��ʱ�� */
#define H21_TIME_MARK_LENGTH			4			/**< ʱ�곤�ȣ���λ���ֽ� */
#define H21_QUALIFY_DESCRIPTION			0x01		/**< ��Ʒ������ */
#define H52_DATA_DOWN_TO_SLAVE			0x0F		/**< ��������д��IO�忨 */
#define H52_PRIORITY_EMERGENCY			0x01		/**< �������ݣ����ȼ���� */
#define H52_CMD_WRITE					0x51		/**< �������룬д������ */
#define H52_HMI_ADDR1					0x50		/**< Դ��ַ(HMI��IoSmart�ĵ�ַ��Χ(0x50��0x60) */
/** @} */

/**
 * @name ��������
 * @{
 */
#define NIC_SUCCESS					0		/**< �ɹ� */
#define ERROR_UNKNOW_SFDCS			1		/**< ��ͷ����0x68H */
#define ERROR_NOBUILDLINK			2		/**< û�н����������ӣ��ͷ���������������������� */
#define ERROR_SUBSC_INVALID			3		/**< ���붩����ʱ��ʧ�� */
#define ERROR_ALREADYHASTIMER		4		/**< ���� */
#define ERROR_TIMEROVERFLOW			5		/**< ����Ķ�ʱ��С��1 */
#define ERROR_HASNOTIMER			6		/**< Ŀǰ���� */
#define ERROR_ALREADYLINK			7		/**< �Ѿ����������ӣ��������������� */
#define ERROR_INVALIDPACKET			8		/**< ��ͷ����0x68H */
#define ERROR_LENTOOSHORT			9		/**< ��ͷ�г���С�ڵ���0 */
#define ERROR_IODEAD				10		/**< ���ʵ�IO���� */
#define ERROR_IOADDR				11		/**< IO��ַ�� */
#define ERROR_UNKNOWNDATATYPE		12		/**< �������ݵ��������ͣ�DPUû�ж��� */
#define ERROR_MAKEFRAME				13		/**< ���������˴��� */
#define ERROR_NOVALIDIO				14		/**< ����������IO��ַ���Ҳ��� */
#define ERROR_INVALID_TARGET		15		/**< DPU��ַ�����϶��� */
#define ERROR_INVALID_SRC			16		/**< HMI�ڵ��ַ�����϶��� */
#define ERROR_REGISTER_DEV			17		/**< ע��ͨ���豸��ַ���� */
#define ERROR_TARGET_NOTCORRECT		18		/**<  */
#define ERROR_DPU_STATUS_NORUN		19		/**< DPU״̬��������̬ */
#define ERROR_DEVNO_INVALID			20		/**< �ڽڵ����豸�����Ч */
#define ERROR_NODE_OVERLIMIT		21		/**< ����DPU��IO����Ŀ̫�࣬���������� */
#define ERROR_NO_PROTOCOL			22		/**< Э��û�г�ʼ�� */
#define ERROR_NODE_HASNOADDR		23		/**< �����г�������Ч��DPU��IO��ַ */
#define ERROR_NODE_INVALID			24		/**< ������Ч�ĵ�ַ */
#define SYS_MEM_ALLOC_ERR			25		/**< ϵͳ�����ڴ���� */
#define SYS_QERR_QFULL				26		/**<  */
#define ERROR_RIGHT_NOWRITE			27		/**< û��дȨ�� */
#define ERROR_OPEN_FILENAME			28		/**< ���ļ��� */
#define ERROR_NOEVENT_ITEM			29		/**< ���¼����������գ� */
#define ERROR_NOLOG_ITEM			30		/**< ��LOG���������գ� */
#define ERROR_OVERLIMIT				31		/**<  */
#define ERROR_NO_LBDATA				32		/**< û��¼���ļ� */
#define ERROR_SETTING_INVALID		33		/**< ��ֵ����Ч��ָ��ȡNVRAMʧ�ܡ���ֵ����Ч�� */
#define ERROR_SETTING_AREA			34		/**< ��ֵ���Ŵ� */
#define ERROR_SETTING_NUM			35		/**< ��ֵ������ */
#define ERROR_SETTING_OVERFLOW		36		/**< ��ֵԽ�� */
#define ERROR_SETTING_CRC			37		/**< ��ֵcrcУ��� */
#define ERROR_SETTING_TYPE			38		/**< ��ֵ���ʹ� */
#define ERROR_SETTING_STATE			39		/**< ����״̬�� */
#define ERROR_SETTING_WFILE			40		/**< �������ļ�д��ֵ���Ŵ� */
#define ERROR_SEND_FAILED			41		/**< ���ݰ�����ʧ�� */
#define ERROR_RECV_TIMEOUT			42		/**< ���ݰ����ճ�ʱ */
#define ERROR_CONNECT_FAILED		43		/**< ����ʧ�� */
#define ERROR_CREATE_THREAD			44		/**< ������̨�߳�ʧ�� */
#define ERROR_STOP_THREAD			45		/**< ֹͣ�߳�ʧ�� */
#define ERROR_H11_TIMER_OUTRANGE	47		/**< �������ٻ�=10H������ʱ�䳬����Χ */
#define ERROR_CREATE_LINK			48		/**< ������·����ʧ�� */
#define ERROR_IS_ENABLED			49		/**< �豸û�б����� */
#define ERROR_DPU_OUT_RANGE			50		/**< DPU��ų�����Χ */
#define ERROR_IO_OUT_RANGE			51		/**< IO�忨��ַ������Χ */
#define ERROR_CHANNEL_OUT_RANGE		52		/**< IOͨ����ų�����Χ */
/** @} */

/**
 * @name �����������
 * @{
 */
#define BOOL_1_BIT			0x1F		/**< 1Bit��BOOL */
#define FLOAT_4_BYTE		0x24		/**< 4�ֽڸ��� */
#define FLOAT_3_BYTE		0x23		/**< 3�ֽڸ��� */
#define FLOAT_2_BYTE		0x22		/**< 2�ֽڸ��� */
#define UNSIGNED_INT_4_BYTE	0x34		/**< 4�ֽ��޷������� */
#define UNSIGNED_INT_2_BYTE	0x32		/**< 2�ֽ��޷������� */
#define UNSIGNED_INT_1_BYTE	0x31		/**< 1�ֽ��޷������� */
#define INT_4_BYTE			0x44		/**< 4�ֽ����� */
#define INT_2_BYTE			0x42		/**< 2�ֽ����� */
#define INT_1_BYTE			0x41		/**< 1�ֽ����� */
/** @} */

#endif	// __NICFDCS_DPU_CONST_H__