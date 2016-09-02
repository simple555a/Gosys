//=========================================================================
//  Nicsys1000   �汾:  2.0
//-------------------------------------------------------------------------
//  
//  ʵʱ���ͨ�ô����붨���ļ�
//
//-------------------------------------------------------------------------
//  ��Ȩ (C) 2012 - �����к˿���ϵͳ�������޹�˾(CNCS)
//=========================================================================
// 
//  �Ķ���ʷ��
//  	1. 2012/02/27 : 	CNCS-0136 �����ļ�
//=========================================================================
#ifndef __Nicsys_ERROR_H__
#define __Nicsys_ERROR_H__

#include "nicPrecomp.h"
CDECL_BEGIN

//=====================================================================
// 
// ��������ֵ������Ŷ���
// 
//=====================================================================
/// <summary> 
///	pSpace������룬��Χ��-20000��-10000
/// </summary> 
/// 
typedef enum
{	
	nicERR_TAG_MIN = -15000,						// �ӿڷ�����С�����
	nicPOINT_NOT_NULL,
	nicERR_TAG_START,						
	nicERR_TAG_ID_ALLOT,						// ID����ʧ��
	nicERR_PROP_SETERR,
	nicERR_PROP_GETERR,
	FAIL_DEL_FILE,
	nicERR_PROP_DATATYPE,
} nicTagBaseErrorCodeEnum;

