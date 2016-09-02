//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update                           
//= (20-1)         1.0.0.20      Zander Sun      2013-10-10       CONetBase:��������һ֡CRCУ�����Ա����
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
//��������ת���ľ��ȶ�ʧ����
#pragma warning(disable:4244)

/**
* @class CONetBase
* @brief ONetЭ�����
* @author CNCS-0136
* @note 
*/

class CONetBase
{
	/**
	* @name ����
	* @{
	*/
protected:
	//�������캯�������Է�ֹ���౻ʵ����

	CONetBase(__u8 downFlag_,__bool CRCCheck_,__bool mutilFlag_,ONet_DB *db);	/**< @brief ���� */
public:
	//�����������������Ա�֤�ӻ�����������
	virtual	~CONetBase();	/**< @brief ���� */

public:
	//�õ����ݰ�ͷ
	__u8 get_pctl_header();
	//�������ݰ�ͷ
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

	//���������ȵ�8λ��
	__u8 get_length_lowcode();
	void set_length_lowcode(__u8 length_low_);

	//���������ȸ�8λ��(���ȸ�λ��4λ+4λ�汾��)
	__u8 get_length_highcode();
	void set_length_highcode(__u8 length_high_);

	//�õ�Э�����ͣ���Э����Ϣ��
	__u8 get_info_code();
	//����Э�����ͣ���Э����Ϣ��
	void set_info_code(__u8 info_code_);
	__u8 get_description_code();
	void set_description_code(__u8 description_code_);
	//�õ�������ʮ���Ƶĳ���(����У��λ)
	__u16 get_data_length();
	//�ж��Ƿ�Ϊ��֡
	__bool get_is_multi_packet();
	//����Ϊ��֡
	void set_is_multi_packet();
	//�ж��Ƿ�Ϊ��֡��ĩ֡
	__bool get_is_multipacket_end();
	//����Ϊ��֡��β�� Ҫ��setdata֮�����ִ����Ч
	void set_is_multipacket_end();
	//�õ�Э�����ݳ���(����Ϣ���Ժ�)��ʮ���Ƴ���(����У��λ)
	__u16 get_buffer_length();
	__u32 get_packet_length();
	//char * get_packet_buff();
	//void clear_packet_buff();
	void clear_data();
	//ȡ�����ݻ���������֡�µ�Ҳ����֡���Լ�ʱ���
	__u8 * get_data();
	//�ϲ���������ڴ�
	void  set_data(__u8 * buffer_,__u16 datalen);
	//�õ�ʮ���Ƶ�֡��
	__u16 get_frame_num();
	//����ʮ���Ƶ�֡��,Ҫ��setdata֮�����ִ����Ч
	void set_frame_num(__u16 frame_num_);
	//�õ���λ��֡��ʮ��������
	__u8 get_frame_num_lowcode();
	//�õ���λ��֡��ʮ��������
	__u8 get_frame_num_highcode();
	__u16 get_crc16();
	void set_crc16(__u16 crc16_);
	void debug_procotol_data();

	__bool get_is_test_frame();
	void set_is_test_frame(__bool islink_);

	//��������У������ɷ��Ͱ�
	virtual __i32 create_packet();
	//�γ�֡��ʽ������Э�����
	virtual __i32 create_frame();
	//����֡������Э�����
	//��socket���յ������ݣ����ղ�ͬЭ����н���
	virtual __i32 parse_packet(__u8 *recvBuff_,__u32 recvLen,__u8 curNum_);

	// packet buffer that RTDB sends to DPU
	__u8 _packetbuff[MAX_SIZE_BUF]; //���з��ͻ������н�����������ָ��  Zander 2014-10-13
	__u32 _packetLen;	//���з��ͻ������н���������

	// packet buffer that DPU sends to RTDB
	__u8 _recvPacktBuf[MAX_SIZE_RECV_BUF];        // Zander 2014-10-13

	__u8 _oldFrameNum; //��һ�ζ�֡��֡��  //WP

	__u16 _oldCrc; // ��һ֡��CRC��, Zander 2013-10-10
protected:
	FD_PACKET_ONet _packData;//Э���
	ONet_DB *_ONetdb;	//����Э������ݻ�����db

private:

	__bool _isDown;    //�Ƿ�Ϊ�������=1 �ǣ�= 0 ������

	__bool _isCRCCheck; //�Ƿ���CRCУ��

	__bool _isMutilPacket;//�Ƿ�Ϊ��֡����

	__bool _isMutilEnd;//�Ƿ��Ƕ�֡���ݵ����һ֡

	__bool _isTestFrame;//�Ƿ�Ϊ��������֡������֡��û����Ϣ�롢�������Լ����ݵ�

	
};
typedef CONetBase* CONetPctlBase_ptr;


//Э��ֿ�
class CONetPctlDepository
{

public:
	/**
	* @name ����
	* @{
	*/
	CONetPctlDepository();
	~CONetPctlDepository();
	/** @} */

	typedef map<__u8 ,CONetPctlBase_ptr> PCTL_LIST;//Э���б�

	//��ʼ���Ѿ����ڵ�Э��
	void init_pctls(ONet_DB *db);
	CONetBase *get_pctl(__u8 pctlnum_);
	//���ӵ�Э�����Ҫ�ھ����ʹ�����������
	__i32 add_pctl(__u8 pctlnum_,CONetBase *  pctl_);
	__i32 del_pctl(__u8 pctlnum_);
	__i32 clear_all_pctl();
	//���Э��ֿ�֧�ֵ�Э������
	void dubeg_pctls();
private:
	PCTL_LIST _pctls;
	
};


#endif