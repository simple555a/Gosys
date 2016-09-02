
// ***************************************************************
// hisd			�汾:	2.0
// hisdcomm.h		�汾:	1.0
// -------------------------------------------------------------
// ��ʷ�⹫��ͷ���壬����ʷ�����ļ�ģ����� 
// -------------------------------------------------------------
// Copyright (C) 2011 - �����к˶�������ϵͳ���޹�˾
// ***************************************************************
//
// ����:cncs-0136 ��ŷ��
// mail:guooujie@cncs.bj.cn
// 
//
// ***************************************************************
//
// �Ķ���ʷ��¼
//
// ***************************************************************
//==============================================================================
// 
// �������ͺ궨��
//
// �ͻ�Ӧ�ó���Ӧ�����¶������������������hisd�ӿں���
// ������Ա���ֻ������еĺ�������ݽṹ���壬��Ҫɾ�������Լ����������ݽṹ��
// ���Ա��˵Ķ����������������������޸�
// 
//==============================================================================
#ifndef _HISDCOMM_H_
#define _HISDCOMM_H_

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "db_config.h"
#include "db_int.h"

//==============================================================================
// 
// ͨ���������ͽṹ����
// 
//==============================================================================

typedef		void				hisdVoid;		                     
typedef		float				hisdFloat;                         
typedef		double				hisdDouble;                        
typedef		__int8				hisdInt8;                          
typedef		__int16				hisdInt16;                         
typedef		__int32				hisdInt32;                         
typedef		__int64				hisdInt64;                         
typedef		unsigned __int8		hisdUInt8;                         
typedef		unsigned __int16	hisdUInt16;                        
typedef		unsigned __int32	hisdUInt32;                        
typedef		unsigned __int64	hisdUInt64;                        
typedef		hisdUInt8			hisdBool;                          
typedef		hisdUInt8			hisdByte;                          
typedef		char				hisdChar;                          
typedef		hisdChar*			hisdStr;                        
typedef		char*				hisdAStr;                 
typedef		hisdUInt16*			hisdUStr;                 
typedef		hisdUInt32			hisdHandle;                        
typedef		hisdInt32			hisdAPIStatus;		// �������ش�����

#define		hisdTRUE				1					// ��������
#define		hisdFALSE				0					// ��������

/// <summary> 
///	��׼ʱ��
/// </summary> 
typedef struct
{	
	hisdUInt32		Second;							// ��
	hisdUInt16		Millisec;						// ����
} hisdTime;	

/// <summary> 
///	�����ȵ�Ansi�ַ���
/// </summary> 
typedef struct 
{
	hisdUInt32		Length;							// �ַ�������
	hisdAStr			Data;							// �ַ�������
} hisdAnsiString;

/// <summary> 
///	�����ȵ�Unicode�ַ���
/// </summary> 
typedef struct
{
	hisdUInt32		Length;							// �ַ�������
	hisdUStr			Data;							// �ַ�������
} hisdUnicodeString;

/// <summary> 
///	�����ȵĶ����ƴ�
/// </summary> 
typedef struct
{
	hisdUInt32		Length;							// ���������ݿ�ĳ���
	hisdByte*			Data;							// ���������ݿ������
} hisdBlob;

