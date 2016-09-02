//= UPDATE RECORD
//= Update for    Version     Update by     Date         UpdateUpdate (function names modified)                            
//= (19-2)        1.0.0.19    Zander Sun    2013-9-24    1.������init_net_tag()��get_net_tn()��check_net_by_tag()��tn_net[4]
//= (22-2)       v1.0.0.22    Zander Sun    2013-10-10   modified greatly                                         

#ifndef __NICSYS_DEVDPU_NETTASK_H_
#define __NICSYS_DEVDPU_NETTASK_H_
#include "precomp.h"
#include "ONetPctl.h"
//#include "nicArgs.h"
//
//#include "nicRefACE.h"


class CONetPctlDepository;

class CNetTask: public ACE_Task<ACE_MT_SYNCH>
{
public:
	CNetTask(	ONet_DB *_dbPtr,
		__bool isUseTimer = __true,		//�Ƿ���ö�ʱ��������������
		CONetPctlDepository * pctls =0);
	~CNetTask();
	void   create_reactor(ACE_Reactor *rct);
	//��ʼ������������Ϣ:��������+����������192.168.1.25;192.168.1.26��
	//�����ı�����ϢΪ��192.168.2.25;192.168.2.26��
	__i32 init_net_config(char * paras_);


	__i32 connect_all();
	__i32 connect_net(int i);
	__i32 disconnect_net(int i);
	__i32 disconnect_all();
	//��鵱ǰ�����Ƿ����ߣ������������ĸ������������л�,���ȼ�Ϊ0.2.1.3
	__bool check_connect();
	//
	__bool check_ping_connect();

	__bool  is_enbled();
	__bool	 is_linked();
	/// Called when input events occur (e.g., connection or data).
	virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

	/**
	* Called when timer expires.  @a current_time represents the current
	* time that the Event_Handler was selected for timeout
	* dispatching and @a act is the asynchronous completion token that
	* was passed in when <schedule_timer> was invoked.
	*/
	virtual int handle_timeout (const ACE_Time_Value &current_time,
		const void *act = 0);

	/// Called when an exceptional events occur (e.g., SIGURG).
	virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);


	/// Called when a handle_*() method returns -1 or when the
	/// remove_handler() method is called on an ACE_Reactor.  The
	/// @a close_mask indicates which event has triggered the
	/// handle_close() method callback on a particular @a handle.
	virtual int handle_close (ACE_HANDLE handle,
		ACE_Reactor_Mask close_mask);

	//ͬ��ִ��Э���������ֵ����
	__i32 do_recv_packet(__i32 IP_i); // Zander 2014-10-10  add "IP_i"
	__i32 do_send_packet(CONetBase * pBase_, __i32 IP_i); // Zander 2014-10-10  add "IP_i"

	/**
	* @name ����
	* @{
	*/
	virtual int open(void * args);		/**< @brief  */

	//�˴�����ACE_task��close(u_long f)��ACE_task��close����svc�˳���ص����õ�
	int close();		/**< @brief  */
	virtual int svc();				/**< @brief �߳����� */

	/** @} */
	__i32 set_cont_outtime(__u32 tm);//ms��λ
	__i32 set_send_outtime(__u32 tm);//ms��λ
	__i32 set_recv_outtime(__u32 tm);//ms��λ
	__i32 set_onet_client_addr(__u8 addr);
	__i32 set_onet_dpu_addr(__u8 addr);
	__i32 set_refresh_time(__u16 tm);//���������ϴ�ʱ��ms

	//=start= Zander  add "IP_i"
	__i32  send_t1_link(__u8 nLinkNet, __i32 IP_i);		 //2015.05.06  GM
	__i32  send_start_link(__u8 nLinkNet, __i32 IP_i);  
	__i32  send_stop_link(__u8 nLinkNet, __i32 IP_i);  
	__i32  send_test_link(__u8 nLinkNet,__i32 IP_i);     //2015.05.06  GM
	__i32  send_read_link(__i32 IP_i);
	//=end=  2014-10-10

	__i32  send_set_vpt_value(
		__u8 valTaype_,
		__u8 ioAddr_,
		__u16 ioCh_,
		__u8 subCh_, 
		pmc_value_t setValue_);
	//��������ʱ
	__i32  send_settime_link(__i32 IP_i); //Zander 2014-10-10

	// Zander
	__i32 m_link_error_count;
	void clearIOChannelNum();  // �����ջ����е�IOͨ������0
	// 2013-7-24
	__i32 m_st_num;  // ����Уʱ��Ϣ��ʱ���ۻ�����
	// 2013-8-29
	__bool m_is_dpu_exist;   // �������Ƿ���ڣ�1���ڣ�0������
	// Zander, 2013-8-30
    // �Ƿ����������Ϲ���1Ϊ���������Ϲ���0Ϊ��û�������Ϲ�
	// [0]ΪСIP������1����[1]Ϊ��IP������1����[2]ΪСIP������2����[3]Ϊ��IP������2��
	__bool m_is_linked[4];
	__i32 m_ping_count[4];    // ������������η�������Ͽ��ˣ���close���DPU���ڵ����ӡ�

	// Zander, 2013-9-24
	TAG_NAME tn_net[4];       // �ĸ�������IP������״̬��ǩ��
	__bool init_net_tag(); //��ȡ��ǩ��
	__bool check_net_by_tag();  // ����״̬��ǩ����ֵ���鿴����״̬


	//2015.05.18  GM
	__i32 CNetTask::handle_15H_event(__u8 *recvBuff_,__u32 recvLen_,__u8 curNetNum_);

	//2015.03.17 GM ������private����Ϊpublic
	__bool _isLinked; 

	//2015.07.30 GM ���Խ�������ʱ����
	DWORD RevTime1;
	DWORD RevTime2;


	//test
	__int send_num[4];
	__int recv_num[4];
