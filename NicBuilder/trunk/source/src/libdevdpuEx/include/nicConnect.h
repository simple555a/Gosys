/**
 * @file
 * @brief ������
 * @author Xiaosong Gao
 * @note ���ӵ�����������ӿ�
 */

#ifndef __NICCONNECT_H__
#define __NICCONNECT_H__

#include "nicFDCS-DPU_type.h"
#include "nicFDCS-DPU_const.h"
#include <vector>
#include <ace/Basic_Types.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Connector.h>
#include <ace/Time_Value.h>
#include <ace/Thread_Mutex.h>
#include <ace/Task.h>

using namespace std;

/** �������� */
#ifdef NICFDCSDPU_EXPORTS
#define NICFDCSDPU_API __declspec(dllexport)
#else
#define NICFDCSDPU_API __declspec(dllimport)
#endif

/** @brief ��ʼ������ */
NICFDCSDPU_API void init_service();
/** @brief ��ñ�ǩʵʱֵ */
NICFDCSDPU_API long get_value(IO_T &);
/** @brief ���ñ�ǩʵʱֵ(���ݿ�) */
NICFDCSDPU_API long set_value_library(IO_T &);
/** @brief ���ñ�ǩʵʱֵ(IO��) */
NICFDCSDPU_API long set_value_iocard(IO_T &);

/**
 * @class Connect
 * @brief ����
 * @author Xiaosong Gao
 * @note ��λ�����������ĵ��������࣬�ṩ����/���յȽӿ�
 */
class NICFDCSDPU_API Connect : public ACE_Task<ACE_MT_SYNCH>
{
public:
	/**
	 * @name ����
	 * @{
	 */
	Connect();	/**< @brief ���� */
	~Connect();	/**< @brief ���� */
	/** @} */

	/**
	 * @name ����
	 * @{
	 */
	long connect(char *, char *, bool thread = true);		/**< @brief ���� */
	long disconnect();										/**< @brief �Ͽ����� */
	long reconnect(bool thread = true);						/**< @brief ���� */
	long change_connect(bool thread = true);				/**< @brief �����л� */
	long connect_to_dpu(long priority = 0, bool connect_only = false);		/**< @brief ���ӵ������� */
	/** @} */

	/**
	 * @name ����
	 * @{
	 */
	void generate_packet(FD_PACKET &, long, char *, long &);							/**< @brief �������ݰ� */
	long send(char * buf, long len);													/**< @brief ����һ����Ϣ */
	long send_start_link_request();														/**< @brief START������·���� */
	long send_start_t1_request(ACE_UINT8);												/**< @brief START����T1���� */
	long send_stop_link_request();														/**< @brief STOP������·���� */
	long send_call_io_data(char, long);													/**< @brief IO�����ٻ�=11H */
	long send_control_ao(ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8 *, ACE_UINT8);		/**< @brief ���п���AO=33H */
	long send_control_do(ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8);					/**< @brief ���п���DO=34H */
	long send_io_transparent_data_down(ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8, ACE_UINT8 *);		/**< @brief IO͸����������=52H */
	long send_control_vpt(ACE_UINT8 ioaddr, IO_T & iot);								/**< @brief ����VPT=3AH */
	/** @} */

	/**
	 * @name ����
	 * @{
	 */
	void debug_all_data();								/**< @brief ���all_data_ */
	void debug_packet_content(char *, int);				/**< @brief ��������� */
	long recv();										/**< @brief ����һ����Ϣ */
	long recv_start_link_ack(FD_PACKET &);				/**< @brief START��·����ȷ�� */
	long recv_stop_link_ack(FD_PACKET &);				/**< @brief STOP��·ֹͣȷ�� */
	long recv_start_t1_ack(FD_PACKET &);				/**< @brief T1��·����ȷ�� */
	long recv_answer_io_data(FD_PACKET &);				/**< @brief IO����Ӧ��=21H */
	long recv_operation_failed_message(FD_PACKET &);	/**< @brief ������=15H */
	/** @} */

	/**
	 * @name ����
	 * @{
	 */
	int open(void *);		/**< @brief �����߳� */
	int svc();				/**< @brief �߳����� */
	/** @} */

	/**
	 * @name ����
	 * @{
	 */
	long setDestAddr(ACE_UINT8 addr);	/**< @brief ����Ŀ���߼���ַ */
	inline bool isEnabled() const;		/**< @brief �鿴����/���� */
	inline bool isValid() const;		/**< @brief �鿴�����Ƿ���Ч */
	void setEnabled(bool);				/**< @brief ������Ч�� */
	/** @} */

	/**
	 * @name ������
	 * @{ 
	 */
	inline int acquire();		/**< @brief ������绺�������� */
	inline int release();		/**< @brief �ͷ����绺�������� */
	/** @} */

	static long setSourceAddr(ACE_UINT8 addr);	/**< @brief ����Դ�߼���ַ */
	static void setTimeout(long);				/**< @brief ���ý��ճ�ʱ */
	static void setT1(ACE_UINT8);				/**< @brief ����T1��ʱ */
	static void setPeriodTime(long);			/**< @brief ���ÿ������������� */

	/**
	 * @name ���Ӳ���
	 * @{
	 */
	ACE_Thread_Mutex net_mutex_;	/**< data_buf_�Ķ�д�� */
	ACE_INET_Addr addr_[2];			/**< ��¼����(0)����(1)����ַ�����ڲ��� */
	ACE_SOCK_Stream peer_;			/**< socket���� */
	ACE_SOCK_Connector connector_;	/**< ������ */
	long current_host_;				/**< ��ǰ���ӵ�������ţ�0Ϊ������1Ϊ���� */
	bool stop_thread_;				/**< �Ƿ�ֹͣ�̣߳�true��ʾ����ֹͣ��false��ʾ�߳̽�һֱ���� */
	bool enabled_;					/**< ��ǰ�����Ƿ���Ч */
	bool linked_;					/**< �Ƿ��Ѿ�������������· */
	static ACE_Time_Value timeout_;	/**< �������ݵĳ�ʱʱ�䣬��λ��ms(����) */
	static ACE_UINT8 t1_;			/**< T1���ӣ���DPU�н���T1Ϊ��ʱʱ�䡣��ΧΪ2-16(��) */
	static long period_time_;		/**< ��ʱ���տ�������Ϣ�����ڣ���λ��ms */
	/** @} */

	/**
	 * @name ���ݲ���
	 * @{
	 */
	static ACE_UINT8 source_addr_;		/**< Э��ͷ�����Դ��ַһ�100-131 */
	ACE_UINT8 dest_addr_;				/**< Э��ͷ�����Ŀ�ĵ�ַһ�1-99 */
	IO_DATA all_data_;					/**< ��ʱ�ӿ��������յ���ȫ�����ٻ�����ֵ */
	TIME_MARK time_mark_;				/**< ��ʱ�ӿ��������յ��ķ���ֵ��ʱ�� */
	char recv_buf_[MAX_SIZE_RECV_BUF];	/**< ���յ����ݻ����� */
	long recv_len_;						/**< recv_buf_�����ݳ��� */
	char data_buf_[MAX_SIZE_RECV_BUF];	/**< all_data_��Ӧ�����ݻ����� */
	long data_len_;						/**< data_buf_�����ݳ��� */
	char * recv_read_ptr_;				/**< ���ڽ��������� */
	/** @} */
};

/** @brief ȫ�ֵ����Ӽ��� */
NICFDCSDPU_API extern Connect conns[MAX_DPU_NUM];

#endif	// __NICCONNECT_H__