/// <summary> 
///	֧�ֵ�������������
/// </summary> 
typedef enum 
{
	hisdDataType_Empty = 0,							// δ����
		hisdDataType_Bool,								// ������
		hisdDataType_Int8,								// һ�ֽ�����
		hisdDataType_UInt8,								// һ�ֽ��޷�������
		hisdDataType_Int16,								// ���ֽ�����
		hisdDataType_UInt16,								// ���ֽ��޷�������
		hisdDataType_Int32,								// ���ֽ�����
		hisdDataType_UInt32,								// ���ֽ��޷�������
		hisdDataType_Int64,								// ���ֽ�����
		hisdDataType_UInt64,								// ���ֽ��޷�������
		hisdDataType_Float,								// �����ȸ�����
		hisdDataType_Double,								// ˫���ȸ�����
		hisdDataType_Time,								// ʱ��
		hisdDataType_AnsiString,							// ANSI�ַ���
		hisdDataType_UnicodeString,						// Unicode�ַ���
		hisdDataType_Blob,								// ���������ݿ�
		hisdDataType_Max									// �����������?
}hisdDataType;	
/// <summary> 
///	ͨ����������
/// </summary> 
typedef struct 
{
	hisdDataType				DataType;					//�������ͣ�hisdDataType��
	union
	{
		hisdBool			Bool;						// ������
		hisdInt8			Int8;						// һ�ֽ�����
		hisdUInt8			UInt8;						// һ�ֽ��޷�������
		hisdInt16			Int16;						// ���ֽ�����
		hisdUInt16			UInt16;						// ���ֽ��޷�������
		hisdInt32			Int32;						// ���ֽ�����
		hisdUInt32			UInt32;						// ���ֽ��޷�������
		hisdInt64			Int64;						// ���ֽ�����
		hisdUInt64			UInt64;						// ���ֽ��޷�������
		hisdFloat			Float;						// �����ȸ�����
		hisdDouble			Double;						// ˫���ȸ�����
		hisdTime			Time;						// ʱ��
		hisdAnsiString		AnsiString;					// Ansi�ַ���
		hisdUnicodeString	UnicodeString;				// Unicode�ַ���
		hisdBlob			Blob;						// �����ƿ�
	};												// ��ͬ�������͵�ֵ����
} hisdVariant;
	
typedef enum
{
	hisdDAQ_OPC_QUALITY_MASK				= 0x30,	/// Quality BITMASK
		
		// Quality(��4λ�͵�5λ)
		hisdDAQ_OPC_BAD					   	= 0x00,	// ������	
		hisdDAQ_OPC_UNCERTAIN					= 0x10,	// ���ɿ�����
		hisdDAQ_OPC_NA						= 0x20,	// N/A	
		hisdDAQ_OPC_GOOD						= 0x30,	// �õ�����	
		
		// Substatus for BAD Quality(��0λ����3λ)
		hisdDAQ_OPC_CONFIG_ERROR				= 0x01,	// ���ô���
		hisdDAQ_OPC_NOT_CONNECTED				= 0x02,	// û�������豸
		hisdDAQ_OPC_DEVICE_FAILURE			= 0x03,	// �豸ʧ��
		hisdDAQ_OPC_SENSOR_FAILURE			= 0x04,	// ������ʧ��
		hisdDAQ_OPC_LAST_KNOWN				= 0x05,	// ��һ�βɼ���ֵ(ͨѶʧ��)
		hisdDAQ_OPC_COMM_FAILURE				= 0x06,	// ͨѶʧ��(������һ�βɼ�ֵ����)
		hisdDAQ_OPC_OUT_OF_SERVICE			= 0x07,	// �豸ͣ��
		hisdDAQ_OPC_WAITING_FOR_INITIAL_DATA	= 0x08,	// ��δȡ���豸����
		
		// Substatus for UNCERTAIN Quality(��0λ����3λ)
		hisdDAQ_OPC_LAST_USABLE				= 0x11,	// ��һ������ֵ
		hisdDAQ_OPC_SENSOR_CAL				= 0x14,	// ������ֵ����ȷ
		hisdDAQ_OPC_EGU_EXCEEDED				= 0x15,	// ������
		hisdDAQ_OPC_SUB_NORMAL				= 0x16,	// ֵ�Ӷ������Դ�õ�����ȱ���㹻��ĺ�����
		
		// Substatus for GOOD Quality(��0λ����3λ)
		hisdDAQ_OPC_LOCAL_OVERRIDE			= 0x36,	// ֵ������(GOOD)
} hisdQuality; 


