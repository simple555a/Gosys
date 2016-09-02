#ifndef Nicsys_NET_H_
#define Nicsys_NET_H_

#include "nicDefs.h"
#include "nicRefACE.h"

//�ಥĿ���ַ�ı�־λ����������32��Ŀ��
typedef enum
{
	Cast_Net = (1<< 0),			//���͸����пͻ��˱�ʶ
	Cast_DB = (1<< 1),			//���͸��ڴ����ݿ�
	Cast_Alarm = (1<< 2),			//���͸�����ģ��
	Cast_IOSS = (1<< 3),			//���͸�IOSSģ��
	Cast_Tran = (1<< 4),			//�෢�͸����ݴ���ģ��
	Cast_Config = (1<<5),		//���͸����ݿ����ģ��
	Cast_Hisd = (1<< 6),			//���͸���ʷ���ݴ洢	
	Cast_Log = (1<< 7),			//���͸�Logģ��
	Cast_SOE = (1<< 8),			//���͸�SOEģ��
	Cast_Event =(1<< 9),		//���͸��¼��洢ģ��
	Cast_AlarmHisd = (1<< 10),	//���͸������洢	
	Cast_Backup = (1<< 11),		//���͸�����ģ��
	Cast_Swtich = (1<< 12),		//���͸����罻����ģ��
}CastTagertFlag;

//��Ŀ���ַ��־λ�������ܽ�������жಥ���ܲ�������Ϣ����
typedef enum
{
	MsgStyle_DB_CastAll = Cast_Net
						|Cast_Alarm
						|Cast_IOSS
						|Cast_Tran
						|Cast_Config,
	MsgStyle_DB_CastLocal = Cast_Alarm
							|Cast_IOSS
							|Cast_Tran
							|Cast_Config,
	MsgStyle_CastNet = Cast_Net,			//�������ڴ����ݿⶼ��������ͻ��˶ಥ���ݵ���Ϣ��
	MsgStyle_DB_CastAlarm = Cast_Alarm,
	MsgStyle_DB_CastIOSS = Cast_IOSS,
	MsgStyle_DB_CastTran = Cast_Tran,
	MsgStyle_DB_CastConfig = Cast_Config,
	MsgStyle_Alarm_CastAll= Cast_Tran
							|Cast_Config
							|Cast_Net,
	MsgStyle_IOSS_CastLocal= Cast_DB
							|Cast_Alarm
							|Cast_Tran
							|Cast_Config,

}MsgStyleFlag;

//��Ϣͷ�ṹ����
//��ͷ��������
//1.�ͻ���Ӧ�ó���--->��������ĳһ�����̣���Ϊ���ذ�����Ϊ�ಥ����
//         Ŀ�������16λΪ������ͨ�ž�����°ಿ��Ϊ����˴�����̾��
//		   Դ�����ϰಿ��Ϊ�ͻ���ͨ�ž������16λΪ�ͻ���Ӧ�ó�����̾��
//2.��������ĳһ������--->�ͻ���Ӧ�ó���:Ϊ���ذ�����Ϊ�ಥ��
//         Դ�����16λΪ������ͨ�ž������16λΪ����˴�����̾��
//		   Ŀ�������ϰಿ��Ϊ�ͻ���ͨ�ž������16λΪ�ͻ���Ӧ�ó�����̾��  
//3.�������˽��̵Ķಥ:Ϊ���ذ���Ϊ�ಥ��
//			Դ�����16λΪ������ͨ�ž�����°ಿ��Ϊ����˴�����̾��
//			Ŀ������Ϊ32λ���̱�ʶSwitchTagertFlag��һ������ռһλ��Ŀ��Ϊ���еĿͻ���ģʽռһλ��
//       XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
//       
typedef struct 
{
	nicUInt16 flag;   							  //��ͷ��ʾ
	nicUInt8  msgPriority;					//���ݰ��Ĵ������ȼ�
	nicUInt32 msgLength;					  //���ݰ��ܳ��ȣ�������β
	nicUInt32 serialNo;						 //���к�
	nicUInt16 functionNo;						  //�������
	nicHandle localHandle;                    //���ؾ��
	nicUInt32 sourceMask;						//���ݰ�ԴĿ������
	nicUInt32 targetMask;						//���ݰ�Ŀ������
	nicUInt8 hasLocalHandle : 1;                  //�Ƿ��б��ؾ��,�϶����У����ֶη��ڰ�ͷ�����ֶ�����
	nicUInt8 hasRemoteHandle : 1;                 //�Ƿ���Զ�̾��
	nicBool  isMultCast : 1;                      //�Ƿ��Ƕಥ
	nicUInt8 hasSequnence : 1;					  //�Ƿ��к�̰���������
	nicUInt8 hasCompressed : 1;                   //�Ƿ�ѹ��
	nicUInt8 hasVerify : 1;                       //�Ƿ�У��
	nicUInt8 waitReturn :1; 					  //�Ƿ�ȴ�����
	nicBool isReturnPack : 1;                   //�Ƿ񷵻ذ�
}msgHeader;