typedef enum
{
	nicERR_MIN = -20000 ,						// �ӿڷ�����С�����

	nicERR_COMMON_BASE = -20000 ,						// ͨ�ô���

	nicERR_DB_CANNOT_START_BASE,                 //ʵʱ���������ģ���޷�����
	nicERR_DB_CANNOT_START_DIGITAL,            //ʵʱ�⿪����ģ���޷�����
	nicERR_DB_CANNOT_START_ANALOG,				//ʵʱ��ģ����ģ���޷�����
	nicERR_DB_CANNOT_START_REAL,					//ʵʱ��ʵʱֵ�����޷�����
	nicERR_DB_CANNOT_START_NOTIFY,				//ʵʱ��֪ͨģ���޷�����

	nicERR_DB_CANNOT_STOP,                            //ʵʱ���޷�����ֹͣ
	nicERR_DB_CANNOT_STOP_BASE,					//ʵʱ���������ģ��ֹͣ����
	nicERR_DB_CANNOT_STOP_DIGITAL,              //ʵʱ�⿪����ģ��ֹͣ����
	nicERR_DB_CANNOT_STOP_ANALOG,              //ʵʱ��ģ����ģ��ֹͣ����
	nicERR_DB_CANNOT_STOP_REAL,                  //ʵʱ��ʵʱֵ����ֹͣ����
	nicERR_DB_CANNOT_STOP_NOTIFY,			   //ʵʱ��֪ͨģ��ֹͣ����

	nicERR_COMMON_FUNCTION_NOT_IMPLEMENT ,				// ����δʵ��
	nicERR_COMMON_PARAMETER_INVALID ,					// ��������
	nicERR_FAIL_IN_BATCH ,						// ���������в��ֲ����������ֳɹ�
	nicERR_COMMON_NO_MEMORY ,							// ���㹻�ڴ�ռ�
	nicERR_COMMON_NO_AUHTORITY,							// �޲���Ȩ��
	nicERR_COMMON_STRING_TOO_LONG,						// �ַ���̫��
	nicERR_COMMON_DATACHANGE_FAIL,                       // ����ת������
	nicERR_COMM_NOMQ,									// ͬ������û�з��������Ϣ����

	nicERR_INTERFACE_BASE = -19900 ,						// �ӿڶ�̬����ش���

	nicERR_INTERFACE_REGISTER_CALLBACK_FAIL,				// �ص�����ע��ʧ��

	nicERR_NET_BASE = -19800 ,					// ͨѶ��ش���

	nicERR_NET_GETQ_TIMEOUT,						// ������Ϣ���г�ʱ
	nicERR_NET_MQ_SHUTDOWN,						// ��Ϣ���йر�
	nicERR_NET_MAPUNBIND,						// ������Ϣ�����ʧ��
	nicERR_NET_STATUSERR,						// ��������״̬����ʧ��,û�ж�Ӧ��״̬��Ϣ����
	nicERR_NET_CONNECT_FAILED,					// ����ʧ��
	nicERR_NET_SEndID_N_ERR,						// ����ʧ��
	nicERR_NET_RECVN_FAILED,                     // ����ʧ��
	nicERR_NET_NEWTHREAD_SEndID_,					// �߳�ʧ��
	nicERR_NET_NO_CONNECTED,						// û������
	nicERR_NET_NOHANDLE_FIND,					// û�б����handle��Ϣ
	nicERR_NET_MAX_CONNECTION,					// ���������
	nicERR_NET_NEEDRESET,						// ���������Ѿ��������ͻ�����Ҫ����
	nicERR_NET_CONNECTING,					//��������������

	nicERR_SCHUDULER_BASE = -19700 ,				// ��������ش���

	nicERR_SCHUDULER_STATUS_ERROR,				// ���������ڴ���״̬
	nicERR_SCHUDULER_STATUS_STOP,				// ����������ֹͣ״̬
	nicERR_SCHUDULER_STATUS_BUSY,				// ������������æ״̬
	nicERR_SCHUDULER_PARA_FILE_FAIL,				// ����ģ���ȡ�����ļ�ʧ��
	nicERR_SCHEDULER_CONNECT_HASHMAP_ERROR, 		// ��ȡhash��ʧ��
	nicERR_SCHEDULER_StartID__ERROR,				// ����ģ������ʧ��
	nicERR_SCHEDULER_CONNECT_NOT_FOUND,  		// ����ģ����δ����������Ϣ
	nicERR_SCHEDULER_API_CONNECT_NOT_FOUND,		// ����ģ����δ����Ӧ������
	nicERR_SCHEDULER_NET_CONNECT_NOT_FOUND,		// ����ģ����δ������������
	nicERR_CONNECTMGR_STOPED,					// ���ӹ������Ѿ�ֹͣ

	nicERR_TAGTYPEMANAGER_BASE = -19600 ,		// ���������ش���

	nicERR_TAGTYPEMANAGER_CANNOT_StartID_,			// ������͹���ģ�鲻������
	nicERR_TAGTYPEMANAGER_MANAGER_FILE_NOT_FOUND,	// ������͹����ļ�������
	nicERR_TAGTYPEMAMAGER_MAMAGER_FILE_FAIL,		// ������͹����ļ�����
	nicERR_TAGTYPEMANAGER_TOTAL_PROP_FILE_NOT_FOUND,	// ����������������ļ�������
	nicERR_TAGTYPEMANAGER_TOTAL_PROP_FILE_FAIL,		// ����������������ļ�����

	nicERR_TAGTYPE_BASE = -19500 ,				// ���������ش���

	nicERR_TAGTYPE_CANNOT_SRART,					// ������Ͷ�̬�ⲻ������
	nicERR_TAGTYPE_HAVE_StartID_ED,					// ������Ͷ�̬���Ѿ�����
	nicERR_TAGTYPE_INFO_FILE_FAIL,				// ���������Ϣ�ļ�����
	nicERR_TAGTYPE_PROP_FILE_FAIL,				// ������������ļ�����

	nicERR_TAG_BASE = -19400 ,					// �����ش���

	nicERR_TAG_HAS_PARENT,						// ???
	nicERR_TAG_WRONG_NAME,						//��������ַ����Ϸ�
	nicERR_TAG_ADD,								// ???
	nicERR_TAG_NAME,								// ???
	nicERR_TAGTREE_DEL,							// ???
	nicERR_TAG_GET_ID,							// ???
	nicERR_TAG_NAMEHASH_UNBIND,					// ???
	nicERR_TAG_GETLONGNAME,						// ???
	nicERR_TAG_NAMEHASH,							// ???
	nicERR_TAG_NAME_TOOLONG,						// ???
	nicERR_TAG_GETSHORTNAME,						// ???
	nicERR_TAGTREE_TRAVERSE,						// ???
	nicERR_TAG_ISANCESTOR,						// ???
	nicERR_TAG_INFOFILE,							// ���ܴ򿪲����Ϣ�ļ�
	nicERR_TAG_DATAFILE,							// ���ܴ򿪲�������ļ�
	nicERR_TAG_DELETE_ROOT,						// ???
	nicERR_TAG_HAVE_DELETED,						// ����Ѿ���ɾ��
	nicERR_TAG_HAVE_CHILD,							//������ӽڵ㣬���ܱ�ɾ��
	nicERR_TAG_PROP_REDUPLICATE,					// �����ظ�
	nicERR_TAG_NAME_REDUPLICATE,					// ��������ظ�
	nicERR_TAG_NAME_NOT_EXIST,					// �����������
	nicERR_TAG_NAMEHASH_FAILURE,					// ����HASH�����
	nicERR_TAG_NOT_NODE,							// ָ���ĵ㲻�ǽ������
	nicERR_TAG_CANNOT_MOVE,						// ��㲻���ƶ�
	nicERR_TAG_CANNOT_COPY,						// ��㲻�ܸ���
	nicERR_TAG_NOT_EXIST,						// ��㲻����
	nicERR_TAG_PROP_NOT_EXIST,					// ���Բ�����
	nicERR_TAG_PROP_NOT_CHANGED,					// ���ֵδ����
	nicERR_TAG_PROP_FILTER,						// ���ֵ������
	nicERR_TAG_CANNOT_GETPROP,					// ���ܶ�����
	nicERR_TAG_CANNOT_SETPROP,					// ����д����		
	nicERR_TAG_LACK_PROP,						// ���Ӳ��ʱȱ�ٱ�Ҫ����

	nicERR_DATATYPE,								// �������ʹ��� ???
	nicERR_DATATYPE_NOT_MATCH,
	nicERR_TAG_NOT_SAVE_HIS,

	nicERR_TAGNOTIFY_BASE = -19300,

	nicERR_TAGNOTIFY_SUBCRIBE_FAILED,			// ע��ʧ��
	nicERR_TAGNOTIFY_CANNOT_StartID_,				// ��㶩�ķ���ģ�鲻������
	nicERR_TAGNOTIFY_SUBSCRIBE_MAP_ERROR,		// ��㶩�ķ���ģ��Ķ��ı����
	nicERR_TAGNOTIFY_PBULISH_MAP_ERROR,			// ��㶩�ķ���ģ��ķ����߱����
	nicERR_TAGNOTIFY_TAG_NOT_SUBSCRIBED,			// ���δ������
	nicERR_TAGNOTIFY_CONNECT_NOT_FOUND,			// ���Ӳ�����

	nicERR_REAL_BASE = -19200 ,					// ʵʱ��ش���

	nicERR_REAL_CANNOT_READ,						// ���ܶ�ʵʱֵ
	nicERR_REAL_CANNOT_WRITE,					// ����дʵʱֵ
	nicERR_REAL_DATA_FILE_FAIL,					// ʵʱ���ݱ����ļ�����

	nicERR_REALNOTIFY_BASE = -19100 ,			// ʵʱ��ش���

	nicERR_REALNOTIFY_CANNOT_StartID_,				// ʵʱ���ķ���ģ�鲻������
	nicERR_REALNOTIFY_SUBSCRIBE_MAP_ERROR,		// ʵʱ���ķ���ģ��Ķ��ı����
	nicERR_REALNOTIFY_PUBLISH_MAP_ERROR,			// ʵʱ���ķ���ģ��ķ����߱����
	nicERR_REALNOTIFY_TAG_NOTSUBSCRIBED,			// ʵʱδ������
	nicERR_REALNOTIFY_CONNECT_NOT_FOUND,			// ���Ӳ�����

	nicERR_AUTHORITY_BASE = -19000,				// Ȩ����ش���

	nicERR_AUTHORITY_SECURITY_AREA_NOT_MATCH,	// ��ȫ����ƥ��
	nicERR_AUTHORITY_WRITE_READONLY_PROPERTY,	// дֻ������
	nicERR_TAG_BEYOND_MAX,						// �ܵ���������Ȩ
	nicERR_LICENSE_UPDATA_ERROR,					// ����License��Ϣʧ�ܣ�ֹͣ���У����Ա�����
	nicERR_LICENSE_TYPE_NO,						// û���κ����֤��Serverֹͣ���С������Ա�����
	nicERR_LICENSE_DEMO_RUN_TIMEOUT,				// ��ʾ��Ϳ������ð����г�ʱ��Serverֹͣ���С������Ա�����
	nicERR_LICENSE_NOT_PSPACE5,					// û�л�ȡpSpace 5.x�汾��License��Serverֹͣ���С������Ա�����
	nicERR_LICENSE_NOT_SIMPLIFIED_CHINESE,		// û�л�ȡ�������İ汾��License��Serverֹͣ���С������Ա�����
	nicERR_LICENSE_SITRAP_StartID_,					// SITrap�����������Ա�����
	nicERR_LICENSE_CONNECT_BEYOND_USERAPP_MAX,	//�û��Ա�д��Ӧ�ó���ͻ�����������������ֵ�������ٽ������ӡ������Ա�����
	nicERR_LICENSE_CONNECT_BEYOND_VIEW_MAX,		//View�ͻ�������������������ֵ�������ٽ������ӡ������Ա�����
	nicERR_LICENSE_CONNECT_BEYOND_NETVIEW_MAX,	//NetView�ͻ�������������������ֵ�������ٽ������ӡ������Ա�����
	nicERR_LICENSE_CONNECT_BEYOND_IO_MAX,		//NetView�ͻ�������������������ֵ�������ٽ������ӡ������Ա�����
	nicERR_AUTHORITY_StartID__LICENSE_FAIL,			//����Licenseʧ�ܣ����Ա�����
	nicERR_LICENSE_EVALUATION_TIMEOUT,			//���������г�ʱ�����Ա�����
	nicERR_LICENSE_NO_EX_LICENSE,				//��չ���û��License��Ȩ�����Ա�����

	nicERR_HIS_BASE = -18900 ,					// ��ʷ����ʷ�ļ���ش���

	nicERR_HIS_NO_DISK_SPACE,					// ���㹻���̿ռ�
	nicERR_HIS_CACHE_OPENED,					    // �������Ѿ���
	nicERR_HIS_CACHE_CLOSED,						// ������û�д�
	// ��ʷ�����ļ�ģ������
	nicERR_HIS_NOT_INI_PATH,						// û�г�ʼ��·��
	nicERR_HIS_MANAGER_FILE_OPENED,				// ��ʷ�����ļ��Ѿ���
	nicERR_HIS_MANAGER_FILE_CLOSED,				// ��ʷ�����ļ�û�д�
	nicERR_HIS_DATA_FILE_IN_LIST,				// �ļ�������ʷ�����ļ��б����ܱ�ע��
	nicERR_HIS_MANAGER_FILE_VERSION,				// �����ļ��ļ��汾��Ч
	nicERR_HIS_MANAGER_FILE_LABEL,				// �����ļ��ļ���ʶ��Ч
	nicERR_HIS_MANAGER_FILE_INVALID,				// �����ļ��ļ���ʽ����
	nicERR_HIS_UNREGIST_CURRENT_FILE,			// ����ע����ǰ�ļ�
	nicERR_HIS_BACKUP_CURRENT_FILE,				// ���ܱ��ݵ�ǰ�ļ�
	nicERR_HIS_DATA_FILE_NO_DATA,				// �����ļ������ݣ����ܱ���
	nicERR_HIS_DATA_FILE_TIME_BEYOND,			// �ָ������ļ�ʱ���ļ�ʱ��ȵ�ǰʱ�仹�󣬳���
	nicERR_HIS_DATA_FILE_TIME_OVERLAP,			// �ָ������ļ�ʱ���ļ�ʱ���������ļ��б��ص�
	nicERR_HIS_DATA_FILE_COUNT_LESS,				// ���滻ģʽ�£���ʷ�����ļ�����������
	nicERR_HIS_CAN_NOT_REGIST,					// ����ע��
	// ��ʷ�����ļ�ģ������
	nicERR_HIS_DATA_FILE_VERSION,				// �����ļ��ļ��汾��Ч
	nicERR_HIS_DATA_FILE_LABEL,					// �����ļ��ļ���ʶ��Ч
	nicERR_HIS_CRC,					 		    // У�����
	nicERR_HIS_PAGE_FULL,				 		// ҳ����������
	nicERR_HIS_DATA_PAGE_BEYOND_MIN,				// ����ʱ��ʱ�������ҳ����Сʱ�仹С
	nicERR_HIS_EndID__OF_INDEX_PAGE,					
	nicERR_HIS_FILE_NOT_IN_LIST,					// �ļ�ID������ʷ�����ļ��������ļ��б����Ҳ���
	nicERR_HIS_SOME_DATA_NOT_SAVED,				// ����һ����������Ϊû�пռ䣬���ܲ���
	nicERR_HIS_NEED_PACK_DATA,					// ����һ���ֿռ䣬��Ҫѹ����ʷ����
	// �α�ģ������
	nicERR_HIS_FIND_ERROR,						// ��ѯ�����쳣���
	nicERR_HIS_BEYOND_MIN,					 	// �ȵ�һ�����ݵ�ʱ�仹С
	nicERR_HIS_BEYOND_MAX, 			 			// �����һ�����ݵ�ʱ�仹��
	nicERR_HIS_NO_EQUAL,							// ����ȷƥ��
	nicERR_HIS_PAGE_EMPTY,						// ҳ��
	nicERR_HIS_DATA_FILE_NOT_EXIST,				// ����ѯ������λ�����Ƴ�ϵͳ���ļ���
	nicERR_HIS_NO_DATA_FILE,						// û����ʷ�����ļ�
	nicERR_HIS_BEYOND_COMPRESS,					// ʱ���ѹ���ṹ������ʱ�仹Ҫ��
	nicERR_HIS_EQUAL_COMPRESS,					// ������ѹ���ṹ������ʱ����ͬ
	nicERR_HIS_PAGE_NOT_OPEN,					// ҳδ��
	nicERR_HIS_NO_DATA,							// ������ļ����޶�Ӧ������
	nicERR_HIS_NO_DATA_IN_INDEX_PAGE,			// ����ҳ������
	nicERR_HIS_NO_DATA_IN_DATA_PAGE,				// ����ҳ������
	// ��ģ������
	nicERR_HIS_NEED_ADJUST_EndID_,					// ��Ҫ�������һ������
	nicERR_HIS_NEED_FOUND_COMPRESS,				// ��Ҫ��ѹ���ṹ�в�ѯ����	
	nicERR_HIS_BEYOND_MAX_READ_COUNT,			// ��ȡ���ݵĸ���������������	
	//ɾ�����������
	nicERR_HIS_DELTIME_LESSTHAN_MIN,   			//ɾ��������ָ��ʱ��αȸò����Сʱ�仹С
	nicERR_HIS_DELTIME_MORETHAN_MAX,   			//ɾ��������ָ��ʱ��αȸò�����ʱ�仹��
	nicERR_HIS_BEYOND_NUMBER,					// ͳ�Ƽ������Ӧ����100      

	nicERR_EVENT_BASE = -18800 ,					// �¼���ش���

	nicERR_EVENTNOTIFY_BASE = -18700 ,			// �¼�������ش���

	nicERR_EVENTNOTIFY_CANNOT_StartID_,				// �¼����ķ���ģ�鲻������
	nicERR_EVENTNOTIFY_SUBSCRIBE_MAP_ERROR,		// �¼����ķ���ģ��Ķ��ı����
	nicERR_EVENTNOTIFY_PUBLISH_MAP_ERROR,		// �¼����ķ���ģ��ķ����߱����
	nicERR_EVENTNOTIFY_CONNECT_NOT_FOUND,		// ���Ӳ�����

	nicERR_USER_BASE = -18600 ,					// �û����û�����ش���

	nicERR_USER_GROUPNAME_CONFLICT,              //�û�������ͻ
	nicERR_USER_USERNAME_CONFLICT,               //�û�����ͻ
	nicERR_USER_USER_IN_GROUP,                   //���û���������д��ڵ��û�
	nicERR_USER_USER_NOT_IN_GROUP,               //���û���ɾ�������ڵ��û�
	nicERR_USER_REIGISTE_USERNAME_FAIL,          //��ϵͳע���û�������
	nicERR_USER_REIGISTE_GROUPNAME_FAIL,         //��ϵͳע���û���������
	nicERR_USER_USERGROUP_NOT_EXIST,             //���������ڵ��û���
	nicERR_USER_USER_NOT_EXIST,                  //���������ڵ��û�
	nicERR_USER_USERNAME_OR_PASSWORD_NOTMATCH,   //�û��������벻ƥ��
	nicERR_USER_READ_PSUSERCONF_FAIL,            //�û����û��������ļ�������
	nicERR_USER_WRITE_PSUSERCONF_FAIL,           //�û����û��������ļ�д����
	nicERR_USER_USERGROUP_ONLYREAD,              //�û���ֻ��
	nicERR_USER_USER_ONLYREAD,                   //�û�ֻ��
	nicERR_USER_LOST_VITAL_DATA ,               //ȱʧ�����ֶ�
	nicERR_USER_NAME_LENGTH_WRONG,              //�û������û��������ȴ���
	nicERR_USER_NAME_ILLEGAL,                   //�û������û������Ƿ�
	nicERR_USER_CONF_DESTROY,                   //�û������ļ����ƻ�
	nicERR_USER_CONF_BACKUP_FAIL,                   //�û������ļ�����ʧ��

	nicERR_IOSS_BASE = -18500 ,					// IOSS��ش����
	nicERR_IOSS_OBJECT_NOT_FOUND,
	nicERR_IOSS_DEVICE_NOT_STARTED,
	nicERR_IOSS_DRIVER_NOT_LOADED,

	nicERR_MAX = -10000 , 						// �ӿڷ����������
} nicErrorCodeEnum;

CDECL_END

#endif ///  __Nicsys_ERROR_H__
