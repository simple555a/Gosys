//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update   
//= (21-1)         1.0.0.21      Zander Sun      2013-11-28      ���ӽ���������IO�б�

#ifndef __NICSYS_DEVDPU_ONETRECV_H_
#define __NICSYS_DEVDPU_ONETRECV_H_

#include "ONetPctl.h"
//����20HЭ�飺 ϵͳ������DPU���ݣ�����IO��״̬��ȫ������
class CONetPctl_20H:public CONetBase
{
	/**
	* @name ����
	* @{
	*/
public:
	explicit CONetPctl_20H(ONet_DB *db);
	~CONetPctl_20H();



	//��ͨ����ֻ֡�����info_code(����)��ʼ����Ϣ
	__i32 parse_packet(
		__u8 *recvBuff_,
		__u32 recvLen_,
		__u8 curNetNum_);
	
	__i32   do_packet_to_onetdb(
		__u8 * buff_, 
		__u32 len_,
		__bool isHaveTime_,
		__u8 cpuNum_);
	
	//����DPU��IDV��Ϣ,����Э���е�����ֻȡһ������Ϣ������Ҫ��ʱ����չ����
	__i32  do_idvbuff_to_dpustatus(
		__u8 * buff_,
		__bool isDpuMaster_ );

	//����IO��IDV��Ϣ,����Э���е�����ֻȡһ������Ϣ������Ҫ��ʱ����չ����
	__i32  do_idvbuff_to_iostatus(
		__u8 * buff_,
		__u8 ioCardAddr_,
		__u8 ioModType_ );

	__i32 do_databuff_to_iedvalue(
		__u8 ioAddr_,		//IO���߼���ַ
		__u8 *qbuff_,		//������ָ��
		__u8 qLen_,			//����������
		__u8 *valbuff_,		//������ָ��
		__u8 vLen_,			//����������
		__u8 qSize_);		//������ռ���ֽ�

	__i32 do_databuff_to_vptvalue(
		__u8 ioAddr_,		//IO�߼���ַ
		__u8 *&valbuff_,		//VPT������ָ��
		__u16 vCount_ )	;	//VPT��VPT_VALUE�ĸ���


	__i32 do_databuff_to_iovalue(
		__u8 ioAddr_,		//IO���߼���ַ
		__u8 *qbuff_,		//������ָ��
		__u8 qLen_,			//����������
		__u8 *valbuff_,		//������ָ��
		__u8 vLen_,			//����������
		IO_DATA_ATTR *datattr_,	//ֵ��������
		__u8 qSize_);		//������ռ���ֽ�
	__i32   do_iobuffer_to_channelvalue(
		__u8 * & buff_,
		__u8 ioCardAddr_,
		__u8 ioModType_);
private:
	//__u8 _oldFrameNum; //��һ�ζ�֡��֡��
	__u32 time_mark_;
	__u8 *_tempData;//���ն�֡��ʱ����ʱ����
	//__bool exioexist[MAX_IO_CARD];     // ��һʱ�̵�IO����״̬
	vector<__u8>iolist;    // ��ν�������IO�б�  Zander 2013-11-28
};

//����15HЭ�飺 �������ϴ�
class CONetPctl_15H:public CONetBase
{
	/**
	* @name ����
	* @{
	*/
public:
	explicit CONetPctl_15H(ONet_DB *db);
	~CONetPctl_15H();

	//��ͨ����ֻ֡�����info_code(����)��ʼ����Ϣ
	__i32 parse_packet(
		__u8 *recvBuff_,
		__u32 recvLen_,
		__u8 curNetNum_);
};

//����40HЭ�飺 �¼��ϴ�
class CONetPctl_40H:public CONetBase
{
	/**
	* @name ����
	* @{
	*/
public:
	explicit CONetPctl_40H(ONet_DB *db);
	~CONetPctl_40H();

	//��ͨ����ֻ֡�����info_code(����)��ʼ����Ϣ
	__i32 parse_packet(
		__u8 *recvBuff_,
		__u32 recvLen_,
		__u8 curNetNum_);
};

#endif