#define get_msg_souceNetid(head) ((head->sourceMask >> 16))
#define get_msg_souceAppid(head) ((head->sourceMask & 0x0000ffff))
#define get_msg_targetNetid(head) ((head->targetMask >> 16))
#define get_msg_targetAppid(head) ((head->targetMask & 0x0000ffff))

#define set_msg_souceNetid(head,id) \
do \
{   \
	head->sourceMask = (head->sourceMask & 0x0000FFFF) |(id << 16);  \
} while (0)

#define set_msg_souceAppid(head,id) \
do \
{   \
	head->sourceMask = (head->sourceMask &0xFFFF0000) |id;   \
} while (0)

#define set_msg_targetNetid(head,id) \
do \
{   \
	head->targetMask = (head->targetMask & 0x0000FFFF) |(id << 16);  \
} while (0);

#define set_msg_targetAppid(head,id)\
do \
{   \
	head->targetMask = (head->targetMask &0xFFFF0000) |id;   \
} while (0)



/// <summary> 
///	IPC��NETͨ�Żص�������Ŷ��壬�˶������IPC��NET����ͨ�ŷ�ʽ��Ҳ�Ƿ���������ͻ��˵Ĺ�ͬ����������
/// ��Connection��صĶ�����ָ�����ĳ�����ӣ�������������ָ����˻��߿ͻ��˵�״̬��һ������˻��߿ͻ��˿����ж��connection
/// </summary> 

#define  callBackFuncsCount						5
#define  idGetMsgBlcokDataCallBack				0		//�����ж�
#define  idGetCSProccessStatusCallBack			1		//���յ�����Ϣ
#define  idGetConnectionStatusCallBack			2		//Client & Server���̵�����״̬
#define  idNewConnectionCallBack				3		//����״̬�ص�
#define  idConnectionInteruptCallBack			4		//�õ�������

/// <summary> 
///		�ӿͻ����յ����ݣ��ص�ת�������ȳ���
/// </summary> 
/// <param name="mBlock">
///		��ת�����ݡ�
/// </param>
/// <param name="timeOut">
///		���ݷ��ͳ�ʱʱ�䣬��ʱ����
/// </param>
/// <param name="CLientHandle">
///		�ͻ������ӵ�����������������ID
/// </param>
/// <returns>
///		�����ء�
/// </returns>

typedef nicVoid (nicAPI *getMsgBlcokDataCallBack) (
	nicUInt16 nNetId,
	ACE_Message_Block * mBlock);

/// <summary> 
///		�����������״̬�ص�֪ͨ
/// </summary> 
/// <param name="status">
///		����������״̬
/// </param>
/// <param name="CLientHandle">
///		�ͻ������ӵ�����������������ID
/// </param>
/// <returns>
///		�����ء�
/// </returns>
typedef nicVoid (nicAPI *getCSProccessStatusCallBack) (
	nicProcessStatus  status);