//==============================================================================
// 
// ��ʷ�����в�������
// 
//==============================================================================
//ϵͳ����ʱĬ������
#define NICHIS_SYSTEMDEFAULT_RUNMODEL hisdRML_INCRE//Ĭ������ģʽ
#define NICHIS_SYSTEMDEFAULT_SAVEINTERVAL 60//Ĭ�ϱ���ʱ��������Ϊ��λ
#define NICHIS_SYSTEMDEFAULT_BAKPERIOD 120//Ĭ�ϱ������ڣ���Ϊ��λ

//��ѯģʽ����
#define NICHIS_QUERYMODE_PRE 0//û�о�ȷƥ��ʱ��ȡǰһֵ
#define NICHIS_QUERYMODE_NEXT 1//û�о�ȷƥ��ʱ��ȡ��һֵ
#define NICHIS_DEFAULT_QUERYMODE NICHIS_QUERYMODE_PRE//ϵͳĬ�ϲ�ѯģʽ

//�����ռ䶨��
#define NICHIS_WORKSPACE_LOG "hisLog\\" //��־�ļ���
#define NICHIS_WORKSPACE_DATA "hisData\\" //�����ļ���
#define NICHIS_WORKSPACE_SYSTEM "hisSys\\" //ϵͳ�ļ���
#define NICHIS_WORKSPACE_BAK "hisBak\\" //�����ļ���
#define NICHIS_WORKSPACE_SUBDIRLEN 10

//ϵͳ����ģʽ����
#define NICHIS_STARTMODE_NEW 0 //������ʷ��ϵͳ����չ����ռ��µ�ϵͳ�ļ�
#define NICHIS_STARTMODE_CONTINUE 1 //���չ����ռ��µ�ϵͳ���ü�������

#define NICHIS_DATAFILE_PREFIXNAME "nichisfile_"//�����ļ�ǰ׺��
#define NICHIS_INDEXFILE_PREFIXNAME "nichisfile_"//�����ļ�ǰ׺��
#define NICHIS_DATAFILE_SUFFIXNAME "nhd"//�����ļ���׺��
#define NICHIS_INDEXFILE_SUFFIXNAME "nhi"//�����ļ���׺��

#ifdef _DEBUG
//	#define DEFAULT_DATAFILE_PAGESIZE 4096//Ĭ�ϵ�����ҳ��С
//	#define DEFAULT_DATAFILE_PAGENUMBER 15000//�����ļ�������ҳ����
//	#define DEFAULT_INDEXFILE_PAGESIZE 256//Ĭ�ϵ�����ҳ��С
//	#define DEFAULT_INDEXFILE_PAGENUMBER 1000//�����ļ�������ҳ����
//#else
	#define DEFAULT_DATAFILE_PAGESIZE 4096
	#define DEFAULT_DATAFILE_PAGENUMBER 500000
	#define DEFAULT_INDEXFILE_PAGESIZE 256//1:15
	#define DEFAULT_INDEXFILE_PAGENUMBER 50000
#endif // _DEBUG

#define DEFAULT_MPOOL_PAGESIZE DEFAULT_INDEXFILE_PAGESIZE//�ڴ��ҳ���С

//ע�⣺��Ӳ�ѯģʽ����NICHIS_DATASUMMARY_COUNT֮ǰ��ӣ�������ֵ���е�������
#define NICHIS_DATASUMMARY_TOTAL 0//�ܺ�
#define NICHIS_DATASUMMARY_MAXIMUM 1//���ֵ
#define NICHIS_DATASUMMARY_MINIMUM 2//��Сֵ
#define NICHIS_DATASUMMARY_AVERAGE 3//ƽ��ֵ
#define NICHIS_DATASUMMARY_STDEV 4//��׼����
#define NICHIS_DATASUMMARY_RANGE 5//���ֵ����Сֵ�Ĳ�
#define NICHIS_DATASUMMARY_COUNT 6//����

