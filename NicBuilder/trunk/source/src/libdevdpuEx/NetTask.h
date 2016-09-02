//= UPDATE RECORD
//= Update for    Version     Update by     Date         UpdateUpdate (function names modified)                            
//= (19-2)        1.0.0.19    Zander Sun    2013-9-24    1.新增：init_net_tag()、get_net_tn()、check_net_by_tag()、tn_net[4]
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
		__bool isUseTimer = __true,		//是否采用定时器处理网络连接
		CONetPctlDepository * pctls =0);
	~CNetTask();
	void   create_reactor(ACE_Reactor *rct);
	//初始化网络配置信息:主控制器+备控制器“192.168.1.25;192.168.1.26”
	//隐含的备网信息为“192.168.2.25;192.168.2.26”
	__i32 init_net_config(char * paras_);


	__i32 connect_all();
	__i32 connect_net(int i);
	__i32 disconnect_net(int i);
	__i32 disconnect_all();
	//检查当前连接是否在线，不在线则在四个连接中来回切换,优先级为0.2.1.3
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

	//同步执行协议解析、赋值功能
	__i32 do_recv_packet(__i32 IP_i); // Zander 2014-10-10  add "IP_i"
	__i32 do_send_packet(CONetBase * pBase_, __i32 IP_i); // Zander 2014-10-10  add "IP_i"

	/**
	* @name 任务
	* @{
	*/
	virtual int open(void * args);		/**< @brief  */

	//此处不是ACE_task的close(u_long f)，ACE_task的close是在svc退出后回调调用的
	int close();		/**< @brief  */
	virtual int svc();				/**< @brief 线程运行 */

	/** @} */
	__i32 set_cont_outtime(__u32 tm);//ms单位
	__i32 set_send_outtime(__u32 tm);//ms单位
	__i32 set_recv_outtime(__u32 tm);//ms单位
	__i32 set_onet_client_addr(__u8 addr);
	__i32 set_onet_dpu_addr(__u8 addr);
	__i32 set_refresh_time(__u16 tm);//设置周期上传时间ms

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
	//控制器对时
	__i32  send_settime_link(__i32 IP_i); //Zander 2014-10-10

	// Zander
	__i32 m_link_error_count;
	void clearIOChannelNum();  // 将接收缓存中的IO通道数置0
	// 2013-7-24
	__i32 m_st_num;  // 发送校时信息的时间累积计数
	// 2013-8-29
	__bool m_is_dpu_exist;   // 控制器是否存在，1存在，0不存在
	// Zander, 2013-8-30
    // 是否曾近连接上过，1为曾经连接上过，0为从没有连接上过
	// [0]为小IP控制器1网，[1]为大IP控制器1网，[2]为小IP控制器2网，[3]为大IP控制器2网
	__bool m_is_linked[4];
	__i32 m_ping_count[4];    // 计数，连续多次发现网络断开了，则close与该DPU网口的连接。

	// Zander, 2013-9-24
	TAG_NAME tn_net[4];       // 四个控制器IP的网络状态标签名
	__bool init_net_tag(); //读取标签名
	__bool check_net_by_tag();  // 根据状态标签名的值，查看网络状态


	//2015.05.18  GM
	__i32 CNetTask::handle_15H_event(__u8 *recvBuff_,__u32 recvLen_,__u8 curNetNum_);

	//2015.03.17 GM 属性由private更改为public
	__bool _isLinked; 

	//2015.07.30 GM 测试接受数据时间间隔
	DWORD RevTime1;
	DWORD RevTime2;


	//test
	__int send_num[4];
	__int recv_num[4];
private:
	ACE_Thread_Mutex _vpt_mutex;	/**< vpt写值切换锁 */
	ACE_Thread_Mutex _net_mutex;	/**< 网络切换锁 */
	__u16 _refreshTime;		//订阅刷新的周期ms
	ACE_Time_Value _conTimeout;//与控制器建立连接超时时间
	__u16 _dpuAddr;	//控制器的逻辑编号,主备控制器都是一个逻辑编号
	ACE_INET_Addr _netAddr[4];
	__bool _isOnline[4];	//网线连接是否为连通在线状态
	__bool _isUsed[4];	//网线连接是否使用，这是设置当配置一个IP地址时，防止connect无效的地址
	__i32 _curNum;	//当前连接的编号
	__i32 _oldNum;//WP
	__bool _isEnbled;

	ACE_Time_Value _sendOutTime;				//连接超时时间
	ACE_Time_Value _recvOutTime;				//连接超时时间
	ACE_SOCK_Stream *_curPeer;			/**< 当前socket连接 */
	ACE_SOCK_Stream _peer[4];			/**< socket连接 */
	ACE_SOCK_Connector _connector;	/**< 连接器 */
	__u8  _recvBuffer[MAX_SIZE_BUF];
	__u8 _clientAddr;		//客户的逻辑地址
	CONetPctlDepository * _pctls;
	__bool _isConntChanged;	//连接状态是否改变
	//=start= Zander
	__i32 _conCount[4];	//socket连接计数器
	__bool _islinkStartAck[4];	//开始连接应答
	__bool _islinkStopAck[4];	//开始连接应答
	__bool _islinkT1Ack[4];    //2015.05.06   GM   T1链接确认标志位
	__bool _islinkDataAck[4];    //2015.05.06   GM   数据链接确认标志位
	//=end= 2014-10-10
	__bool _isEnbleTimer;	//是否采用定时器，不采用定时器则会开一个线程处理某些事务
	ACE_Manual_Event _StopHandle;	//线程停止句柄
	ONet_DB *_dbPtr;				//DB缓冲区指针，主要利用Dpustatus	

	// Zander, 2013-9-24
	__bool get_net_tn(PCRTK_TAG pTag);  // 判断标签的地址是否为控制器IP，将网络标签名赋给tn_net[]

	// Zander, 2014-10-10
	ACE_thread_t m_thread_ids[2];       // 子线程id
};
#endif