/// <summary> 
///		���������������״̬�ص�֪ͨ
/// </summary> 
/// <param name="pServer">
///		�������Ӿ��
/// </param>
/// <param name="status">
///		�����Ӧ����������Ŀǰ״̬
/// </param>
/// <param name="CLientHandle">
///		�ͻ������ӵ�����������������ID
/// </param>
/// <returns>
///		�����ء�
/// </returns>
typedef nicVoid (nicAPI *getConnectionStatusCallBack) (
	nicUInt16 ClientHandle,
	nicConnectionStatus  status);

/// <summary> 
///		����������������ӽ����֪ͨ
/// </summary> 
/// <param name="pServer">
///		�������Ӿ��
/// </param>
/// <param name="status">
///		�����Ӧ����������Ŀǰ״̬
/// </param>
/// <param name="CLientHandle">
///		�ͻ������ӵ�����������������ID
/// </param>
/// <returns>
///		�����ء�
/// </returns>
typedef nicVoid (nicAPI *newConnectionCallBack) (
	nicUInt16 ClientHandle,
	nicVoid *  args);

/// <summary> 
///		��������������ӶϿ��ص�֪ͨ
/// </summary> 
/// <param name="status">
///		�����Ӧ����������Ŀǰ״̬
/// </param>
/// <param name="CLientHandle">
///		�ͻ������ӵ�����������������ID
/// </param>
/// <returns>s
///		�����ء�
/// </returns>
typedef nicVoid (nicAPI * connectionInteruptCallBack) (
	nicHandle ClientHandle,
	nicVoid *  args);	

#define MSG_HEADER_SIZE                            28

//����Ĭ�϶˿ں�
const nicUInt32 DEFUALT_NET_PPORT =	8889;
//IPCĬ�϶˿ں�
const nicUInt32 DEFUALT_IPC_PPORT =	9090;
//��Ϣ����β��־
const nicUInt16 BEGFLAG  =	0x5a5a;

const nicUInt16 ENDFLAG	 =	0x6a6a;	

//Ĭ�Ͽ������ӵķ������������
const nicUInt16 DEFAULT_SERVER_COUNT = 16;

//���л����������Ĭ�ϳ���
const nicUInt32 PSMAX_BUFFER_SIZE = 1024*1024;

//��������ʼĬ�ϳ���
const nicUInt32 DEFAULT_BUFFER_SIZE = 8192;

//���к����ֵ
const nicUInt32 nicMAX_SER_NO =  2147483648u;

//������ص����������кŻ���
const nicUInt32 nicCALLBACK_BASE = 2147483548u;

//ϵͳ��ʱʱ������
const nicUInt32 nicMax_TIME_OUT = 10;

//���ö�Message_queue������ͳһ������ʱʱ��
const nicUInt32 nicMQ_OP_TIMEOUT = 2;

//������������
const nicUInt32 nicReconnect_Times = 5;

//x�̳߳�˯ʱ�䣬���ڵ����´���������
const nicInt32 PSPACECONT_SLEEP_TIME = 2;

//Ĭ����Ϣ����С
const size_t DEFAULT_MB_SIZE = 1024;

//Ĭ�ϵ�֧�����������
const nicUInt32 MAX_CONNECT_COUNT = 128;

//��������Ͽ���־����
const nicInt32 NONEEDRECONNECT = -1;

//����������Ҫ������־
const nicInt32 NEEDCONNECT = 1;

//�������粻��Ҫ������־
const nicInt32 NONEEDCONNECT = 0;

//����IP��ַ����󳤶�
const nicInt32 MAX_IP_SIZE = 16;

const nicInt32 MAX_NODE_SIZE = MAX_IP_SIZE;

const nicStr  DEFUALT_NODENAME =("NODE");


#endif/**/