//ϵͳ����ģʽ
typedef enum
{
	hisdRML_INCRE = 0x01,//��������ģʽ
	hisdRML_REPLACE,//ѭ���滻ģʽ
}HisdRunModel;

//ϵͳ�����飺���ȫ�ֱ����������洢�ļ��Ͳ�㣩����ʱ����ַ�仯��������α��޷�Ѱַ������
typedef struct
{
	hisdBool isOpen;       //����
	hisdInt16 tokennum;    //���Ƹ���	

	hisdInt32 writefileID;//д�α�����Ĵ洢�ļ���ID
	hisdInt16 cursorcout;//����writefileID���α�������������α�
} HisdTokens ;

//ϵͳ������
typedef struct
{
	hisdAStr nic_workspace;//��ʷ��ϵͳ�����ռ�·��
	hisdUInt64 nic_initime;//��ʷ�����г�ʼʱ��
	HisdRunModel nic_runmodel;//��ʷ������ģʽ����Ϊ����������ѭ���滻����ģʽ
	hisdUInt64 nic_maxhisfilenum;//ѭ���滻ģ���У���ʷ�����ļ���������
	hisdUInt32 nic_syssaveperiod;//��ʷ������ʱ���洢�ļ��Ĵ��̼��������Ϊ��λ
	hisdUInt64 nic_syslastsavetime;//��ʷ������ʱ�����һ�δ��̵�ʱ��
	hisdUInt32 nic_sysbakperiod;//ϵͳ���в����ļ��������ڣ�����Ϊ��λ���������ݰ����������ļ��ʹ洢�ļ������ļ�
	hisdUInt64 nic_syslastbaktime;//��ʷ������ʱ�����һ�α��ݲ����ļ���ʱ��

	hisdInt64 nic_mpoolsize;//�ڴ�ش�С
	DB_MPOOL_STAT *nic_mpoolstat;//�ڴ��״̬����
	DB_MPOOL_FSTAT **nic_mpoolfilestat;//�ڴ���ļ�״̬����

	hisdUInt32 nic_datafile_pagenumber;//�����ļ�ҳ��
	hisdUInt32 nic_indexfile_pagenumber;//�����ļ�ҳ��

	HisdTokens *nic_psystokens;//ϵͳȫ�ֹ���������������

	hisdBool nic_ishisdataCheck;//�Ƿ����д�����ʷ���ݼ�飬�������������Ƿ�һ�£�������ʱ��д�롣
}HisdSysPara;

//�������Ż�����
typedef struct nicTagRWInfo
{
	hisdInt32 fileID;//�����ļ�ID
	hisdInt32 indexpagenumber;//����ҳ��λ��
	hisdInt16 indexnumber;//�������λ��
	hisdInt16 datanumber;//�������λ��

	hisdBool isdatainsert;//�Ƿ����м����ݲ�������
}TagRWInfo;

//��ʷ���ݸ�ʽ
typedef struct 
{
	hisdTime				Time ;				// ʱ���
	hisdVariant				Value ;				// ֵ
	hisdUInt8				Quality ;			// ������
} HisdData ;

//==============================================================================
// 
// ϵͳ���д������
// 
//==============================================================================

#define NICHIS_SUCCESS 0

//ͨ�ô���
#define NICHIS_ERROR_UNKNOWN NICHIS_SUCCESS-1
#define NICHIS_ERROR_SYSTEMPARA_NULL  NICHIS_SUCCESS-2
#define NICHIS_ERROR_WORKSPACE_NULL  NICHIS_SUCCESS-3
#define NICHIS_ERROR_FILE_NOEXIST NICHIS_SUCCESS-4
#define NICHIS_ERROR_MALLOC_NULL NICHIS_SUCCESS-5
#define NICHIS_ERROR_FUNCTION_PARA NICHIS_SUCCESS-6
#define NICHIS_ERROR_INVALID_DATATYPE NICHIS_SUCCESS-7
#define NICHIS_ERROR_INVALID_SYSSTARTMODE NICHIS_SUCCESS-8
#define NICHIS_ERROR_INVALID_WORKSPACE NICHIS_SUCCESS-9