private:
	ACE_Thread_Mutex _vpt_mutex;	/**< vptдֵ�л��� */
	ACE_Thread_Mutex _net_mutex;	/**< �����л��� */
	__u16 _refreshTime;		//����ˢ�µ�����ms
	ACE_Time_Value _conTimeout;//��������������ӳ�ʱʱ��
	__u16 _dpuAddr;	//���������߼����,��������������һ���߼����
	ACE_INET_Addr _netAddr[4];
	__bool _isOnline[4];	//���������Ƿ�Ϊ��ͨ����״̬
	__bool _isUsed[4];	//���������Ƿ�ʹ�ã��������õ�����һ��IP��ַʱ����ֹconnect��Ч�ĵ�ַ
	__i32 _curNum;	//��ǰ���ӵı��
	__i32 _oldNum;//WP
	__bool _isEnbled;

	ACE_Time_Value _sendOutTime;				//���ӳ�ʱʱ��
	ACE_Time_Value _recvOutTime;				//���ӳ�ʱʱ��
	ACE_SOCK_Stream *_curPeer;			/**< ��ǰsocket���� */
	ACE_SOCK_Stream _peer[4];			/**< socket���� */
	ACE_SOCK_Connector _connector;	/**< ������ */
	__u8  _recvBuffer[MAX_SIZE_BUF];
	__u8 _clientAddr;		//�ͻ����߼���ַ
	CONetPctlDepository * _pctls;
	__bool _isConntChanged;	//����״̬�Ƿ�ı�
	//=start= Zander
	__i32 _conCount[4];	//socket���Ӽ�����
	__bool _islinkStartAck[4];	//��ʼ����Ӧ��
	__bool _islinkStopAck[4];	//��ʼ����Ӧ��
	__bool _islinkT1Ack[4];    //2015.05.06   GM   T1����ȷ�ϱ�־λ
	__bool _islinkDataAck[4];    //2015.05.06   GM   ��������ȷ�ϱ�־λ
	//=end= 2014-10-10
	__bool _isEnbleTimer;	//�Ƿ���ö�ʱ���������ö�ʱ����Ὺһ���̴߳���ĳЩ����
	ACE_Manual_Event _StopHandle;	//�߳�ֹͣ���
	ONet_DB *_dbPtr;				//DB������ָ�룬��Ҫ����Dpustatus	

	// Zander, 2013-9-24
	__bool get_net_tn(PCRTK_TAG pTag);  // �жϱ�ǩ�ĵ�ַ�Ƿ�Ϊ������IP���������ǩ������tn_net[]

	// Zander, 2014-10-10
	ACE_thread_t m_thread_ids[2];       // ���߳�id
};
#endif