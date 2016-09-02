//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update                           
//= (20-1)         1.0.0.20      Zander Sun      2013-10-10       CONetBase:增加了上一帧CRC校验码成员变量
//= (23-1)         v1.0.0.23     Zander Sun      2014-10-13       change receive buffer and send buffer pointer to array
//=                                                               use __u8* indicate packet buffer pointer, not char*


#ifndef __NICSYS_DEVDPU_ONETPCTL_H_
#define __NICSYS_DEVDPU_ONETPCTL_H_



#include <map>
#include <vector>
//#include "nicDefs.h"
#include "nicRefACE.h"
#include "ONetConst.h"
#include "ONetType.h"

using namespace std;


using namespace ONet_Const;
//屏蔽类型转换的精度丢失警告
#pragma warning(disable:4244)

/**
* @class CONetBase
* @brief ONet协议基类
* @author CNCS-0136
* @note 
*/

class CONetBase
{
	/**
	* @name 构造
	* @{
	*/
protected:
	//包含构造函数，可以防止基类被实例化

	CONetBase(__u8 downFlag_,__bool CRCCheck_,__bool mutilFlag_,ONet_DB *db);	/**< @brief 构造 */
public:
	//虚拟析构函数，可以保证从基类析构子类
	virtual	~CONetBase();	/**< @brief 析构 */

public:
	//得到数据包头
	__u8 get_pctl_header();
	//设置数据包头
	void set_pctl_header(__u8 sync_header_);
	__u8 get_pctl_dest_addr();
	void set_pctl_dest_addr(__u8 dest_addr_);
	__u8 get_pctl_source_addr();
	void set_pctl_source_addr(__u8 source_addr_);
	__u8 get_protocol_type();
	void set_protocol_type(__u8 protocol_type_);
	__u8 get_control_area_1();
	void set_control_area_1(__u8 control_area_1_);
	__u8 get_control_area_2();
	void set_control_area_2(__u8 control_area_2_);
	__u8 get_control_area_3();
	void set_control_area_3(__u8 control_area_3_);

	//数据区长度低8位码
	__u8 get_length_lowcode();
	void set_length_lowcode(__u8 length_low_);

	//数据区长度高8位码(长度高位码4位+4位版本码)
	__u8 get_length_highcode();
	void set_length_highcode(__u8 length_high_);

	//得到协议类型，即协议信息码
	__u8 get_info_code();
	//设置协议类型，即协议信息码
	void set_info_code(__u8 info_code_);
	__u8 get_description_code();
	void set_description_code(__u8 description_code_);
	//得到数据区十进制的长度(不含校验位)
	__u16 get_data_length();
	//判断是否为多帧
	__bool get_is_multi_packet();
	//设置为多帧
	void set_is_multi_packet();
	//判断是否为多帧最末帧
	__bool get_is_multipacket_end();
	//设置为多帧的尾部 要在setdata之后才能执行有效
	void set_is_multipacket_end();
	//得到协议数据长度(含信息码以后)的十进制长度(不含校验位)
	__u16 get_buffer_length();
	__u32 get_packet_length();
	//char * get_packet_buff();
	//void clear_packet_buff();
	void clear_data();
	//取得数据缓冲区，多帧下的也包括帧号以及时间戳
	__u8 * get_data();
	//上层分配数据内存
	void  set_data(__u8 * buffer_,__u16 datalen);
	//得到十进制的帧号
	__u16 get_frame_num();
	//设置十进制的帧号,要在setdata之后才能执行有效
	void set_frame_num(__u16 frame_num_);
	//得到低位的帧号十六进制码
	__u8 get_frame_num_lowcode();
	//得到高位的帧号十六进制码
	__u8 get_frame_num_highcode();
	__u16 get_crc16();
	void set_crc16(__u16 crc16_);
	void debug_procotol_data();

	__bool get_is_test_frame();
	void set_is_test_frame(__bool islink_);

	//如果是下行，则生成发送包
	virtual __i32 create_packet();
	//形成帧格式，下行协议才用
	virtual __i32 create_frame();
	//解析帧，上行协议才用
	//从socket接收到的数据，按照不同协议进行解析
	virtual __i32 parse_packet(__u8 *recvBuff_,__u32 recvLen,__u8 curNum_);

	// packet buffer that RTDB sends to DPU
	__u8 _packetbuff[MAX_SIZE_BUF]; //下行发送或者上行解析包缓冲区指针  Zander 2014-10-13
	__u32 _packetLen;	//下行发送或者上行解析包长度

	// packet buffer that DPU sends to RTDB
	__u8 _recvPacktBuf[MAX_SIZE_RECV_BUF];        // Zander 2014-10-13

	__u8 _oldFrameNum; //上一次多帧的帧号  //WP

	__u16 _oldCrc; // 上一帧的CRC码, Zander 2013-10-10
protected:
	FD_PACKET_ONet _packData;//协议包
	ONet_DB *_ONetdb;	//上行协议的数据缓冲区db

private:

	__bool _isDown;    //是否为下行命令，=1 是，= 0 是上行

	__bool _isCRCCheck; //是否有CRC校验

	__bool _isMutilPacket;//是否为多帧数据

	__bool _isMutilEnd;//是否是多帧数据的最后一帧

	__bool _isTestFrame;//是否为测试链接帧，此类帧是没有信息码、描述码以及数据的

	
};
typedef CONetBase* CONetPctlBase_ptr;


//协议仓库
class CONetPctlDepository
{

public:
	/**
	* @name 构造
	* @{
	*/
	CONetPctlDepository();
	~CONetPctlDepository();
	/** @} */

	typedef map<__u8 ,CONetPctlBase_ptr> PCTL_LIST;//协议列表

	//初始化已经存在的协议
	void init_pctls(ONet_DB *db);
	CONetBase *get_pctl(__u8 pctlnum_);
	//增加的协议参数要在具体的使用中填充内容
	__i32 add_pctl(__u8 pctlnum_,CONetBase *  pctl_);
	__i32 del_pctl(__u8 pctlnum_);
	__i32 clear_all_pctl();
	//输出协议仓库支持的协议类型
	void dubeg_pctls();
private:
	PCTL_LIST _pctls;
	
};


#endif