//��־�ʹ�����ģ���еĴ���
#define NICHIS_ERROR_LOGFILE_OPEN  NICHIS_SUCCESS-200
#define NICHIS_ERROR_LOGFILE_CLOSE  NICHIS_SUCCESS-201
#define NICHIS_ERROR_LOGFILE_RENAME  NICHIS_SUCCESS-202

//�����ļ�����ģ��
#define NICHIS_ERROR_PAGEHANDLE_CREATEENV  NICHIS_SUCCESS-250
#define NICHIS_ERROR_PAGEHANDLE_ENVNULL  NICHIS_SUCCESS-251
#define NICHIS_ERROR_PAGEHANDLE_MPOOLSET  NICHIS_SUCCESS-252
#define NICHIS_ERROR_PAGEHANDLE_DATADIRNULL  NICHIS_SUCCESS-253
#define NICHIS_ERROR_PAGEHANDLE_OPENENV   NICHIS_SUCCESS-254
#define NICHIS_ERROR_PAGEHANDLE_CLOSEENV   NICHIS_SUCCESS-255
#define NICHIS_ERROR_PAGEHANDLE_SYNENV   NICHIS_SUCCESS-256

#define NICHIS_ERROR_PAGEHANDLE_MPFNULL   NICHIS_SUCCESS-257
#define NICHIS_ERROR_PAGEHANDLE_CREATEMPF   NICHIS_SUCCESS-258
#define NICHIS_ERROR_PAGEHANDLE_OPENMPF   NICHIS_SUCCESS-259
#define NICHIS_ERROR_PAGEHANDLE_CLOSEMPF   NICHIS_SUCCESS-260
#define NICHIS_ERROR_PAGEHANDLE_SYNMPF   NICHIS_SUCCESS-261

#define NICHIS_ERROR_PAGEHANDLE_GETPAGE   NICHIS_SUCCESS-262
#define NICHIS_ERROR_PAGEHANDLE_SETPAGE   NICHIS_SUCCESS-263

#define NICHIS_ERROR_PAGEHANDLE_GETMPOOLSTAT   NICHIS_SUCCESS-264

//�洢�ļ�����ģ��
#define NICHIS_ERROR_FILEMGR_MGRNULL   NICHIS_SUCCESS-300
#define NICHIS_ERROR_HISFILE_NULL   NICHIS_SUCCESS-302
#define NICHIS_ERROR_FILEDATAID_OVERFLOW   NICHIS_SUCCESS-303
#define NICHIS_ERROR_OPENFILE_FILEID  NICHIS_SUCCESS-304
#define NICHIS_ERROR_HISFILE_NONEXITE  NICHIS_SUCCESS-305
#define NICHIS_ERROR_HISFILE_OPENED  NICHIS_SUCCESS-306
#define NICHIS_ERROR_HISFILE_FILEID  NICHIS_SUCCESS-307
#define NICHIS_ERROR_FILEMGR_NOFILE   NICHIS_SUCCESS-308
#define NICHIS_ERROR_FILEMGR_OPENED   NICHIS_SUCCESS-309
#define NICHIS_ERROR_HISFILE_FULL   NICHIS_SUCCESS-310
#define NICHIS_ERROR_HEADPAGE_LOADED   NICHIS_SUCCESS-311
#define NICHIS_ERROR_HISFILE_NAME   NICHIS_SUCCESS-312
#define NICHIS_ERROR_HISFILE_PATH   NICHIS_SUCCESS-313
#define NICHIS_ERROR_DATAFILE_UNFULL   NICHIS_SUCCESS-314

//������ģ��
#define NICHIS_ERROR_TAGMGR_MGRNULL   NICHIS_SUCCESS-350
#define NICHIS_ERROR_TAGMGR_TAGTYPE   NICHIS_SUCCESS-351
#define NICHIS_ERROR_TAGMGR_TAGID   NICHIS_SUCCESS-352
#define NICHIS_ERROR_TAGMGR_TAGADDED   NICHIS_SUCCESS-353
#define NICHIS_ERROR_TAGMGR_ADDDELETED   NICHIS_SUCCESS-354
#define NICHIS_ERROR_TAGMGR_NOFILE   NICHIS_SUCCESS-355

//ѹ��ģ��
#define NICHIS_ERROR_COMPRESS_UNPOINTK   NICHIS_SUCCESS-400
#define NICHIS_ERROR_COMPRESS_UNPARALLE  NICHIS_SUCCESS-401
#define NICHIS_ERROR_COMPRESS_PCOMPNULL  NICHIS_SUCCESS-402
#define NICHIS_ERROR_COMPRESS_UNDATATYPE  NICHIS_SUCCESS-403
#define NICHIS_ERROR_COMPRESS_UNCOMPTYPE  NICHIS_SUCCESS-404

/***************************
	ҳ���ݽṹģ��
***************************/
//�ļ�ͷҳ�ṹ
#define NICHIS_ERROR_HEADPAGE_NULL  NICHIS_SUCCESS-450
//������ҳ�ṹ
#define NICHIS_ERROR_FIRSTINDEXPAGE_NULL  NICHIS_SUCCESS-500
#define NICHIS_ERROR_FIRSTINDEX_PUTTED  NICHIS_SUCCESS-501
//����ҳ�ṹ
#define NICHIS_ERROR_INDEXPAGE_NULL  NICHIS_SUCCESS-550
#define NICHIS_ERROR_INDEXPAGE_INDEXNUMBER  NICHIS_SUCCESS-551
#define NICHIS_ERROR_INDEXPAGE_ITEMDATA  NICHIS_SUCCESS-552
#define NICHIS_FIND_INDEXPAGE_TOFRONT  NICHIS_SUCCESS-553
#define NICHIS_FIND_INDEXPAGE_TOBEHIND  NICHIS_SUCCESS-554
//����ҳ�ṹ
#define NICHIS_ERROR_DATAPAGE_NULL  NICHIS_SUCCESS-600
#define NICHIS_ERROR_DATATYPE_DIFF  NICHIS_SUCCESS-601
#define NICHIS_FIND_DATAPAGE_TOFRONT  NICHIS_SUCCESS-602
#define NICHIS_FIND_DATAPAGE_TOBEHIND  NICHIS_SUCCESS-603

//�α�ģ��
#define NICHIS_ERROR_CURSOR_INVALID_TAGID  NICHIS_SUCCESS-650
#define NICHIS_ERROR_CURSOR_INVALID_FILEID  NICHIS_SUCCESS-651
#define NICHIS_ERROR_CURSOR_TAGNODATA  NICHIS_SUCCESS-652
#define NICHIS_ERROR_CURSOR_UNINIT  NICHIS_SUCCESS-653 
#define NICHIS_ERROR_CURSOR_CREATEFAIL  NICHIS_SUCCESS-654
#define NICHIS_ERROR_CURSOR_HISFILECLOSE  NICHIS_SUCCESS-655
#define NICHIS_ERROR_CURSOR_NOINDEXPAGE  NICHIS_SUCCESS-656
#define NICHIS_ERROR_CURSOR_INVALID_INDEXNUM  NICHIS_SUCCESS-657
#define NICHIS_ERROR_CURSOR_INVALID_CURSORMODE  NICHIS_SUCCESS-658
#define NICHIS_ERROR_CURSOR_NONEXTHISFILE  NICHIS_SUCCESS-659
#define NICHIS_ERROR_CURSOR_DEAD  NICHIS_SUCCESS-660
#define NICHIS_ERROR_CURSOR_INVALID_TIME  NICHIS_SUCCESS-661
#define NICHIS_FIND_HISFILE_TOFRONT  NICHIS_SUCCESS-662
#define NICHIS_FIND_HISFILE_TOBEHIND  NICHIS_SUCCESS-663
#define NICHIS_ERROR_CURSOR_INVALID_HISFILE  NICHIS_SUCCESS-664
#define NICHIS_ERROR_CURSOR_FINDDATA  NICHIS_SUCCESS-665
#define NICHIS_ERROR_CURSOR_INVALID_QUERYMODE  NICHIS_SUCCESS-666
#define NICHIS_ERROR_CURSOR_ADDHISFILE  NICHIS_SUCCESS-667

//API�ӿ�
#define NICHIS_ERROR_API_NODATA   NICHIS_SUCCESS-700
#define NICHIS_ERROR_API_RECEIVEDATA_NOTALL   NICHIS_SUCCESS-701
#define NICHIS_ERROR_API_INVALID_SUMMARYMODE   NICHIS_SUCCESS-702
#define NICHIS_ERROR_API_INVALID_HISDATA_DATATYPE   NICHIS_SUCCESS-703
#define NICHIS_ERROR_API_INVALID_HISDATA_DATATIME   NICHIS_SUCCESS-704
#define NICHIS_ERROR_API_FAILURE_BAKPARAFILE   NICHIS_SUCCESS-705

/*
 * ϵͳ������ɽ���
 */

//*******************************************************************************
// ϵͳ���������ͳ������� ��˧����� 2011-6-30
//*******************************************************************************

extern HisdSysPara * g_NicHis_syspara;//ϵͳ����ʱ��ȫ�ֲ�������

//*******************************************************************************
// ϵͳ�������� ��˧����� 2011-6-30
//*******************************************************************************

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * �������ܣ�Ϊ�ļ��������ļ�·������Ҫ���շ��ص�ָ��
 * ����������ļ�����c_filename���ļ����ڵ���Ŀ¼c_foldername
 * �����������
 * ����ֵ�����ļ�����·��
 */
hisdAStr __GetFilepath(const hisdAStr c_foldername, const hisdAStr c_filename);

/***********************************
*         ʱ���������             *
***********************************/

/*
* �������ܣ�hisdTimeʱ���Ƚ�
* ���������ʱ�����1��pfirsttime��ʱ�����2��psecondtime
* �����������
* ����ֵ�����ȽϽ�������1>2����true�����򷵻�false
*/
hisdBool __HisdTimeMoreThan(const hisdTime *pfirsttime, const hisdTime *psecondtime);

/*
* �������ܣ�hisdTimeʱ��С�Ƚ�
* ���������ʱ�����1��pfirsttime��ʱ�����2��psecondtime
* �����������
* ����ֵ�����ȽϽ�������1<2����true�����򷵻�false
*/
hisdBool __HisdTimeLessThan(const hisdTime *pfirsttime, const hisdTime *psecondtime);

/*
* �������ܣ�hisdTimeʱ����ȱȽ�
* ���������ʱ�����1��pfirsttime��ʱ�����2��psecondtime
* �����������
* ����ֵ�����ȽϽ�������1=2����true�����򷵻�false
*/
hisdBool __HisdTimeEqual(const hisdTime *pfirsttime, const hisdTime *psecondtime);

/*
* �������ܣ����hisdTimeʱ��ֵ�Ƿ�Ϊ0
* ���������ʱ�����ptime
* �����������
* ����ֵ���������
*/
hisdBool __IsHisdTimeZero(const hisdTime *ptime);

/***********************************
*      ��ʷ���ݽṹ��������        *
***********************************/

/*
* �������ܣ�����ʱ��ε�����ʱ�䣬ģ������ֵ��������ĳһ��ʱ�̵�ֵ��������ȡǰһʱ��ֵ
* �����������ʼʱ��pstartdata������ʱ��penddata
* �������������ó��Ĳ�ֵ����pdataitem
* ����ֵ����������
*/
hisdInt16 __HisdDataInterpolate(HisdData *pstartdata, HisdData *penddata, HisdData *pdataitem);

/*
 * �������ܣ���ȡhisdVariant���ݽṹ�е�ǰ�������͵Ĵ�С
 * ���������Ŀ�������ָ��pvariant
 * �����������
 * ����ֵ������Ӧ�������͵����ݴ�С
 */
hisdInt16 __SizeofVariant(const hisdVariant *pvariant);

/*
 * �������ܣ����hisdVariant�ṹ����ֵ������
 *           ��pdstΪNULL����pvariantΪĿ�������psrcΪNULL����pvariantΪԴ������pdst��psrc����ΪNULL������
 * ���������Ŀ���ַpdst��Դ��ַpsrc��������hisdVariant�ṹָ��pvariant
 * �����������
 * ����ֵ�����������
 */
hisdInt16 __MemcpyofVariant(hisdVoid *pdst, hisdVoid *psrc, hisdVariant *pvariant);

/*
 * �������ܣ���pvariant����ֵת����double��
 * ���������������hisdVariant�ṹָ��pvariant
 * �����������
 * ����ֵ����ת�����doubleֵ
 */
hisdDouble __VariantToDouble(const hisdVariant *pvariant);


/***********************************
*       �������������             *
***********************************/

/*
* �������ܣ�����һ�������飬����ʼ��
* ���������Ŀ��������ptokens
* �����������
* ����ֵ�����������
*/
hisdInt16 __CreateHisdTokens(HisdTokens **pptokens);

/*
* �������ܣ�����һ��������
* ���������Ŀ��������ptokens
* �����������
* ����ֵ������
*/
hisdVoid __DestroyHisdTokens(HisdTokens **pptokens);

/*
* �������ܣ���һ��������
* ���������Ŀ��������ptokens
* �����������
* ����ֵ������
*/
hisdVoid __OpenHisdTokens(HisdTokens *ptokens);

/*
* �������ܣ��������������������0ʱ���ر�һ�������飬����һֱ�ȴ�
* ���������Ŀ��������ptokens
* �����������
* ����ֵ������
*/
hisdVoid __CloseHisdTokens(HisdTokens *ptokens);

/*
* �������ܣ���ĳһ�����������õ�һ�����ƣ��������鿪�ش��Ҹ�������1ʱ�����õ����ƣ�����һֱ���ڵȴ�״̬
* ���������Ŀ��������ptokens
* �����������
* ����ֵ������
*/
hisdVoid __GetHisdToken(HisdTokens *ptokens);

/*
* �������ܣ��Ż������������
* ���������Ŀ��������ptokens
* �����������
* ����ֵ������
*/
hisdVoid __PutHisdToken(HisdTokens *ptokens);

/*
* �������ܣ������������д�ļ�
* ��������������飬д�ļ���ID
* �����������
* ����ֵ������
*/
hisdVoid __SetHisdTokenFile(HisdTokens *ptokens, const hisdInt32 hisfileID);

/*
* �������ܣ���ȡ�������д�ļ�ID
* ���������������
* �����������
* ����ֵ����д�ļ���ID
*/
const hisdInt32 __GetHisdTokenFile(HisdTokens *ptokens);

/*
* �������ܣ�����������д�ļ�
* ���������������
* �����������
* ����ֵ������
*/
hisdVoid __ClearHisdTokenFile(HisdTokens *ptokens);

/*
* �������ܣ�д�ļ��α��������1
* ���������������
* �����������
* ����ֵ������Ӻ�ķ����α����
*/
hisdInt16 __AddTokenFileCount(HisdTokens *ptokens);

/*
* �������ܣ�д�ļ��α��������1
* ���������������
* �����������
* ����ֵ�������ٺ�ķ����α����
*/
hisdInt16 __MinusTokenFileCount(HisdTokens *ptokens);

#ifdef __cplusplus
};
#endif

#endif //#ifndef _HISDCOMM_H_