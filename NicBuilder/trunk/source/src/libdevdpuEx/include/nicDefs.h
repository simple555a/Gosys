//=========================================================================
//  Nicsys1000   �汾:  2.0
//-------------------------------------------------------------------------
//  
//  ʵʱ���ͨ�ñ��������ݸ�ʽ�����ļ�
//
//-------------------------------------------------------------------------
//  ��Ȩ (C) 2012 - �����к˿���ϵͳ�������޹�˾(CNCS)
//=========================================================================
// 
//  �Ķ���ʷ��
//  	1. 2012/02/27 : 	CNCS-0136 �����ļ�
//=========================================================================
#ifndef __Nicsys_DEFS_H__
#define __Nicsys_DEFS_H__

#include "nicPrecomp.h"



CDECL_BEGIN
#pragma pack( push ,NicDefs)
#pragma pack( 1 )

	//=====================================================================
	// 
	// �������ͺ궨��
	//
	// �ͻ�Ӧ�ó���Ӧ�������¶������������������
	// 
	//=====================================================================
	typedef		void				nicVoid ;		                     
	typedef		float				nicFloat ;                         
	typedef		double				nicDouble ;                        
#ifndef _Linux_
	typedef		__int8				nicInt8 ;
	typedef		__int16				nicInt16 ;
	typedef		__int32				nicInt32 ;
	typedef		__int64				nicInt64 ;
	typedef		unsigned __int8		nicUInt8 ;
	typedef		unsigned __int16	nicUInt16 ;
	typedef		unsigned __int32	nicUInt32 ;
	typedef		unsigned __int64	nicUInt64 ;
#else
#include <sys/types.h>
	typedef     int8_t              nicInt8;
	typedef     int16_t             nicInt16;
	typedef     int32_t             nicInt32;
	typedef     int64_t             nicInt64;
	typedef     unsigned char       nicUInt8;
	typedef     unsigned short      nicUInt16;
	typedef     unsigned int        nicUInt32;
	typedef     unsigned long       nicUInt64;
#endif                       
	typedef		nicUInt8			nicBool ;                          
	typedef		nicUInt8			nicByte ;                          
	typedef		char				nicChar ;                          
	typedef		char				nicAChar ;                 
	typedef		nicUInt16			nicUChar ;                 
	typedef		nicAChar*			nicAStr ;                 
	typedef		nicUChar*			nicUStr ;                 
	typedef		nicChar*			nicStr ;                        
	typedef		nicUInt16			nicHandle ;                        
	typedef		nicInt32			nicAPIStatus ;// �������ش�����

	/// <summary> 
	///	��������
	/// </summary> 
#define		nicFALSE				0 			// ��������
#define		nicTRUE					1 			// ��������

	//=====================================================================
	// 
	// ͨ���������ͽṹ����
	// 
	//=====================================================================

	/// <summary> 
	///	��׼ʱ��
	/// </summary> 
	typedef struct 
	{
		nicUInt32		Second ;				// ��
		nicUInt16		Millisec ;				// ����
	} nicTime ,*nicTime_Ptr;	

	/// <summary> 
	///	�����ȵ�Ansi�ַ���
	/// </summary> 
	typedef struct 
	{
		nicUInt32		Length ;				// �ַ�������
		nicAStr			Data ;					// �ַ�������
	} nicAnsiString ;

	/// <summary> 
	///	�����ȵ�Unicode�ַ���
	/// </summary> 
	typedef struct
	{
		nicUInt32		Length ;				// �ַ�������
		nicUStr			Data ;					// �ַ�������
	} nicUnicodeString ;

	/// <summary> 
	///	�����ȵ��ַ���
	/// </summary> 
	typedef struct
	{
		nicUInt32		Length ;				// �ַ�������
		nicStr			Data ;					// �ַ�������
	} nicString ;

	/// <summary> 
	///	�����ȵĶ����ƴ�
	/// </summary> 
	typedef struct
	{
		nicUInt32		Length ;				// ���������ݿ�ĳ���
		nicByte*		Data ;					// ���������ݿ������
	} nicBlob ;

	/// <summary> 
	///	֧�ֵ�������������
	/// </summary> 
	typedef enum 
	{
		nicDataType_Empty = 0 ,					// δ����
		nicDataType_Bool ,						// ������
		nicDataType_Int8 ,						// һ�ֽ�����
		nicDataType_UInt8 ,						// һ�ֽ��޷�������
		nicDataType_Int16 ,						// ���ֽ�����
		nicDataType_UInt16 ,						// ���ֽ��޷�������
		nicDataType_Int32 ,						// ���ֽ�����
		nicDataType_UInt32 ,						// ���ֽ��޷�������
		nicDataType_Int64 ,						// ���ֽ�����
		nicDataType_UInt64 ,						// ���ֽ��޷�������
		nicDataType_Float ,						// �����ȸ�����
		nicDataType_Double ,						// ˫���ȸ�����
		nicDataType_Time ,						// ʱ��
		nicDataType_AnsiString ,					// ANSI�ַ���
		nicDataType_UnicodeString ,				// Unicode�ַ���
		nicDataType_String ,						// �ַ���
		nicDataType_Blob ,						// ���������ݿ�
		nicDataType_Max ,						// ����������Ͷ���
	} nicDataTypeEnum ;	

	/// <summary> 
	///	ͨ����������
	/// </summary> 
	typedef struct 
	{
		nicUInt8			DataType ;			//�������ͣ�nicDataTypeEnum��
		union
		{
			nicBool			Bool ;				// ������
			nicInt8			Int8 ;				// һ�ֽ�����
			nicUInt8		UInt8 ;				// һ�ֽ��޷�������
			nicInt16		Int16 ;				// ���ֽ�����
			nicUInt16		UInt16 ;			// ���ֽ��޷�������
			nicInt32		Int32 ;				// ���ֽ�����
			nicUInt32		UInt32 ;			// ���ֽ��޷�������
			nicInt64		Int64 ;				// ���ֽ�����
			nicUInt64		UInt64 ;			// ���ֽ��޷�������
			nicFloat		Float ;				// �����ȸ�����
			nicDouble		Double ;			// ˫���ȸ�����
			nicTime			Time ;				// ʱ��
			nicAnsiString	AnsiString ;		// Ansi�ַ���
			nicUnicodeString	UnicodeString ;		// Unicode�ַ���
			nicString		String ;			// Unicode�ַ���
			nicBlob			Blob ;				// �����ƿ�
		}; 										// ��ͬ�������͵�ֵ����
	} nicVariant, *nicVariant_Ptr;	 
	typedef const nicVariant*  nicVariant_CPtr;

	/// <summary> 
	///	�ֶδ��б�
	/// ע��:�ַ����б���nicstring��Ϊ���С�
	/// </summary> 
	typedef struct
	{
		nicUInt32			Count ;				// ����
		nicStr*				List ;				// �ַ����б�
	} nicStrList ;
	

	/// <summary> 
	///	 ����״̬�б�
	/// </summary> 
	typedef struct
	{
		nicUInt32			Count ;				// ����״̬����
		nicAPIStatus*		List ;				// ����״̬�б�
	} nicAPIStatusList ;

	//=====================================================================
	// 
	// ���ù��ܺ궨��
	// 
	//=====================================================================
	
#define nicSTR(str)		_TEXT(str)			// ���ַ��������Զ�ת��ΪUnicode��Ansi



	//=====================================================================
	// 
	// ��������nicServer_*����غ��������ݽṹ���궨��
	// 
	//=====================================================================


	/// <summary> 
	///	 �������ĵ�ǰ״̬
	/// </summary> 
	typedef enum
	{
		nicServerStatus_Stop = 0 ,				// ��������ֹͣ������δ�˳�
		nicServerStatus_Busy ,					// ��������æ����ʱ������Ӧ�ͻ�������
		nicServerStatus_Run ,					// ��������������
		nicServerStatus_Error ,					// �������й���
		nicServerStatus_NetError,				// ������Ҫ����
		nicServerStatus_Max ,					// ��������״̬
	} nicServerStatusEnum ;



	//=====================================================================
	// 
	// ������ͣ�nicTagType_*����غ��������ݽṹ���궨��
	// 
	//=====================================================================

	/// <summary> 
	///	��������ID����
	/// ע�⣺
	/// 1. ����ϵͳ��һ������ID������ͬ��������У���ͬID�����Ծ�����ͬ�����ԣ�
	/// 2. ������ֻ���Ϳɶ�д���ࣻ
	/// 3. �����ڲ���ڲ���һ����Ҫһ���ض����ֶ�������
	/// </summary> 
	/// 
	typedef enum 
	{
		nicProp_Min = 0,
		nicProp_TagBase_Min = nicProp_Min,
		//��������
		nicProp_Name_Min = nicProp_TagBase_Min,
		nicProp_TagType = nicProp_Name_Min ,	
		nicProp_Name ,		
		nicProp_Description ,
		nicProp_Name_Max ,

		//���ýṹ����
		nicProp_Inter_Min =nicProp_Name_Max,	
		nicProp_Id  =nicProp_Inter_Min,
		nicProp_ParentId ,	
		nicProp_FirstChildId ,
		nicProp_PrevId,		
		nicProp_NextId ,		
		nicProp_LastModifyTime ,
		nicProp_CreateTime ,
		nicProp_ExtendID,
		nicProp_RelateID,
		nicProp_Inter_Max,

		//��������
		nicProp_Proj_Min =nicProp_Inter_Max,
		nicProp_DataType = nicProp_Proj_Min ,	
		nicProp_InitVal,
		nicProp_OperateClass,
		nicProp_EnableWrite,
		nicProp_ProjectID,
		nicProp_ProjectSysName,
		nicProp_ProjectSysSign,
		nicProp_ProjectDevName,
		nicProp_ProjectDevSign,
		nicProp_CPAddr,
		nicProp_PhysicsMoAddr,
		nicProp_PhysicsMoType,
		nicProp_Proj_Max,

		//��ʷ����
		nicProp_His_Min = nicProp_Proj_Max,
		nicProp_His_IsSave =	nicProp_His_Min ,	  //�Ƿ񱣴濪��
		nicProp_His_MaxSaveTime ,		              //��󱣴�ʱ��
		nicProp_His_CompressRate ,		              //ѹ����
		nicProp_His_CompressMode,		              //ѹ��ģʽ
		nicProp_His_Max,		

		//��������
		nicProp_Alarm_Min = nicProp_His_Max,
		nicProp_EnableAlarm = nicProp_Alarm_Min,
		nicProp_AlarmClass,
		nicProp_AlarmClassify,
		nicProp_EnableQualityAlarm,
		nicProp_Alarm_Max,

		//��������
		nicProp_IOSS_Min = nicProp_Alarm_Max,
		nicProp_IODriveDev = nicProp_IOSS_Min,
		nicProp_IODriveAddr,
		nicProp_RefreshPeriod,
		nicProp_IOSS_Max,

		nicProp_TagBase_Max =nicProp_IOSS_Max,
		


		////ʵʱ����
		//nicPropFlag_Pv_Min = nicProp_TagBase_Max,
		//nicProp_PV = nicPropFlag_Pv_Min,
		//nicProp_LastPV,
		//nicProp_PV_Time,
		//nicProp_PV_Quality,
		//nicProp_LastPV_Time,
		//nicProp_LastPV_Quality,
		//nicPropFlag_Pv_Max,
		// ��չ������Сֵ
		nicPropFlag_Extend_Min = nicProp_TagBase_Max,
		nicPropFlag_Extend_Digital_Min = nicPropFlag_Extend_Min,
		nicProp_OffMsg=nicPropFlag_Extend_Digital_Min,
		nicProp_OnMsg,
		nicProp_EnableOffMsg,
		nicProp_EnableOnMsg,
		nicProp_AlarmDelayTime,
		nicPropFlag_Extend_Digital_Max,

		//ģ���
		nicPropFlag_Extend_Analogy_Min = nicPropFlag_Extend_Digital_Max,
		nicProp_MinValue =nicPropFlag_Extend_Analogy_Min,//��Сֵ
		nicProp_MaxValue,					//���ֵ
		nicProp_Unit,							//��λ
		nicProp_EnableLLLL,			///�Ƿ�֧��L4����
		nicProp_EnableLLL,			///�Ƿ�֧��L3����
		nicProp_EnableLL,			///�Ƿ�֧��L2����
		nicProp_EnableLO,		   ///�Ƿ�֧�ֵͱ���
		nicProp_EnableHI,		   ///�Ƿ�֧�ָ߱���
		nicProp_EnableHH,		   ///�Ƿ�֧�ָ߸߱���
		nicProp_EnableHHH,			///�Ƿ�֧�ָ߸߸߱���
		nicProp_EnableHHHH,			///�Ƿ�֧�ָ߸߸߱���
		nicProp_EnableRate,		   ///�仯�ʱ�����ֵ
		nicProp_LLLL,		///�͵͵�
		nicProp_LLL,		///�͵͵�
		nicProp_LL,		   ///�͵���
		nicProp_LO,		   ///����
		nicProp_HI,		   ///����
		nicProp_HH,		   ///H2��
		nicProp_HHH,		///H3
		nicProp_HHHH,		///H4
		nicProp_Rate,		   ///�仯�ʱ�����ֵ
		nicProp_DeadBand,     ///��������
		nicPropFlag_Extend_Analogy_Max,
		nicPropFlag_Extend_Max =nicPropFlag_Extend_Analogy_Max,

		nicPropFlag_Config_Max =nicPropFlag_Extend_Max, //��̬���������


		nicPropFlag_SnapShot_Min = nicPropFlag_Config_Max,
		nicProp_CurValue=nicPropFlag_SnapShot_Min,
		nicProp_SQRT,
		nicProp_TOT10,
		nicProp_AVG10,
		nicProp_MIN10,
		nicProp_MAX10,
		nicProp_TOT30,                       
		nicProp_AVG30,
		nicProp_MIN30,
		nicProp_MAX30,
		nicProp_TOT60,
		nicProp_AVG60,
		nicProp_MIN60,            
		nicProp_MAX60,

		nicPropFlag_SnapShot_Max,

		nicProp_Max = nicPropFlag_SnapShot_Max,

	}nicTagPropEnum;
	/// <summary> 
	///	�����ͳ�������
	/// </summary> 

#define	nicTagType_All	0xFFFF 					// ���е�����


	/// <summary> 
	///	����ID�б�
	/// </summary> 
	typedef struct
	{
		nicUInt16			PropCount ;			// ���Ը���
		nicUInt16*			PropIdList ;		// ����ID�б�
	} nicPropIdList ;

	/// <summary> 
	///	�������Ե�ֵ
	/// </summary> 
	typedef struct
	{
		nicUInt16			PropId ;			// ����ID
		nicVariant			Value ;				// ����ֵ
	} nicPropValue ;

	/// <summary> 
	///	 ����ֵ�б�
	/// </summary> 
	typedef struct
	{
		nicUInt16			PropCount ;			// ���Ը���
		nicPropValue*		PropValueList ;		// ����ֵ�б�
	} nicPropValueList ;

	//=====================================================================
	// 
	// ��㣨nicTag_*����غ��������ݽṹ���궨��
	// 
	//=====================================================================

	/// <summary> 
	///	���ID��������
	/// </summary> 
#define	nicTagId_Unused	0xFFFFFFFF				// ��Ч�Ĳ��ID
#define	nicTagId_Root	0x00000000				// �����ID



	/// <summary> 
	///	��ǩ����״̬��־
	/// </summary> 
	typedef struct
	{
		nicUInt8 Deleted:1;
		nicUInt8 IsNode:1;
		nicUInt8 Changed:1;
		nicUInt8 Unused:5;
	} nicTagStatusFlag;


	/// <summary> 
	/// ��ǩ�����ͣ�������չ
	/// </summary> 
	typedef enum 
	{
		nicTagType_Null = 0,		//������
		nicTagType_Node,		//��ڵ�
		nicTagType_Ana,		//ģ������
		nicTagType_Dig,		//��������
		nicTagType_Compound,	//��ϵ�
		nicTagType_Device,		//�豸��
		nicTagType_Custom,		//�Զ���
	}nicTagTypeEnum;				



	/// <summary> 
	///	�������Ե�����
	/// </summary> 
	typedef struct 
	{
		nicUInt16		 PropCount;
		nicVariant**	 Value;
		nicBool			 HaveBaseProp;		
		nicBool			 HaveHisProp;
		nicBool			 HaveProjProp;
		nicBool			 HaveAlarmProp;
		nicBool			 HaveExtEndID_Prop;		
	}nicTagPropList;

	/// <summary> 
	///	���ID�б�
	/// </summary> 
	typedef struct
	{
		nicUInt32			TagCount ;			// ������
		nicUInt32*			TagIdList ;			// ���ID�б�
	} nicTagIdList ;

	/// <summary> 
	///	ReconID�б�
	/// </summary> 
	typedef struct
	{
		nicUInt32			RecnoCount ;		// ��¼�ŵĸ���
		nicUInt32*			RecnoList ;			// ��¼���б�
	} nicRecnoList ;


	/// <summary> 
	///	nicQueryArea.QueryLevel�е�����ֵ
	/// </summary> 
#define nicQueryLevel_ALL	0xFFFF				// ��ѯ����µ��������ݣ�����nicQueryArea

	/// <summary> 
	///	 ��ѯ��Χ
	/// </summary> 

	typedef struct
	{
		nicUInt32			RootTagId ;			// ��ѯ����ʼ��㣨����������
		nicUInt16			QueryLevel ;		// ��ѯ������ȣ�0�鱾��nicQueryLevel_ALL�����У�1��2��3...��������
		nicBool				QuerySelf ;			// ����ѯ����ȳ���0ʱ���Ƿ����ʼ��㱾��
	} nicQueryArea ;

	/// <summary> 
	///	 ֵ�б�
	/// </summary> 
	typedef struct 
	{
		nicUInt32			ValueCount;			// ֵ����
		nicVariant*			ValueList;			// ֵ�б�
	} nicValueList;



	/// <summary> 
	///	���ı������
	/// </summary> 
	/// 
	typedef enum
	{
		nicTagChangeType_Create = 0 ,			// ��㽨��
		nicTagChangeType_Change ,				// ������ԸĶ�
		nicTagChangeType_Delete ,				// ��㱻ɾ��
		nicTagChangeType_Init ,					// ϵͳ��ʼ�������в�㱻ɾ��
		nicTagChangeType_Max ,					// 
	} nicTagChangeTypeEnum ;

	//=====================================================================
	// 
	// ʵʱ���ݴ���nicReal_*����غ��������ݽṹ���궨��
	// 
	//=====================================================================

	typedef enum
	{
		nicDAQ_OPC_QUALITY_MASK				= 0x30,	/// Quality BITMASK

		// Quality(��4λ�͵�5λ)
		nicDAQ_OPC_BAD					   	= 0x00,	// ������	
		nicDAQ_OPC_UNCERTAIN					= 0x10,	// ���ɿ�����
		nicDAQ_OPC_NA						= 0x20,	// N/A	
		nicDAQ_OPC_GOOD						= 0x30,	// �õ�����	

		// Substatus for BAD Quality(��0λ����3λ)
		nicDAQ_OPC_CONFIG_ERROR				= 0x01,	// ���ô���
		nicDAQ_OPC_NOT_CONNECTED				= 0x02,	// û�������豸
		nicDAQ_OPC_DEVICE_FAILURE			= 0x03,	// �豸ʧ��
		nicDAQ_OPC_SENSOR_FAILURE			= 0x04,	// ������ʧ��
		nicDAQ_OPC_LAST_KNOWN				= 0x05,	// ��һ�βɼ���ֵ(ͨѶʧ��)
		nicDAQ_OPC_COMM_FAILURE				= 0x06,	// ͨѶʧ��(������һ�βɼ�ֵ����)
		nicDAQ_OPC_OUT_OF_SERVICE			= 0x07,	// �豸ͣ��
		nicDAQ_OPC_WAITING_FOR_INITIAL_DATA	= 0x08,	// ��δȡ���豸����

		// Substatus for UNCERTAIN Quality(��0λ����3λ)
		nicDAQ_OPC_LAST_USABLE				= 0x11,	// ��һ������ֵ
		nicDAQ_OPC_SENSOR_CAL				= 0x14,	// ������ֵ����ȷ
		nicDAQ_OPC_EGU_EXCEEDED				= 0x15,	// ������
		nicDAQ_OPC_SUB_NORMAL				= 0x16,	// ֵ�Ӷ������Դ�õ�����ȱ���㹻��ĺ�����

		// Substatus for GOOD Quality(��0λ����3λ)
		nicDAQ_OPC_LOCAL_OVERRIDE			= 0x36,	// ֵ������(GOOD)
	} nicQualityEnum; 

#define IsDataBad( dataQuality )		(((dataQuality) & nicDAQ_OPC_QUALITY_MASK) == nicDAQ_OPC_BAD )
#define IsDataUncertain( dataQuality )	(((dataQuality) & nicDAQ_OPC_QUALITY_MASK) == nicDAQ_OPC_UNCERTAIN )
#define IsDataGood( dataQuality )		(((dataQuality) & nicDAQ_OPC_QUALITY_MASK) == nicDAQ_OPC_GOOD )

	/// <summary> 
	///  ���ݼ�¼����
	/// </summary> 
	typedef struct 
	{
		nicTime				Time ;				// ʱ���
		nicVariant			Value ;				// ֵ
		nicUInt8				Quality ;			// ������
	} nicData ;							


	/// <summary> 
	///	 ���ݼ�¼�б�
	/// </summary> 
	typedef struct
	{
		nicUInt32			DataCount ;			// ���ݼ�¼����
		nicData*				DataList ;			// ���ݼ�¼�б�
	} nicDataList ;

	/// <summary> 
	///	 ���ʵʱֵ
	/// </summary> 

	typedef struct
	{
		nicUInt32			TagId ;				// ���ID
		nicData				RealData ;			// ���ʵʱ����
	} nicRealData ;


	/// <summary> 
	///	 ������ֵ�б�
	/// </summary> 
	typedef struct
	{
		nicUInt32			TagCount ;			// ������
		nicRealData*		TagListRealData ;	// ���ʵʱ�����б�
	} nicRealDataList ;

	//=====================================================================
	// 
	// ʵʱ��ʷ���ݴ���nicHis_*����غ��������ݽṹ���궨��
	// 
	//=====================================================================

	/// <summary> 
	///  �����ʷ���ݶ���
	/// </summary> 
	typedef struct 
	{
		nicUInt32			TagId ; 			// ���ID
		nicUInt32			DataCount ;			// ���ݸ���
		nicData*				DataList ; 			// �����б�
	} nicTagHisData ;
	//ϵͳ����ģʽ����
	#define NICHIS_STARTMODE_NEW 0 //������ʷ��ϵͳ����չ����ռ��µ�ϵͳ�ļ�
	#define NICHIS_STARTMODE_CONTINUE 1 //���չ����ռ��µ�ϵͳ���ü�������
	//ϵͳ����ģʽ
	typedef enum
	{
		nicRML_INCRE = 0x01,//��������ģʽ
		nicRML_REPLACE,//ѭ���滻ģʽ
	}nicHisdRunModel;
	/// <summary> 
	///  ��������ʷ���ݶ���
	/// </summary> 
	typedef struct 
	{
		nicUInt32			TagCount ;			// ������
		nicTagHisData*		TagListHisData ;	// �����ʷ�����б�ÿ����¼����һ��������ʷ�����б�
	} nicTagListHisData ;						

	/// <summary> 
	/// ��ѯ��ʷ����ʱ��ȡ��ʷ���ݵķ�ʽ
	/// </summary> 
	/// 
	typedef enum 
	{
		nicQueryMode_Undefined = 0 ,				// δ����
		nicQueryMode_Interpolated ,				// ��ʱ������ÿ��ʱ��������ȡ���ֲ�ֵ������ֵ
		nicQueryMode_RawByTime ,					// ȡѹ��������ݣ���ʼ�ͽ����㲻���ж��ֲ�ֵ����
		nicQueryMode_RawByNumber ,				// ȡѹ��������ݣ�������ʱ��Σ�ȡ���㹻����������
		nicQueryMode_Stepped ,					// ��������ʽȡ���ݣ���Ҫ���ڲ��ɽ������Բ�ֵ���������͵Ĳ�ֵ����
		nicQueryMode_Statistic ,					// ȡ��ʱ��ֶε�ͳ����
		nicQueryMode_Max ,						// ���ȡ��ʷ���ݷ�ʽ
	} nicQueryModeEnum ;

	/// <summary> 
	/// ���ݲ�ѯʱ�Ĺ��˷���
	/// </summary> 
	/// 
	typedef enum 
	{
		nicFilterMode_Undefined = 0 ,			// δ����
		nicFilterMode_ExactTime ,				// ȡ�������������Ӧ��ʱ���
		nicFilterMode_BeforeTime ,				// ȡ�����������֮ǰ��ʱ���
		nicFilterMode_AfterTime ,				// ȡ�����������֮���ʱ���
		nicFilterMode_BeforeAndAfterTime ,		// ȡ�����������֮ǰ��֮���ʱ���
		nicFilterMode_Max ,						// �����˷���
	} nicFilterModeEnum ;	

	/// <summary> 
	/// ��ѯ��ʷ����ʱ��ȡ��ķ��򣨰�ʱ���С��
	/// </summary> 
	/// 
	typedef enum 
	{
		nicDirection_Forward = 0 ,				// ���ȡ��
		nicDirection_Backward ,					// ��ǰȡ��
		nicDirection_Max ,						// 
	} nicDirectionEnum ;								

	/// <summary> 
	/// ͳ�Ʒ���
	/// </summary> 
	/// 
	typedef enum 
	{
		nicStatisticMode_Undefined = 0 ,			// δ֪�ļ��㷽ʽ
		nicStatisticMode_Count ,					// ����(ÿ�����������ԭʼֵ�ĸ�����������������)
		nicStatisticMode_Average ,				// ԭʼƽ��ֵ
		nicStatisticMode_Total ,					// ԭʼֵ���
		nicStatisticMode_Stdev ,					// ��׼����
		nicStatisticMode_Time_Total ,			// ʱ��ϳ����
		nicStatisticMode_Time_Average ,			// ʱ��ϳ�ƽ��ֵ
		nicStatisticMode_Minimum ,				// ��Сֵ(ÿ�����������ڵ���Сֵ�����������Ǻõ�ԭʼֵ)
		nicStatisticMode_Maximum ,				// ���ֵ(ÿ�����������ڵ����ֵ�����������Ǻõ�ԭʼֵ)
		nicStatisticMode_Minimum_Time ,			// ��Сֵ��Ӧ��ʱ��
		nicStatisticMode_Maximum_Time ,			// ���ֵ��Ӧ��ʱ��
		nicStatisticMode_Duration_Good ,			// �����ݵ�ʱ���ܳ�(ms)
		nicStatisticMode_Duration_Bad ,			// �����ݵ�ʱ���ܳ�(ms)
		nicStatisticMode_Start ,					// �����ڵĵ�һ��ԭʼ���ݣ������������Ǻõģ�Ҳ�����ǻ��ģ�
		nicStatisticMode_End ,					// �����ڵ����һ��ԭʼ���ݣ������������Ǻõģ�Ҳ�����ǻ��ģ�
		nicStatisticMode_Delta ,					// �����ڵĵ�һ�������һ���õ�ԭʼ����֮��
		nicStatisticMode_Range ,					// ���������/��Сֵ�ľ��Բ�ֵ��ֻ��������ݣ� 
		nicStatisticMode_Percent_Good ,			// �������ܳ�/���䳤��
		nicStatisticMode_Percent_Bad ,			// �������ܳ�/���䳤��
		nicStatisticMode_Max ,					// 
	} nicStatisticModeEnum ;	

	/// <summary> 
	/// ���ݲ�ѯʱ��ʱ��������
	/// </summary> 
	/// 
	typedef enum 
	{
		nicIntervalMode_Undefined = 0 ,			// δ����
		nicIntervalMode_MilliSecond ,			// ����
		nicIntervalMode_Second ,					// ��
		nicIntervalMode_Minute ,					// ��
		nicIntervalMode_Hour ,					// ʱ
		nicIntervalMode_Day ,					// ��
		nicIntervalMode_Month ,					// ��
		nicIntervalMode_Year ,					// ��
		nicIntervalMode_Week ,					// ��
		nicIntervalMode_Max ,					// ���������
	} nicIntervalModeEnum ;

	/// <summary> 
	///  ��ʷ���ݲ�ѯ������
	/// </summary> 
	typedef struct 
	{
		nicUInt32			TagCount ;			// ������
		nicUInt32*			TagIdList;			// ���ID�б�
		nicQueryModeEnum		QueryMode ;			// ��ѯ����
		nicTime				StartTime ;			// ��ʼʱ��
		nicTime				EndTime ;			// ����ʱ��
		nicDirectionEnum		Direction ;			// ȡ������
		nicUInt32			SamplingCount ;		// ȡ��������
		nicIntervalModeEnum	IntervalMode ;		// ȡ���������
		nicUInt32			IntervalTime ;		// ȡ�����ʱ��
		nicStatisticModeEnum	StatisticMode ;		// ͳ�Ʒ�ʽ 
		nicFilterModeEnum	FilterMode ;		// ���˷�ʽ 	
		nicStr				FilterExpression ;	// ���˱��ʽ
	} nicHisDataFilter ;	

	/// <summary> 
	///  ����ɾ����������
	/// </summary> 

	typedef struct 
	{
		nicUInt32			TagCount ;			// ������
		nicUInt32*			TagIdList ;			// ���ID�б�
		nicTime				StartTime ;			// ��ʼʱ��
		nicTime				EndTime ;			// ����ʱ��
	} nicDeleteDataFilter ;

	//=====================================================================
	// 
	// ��ʷ�����ļ�����nicArchive_*����غ��������ݽṹ���궨��
	// 
	//=====================================================================

	/// <summary> 
	///	 ��ʷ�����ļ���Ϣ
	/// </summary> 

	typedef struct 
	{
		nicUInt16			FileId ;			// �ļ���ʶ��
		nicTime				StartTime ;			// �ļ���¼���ݵĿ�ʼʱ��
		nicTime				EndTime ;			// �ļ���¼���ݵĽ���ʱ��
		nicStr				FileName ;			// �ļ���
	} nicDataFileInfo ;

	/// <summary> 
	///	 ��ʷ�����ļ��б�
	/// </summary> 

	typedef struct
	{
		nicUInt16			DataFileCount ;		// ��ʷ�����ļ�����
		nicDataFileInfo*		DataFileList ;		// ��ʷ�����ļ��б�
	} nicDataFileList ;




	/// <summary> 
	///	��������״̬
	/// </summary> 
	typedef enum
	{
		nicProcess_run = 0 ,			// ��������
		nicProcess_stop ,				// ����ֹͣ
		nicProcess_noBuffer ,			// ���������岻��
		nicProcess_Max,
	} nicProcessStatus ;

	/// <summary> 
	///	��������״̬
	/// </summary> 
	typedef enum
	{
		Connections_run = 0 ,			// ����������������
		Connections_stop ,              // ���������߳�ֹͣ
		Connections_noBuffer ,			// �������ӻ��岻��
		Connections_waitReConnect,		// �������ӵȴ�����
		Connections_notexist ,          // ���������Ѳ�����
		Connections_clientInterrupt,	// �ͻ��������ѶϿ�
	} nicConnectionStatus;

	/// <summary> 
	///	 ��������Ϣ��������ȼ�
	/// </summary> 

	typedef enum
	{
		MSGQ_Priority_High = 1,
		MSGQ_Priority_Normal = 2,
		MSGQ_Priority_Low = 4,
	}MsgQueuePriority;


	/// <summary> 
	///	����������˹���ģʽ
	/// </summary> 
	/// 
	typedef enum
	{
		IP_DISABLE_FILTER = 1,
		IP_IS_FILTER = 2,
		IP_IS_ACCESS = 4,
	}IPFilterMode;


	///IPC����ͨ�ŵı�ʶ

	typedef enum
	{
		IPC_ID_Max = 0xFFFF,
		IPC_ID_Switch = IPC_ID_Max -1,
		//ʵʱ���ϵĽ���ID��
		IPC_ID_Alarm =IPC_ID_Max - 2,
		IPC_ID_IOSS = IPC_ID_Max - 3,
		IPC_ID_Tran = IPC_ID_Max - 4,
		IPC_ID_Db  = IPC_ID_Max - 5,
		IPC_ID_Config  = IPC_ID_Max - 6,
		IPC_ID_User1  = IPC_ID_Max - 7,
		IPC_ID_User2  = IPC_ID_Max - 8,
		IPC_ID_User4  = IPC_ID_Max - 9,
		//��ʷ���
		IPC_ID_Hisd  = IPC_ID_Max - 10,	
		IPC_ID_AalrmHisd  = IPC_ID_Max - 11,	
		IPC_ID_Event  = IPC_ID_Max - 12,	
		IPC_ID_Log  = IPC_ID_Max - 13,	
		IPC_ID_SOE = IPC_ID_Max - 14,

		IPC_ID_Min = IPC_ID_Max - 32,
	}Nicsys_IPC_ID;

	///���罻��������ģ��״̬
	typedef enum
	{
		SwitchCore_Stop = 0,
		SwitchCore_Puase,
		SwitchCore_Idle,
		SwitchCore_Normal,
		SwitchCore_Bussy,
		SwitchCore_Run,
	}nicSwitchTaskStatus;

	///���罻�����������ṹ��

	typedef struct
	{
		nicUInt32  Switch_P2P_up;			//��������
		nicUInt32  Switch_P2P_down;			//��������
		nicUInt32  Switch_DB_CastAll;		//�ڴ����ݿ�ಥȫ��
		nicUInt32  Switch_DB_CastLocal;		//�ڴ����ݿ�ಥ������������
		nicUInt32  Switch_CastNet;			//DB�ͱ���ģ��ಥ����������ͻ���
		nicUInt32  Switch_DB_CastAlarm;		//DB�ಥ������ģ��
		nicUInt32  Switch_DB_CastIOSS;		//DB�ಥ��IOSSģ��
		nicUInt32  Switch_DB_CastTran;		//DB�ಥ�����ݷ���ģ��
		nicUInt32  Switch_DB_CastConfig;	//DB�ಥ�����ݿ����ģ��
		nicUInt32  Switch_Alarm_CastAll;	//����ģ��ಥ������ģ��
		nicUInt32  Switch_IOSS_CastLocal;	//IOSSģ��ಥ����������������ģ��
		nicUInt32  Switch_Client_Write;		//�ͻ���д����
		nicUInt32  Switch_Other;			//�������͵�����
	}nicSwitchStatistic;

//////////////////////////////////////////////////////////////////////////
	//���ı���״̬����
	//��Ϊ�˼�
	typedef struct
	{
		nicBool		EnableHHHH:1;				//�Ƿ�֧��H4����
		nicBool		EnableHHH:1;				//�Ƿ�֧��HHH����
		nicBool		EnableHH:1;				//�Ƿ�֧��HH����
		nicBool		EnableHI:1;				//�Ƿ�֧��H����
		nicBool		EnableLO:1;				//�Ƿ�֧��L����
		nicBool		EnableLL:1;				//�Ƿ�֧��LL����
		nicBool		EnableLLL:1;				//�Ƿ�֧��LLL����
		nicBool		EnableLLLL:1;				//�Ƿ�֧��L4����
		nicBool		EnableRate:1;				//�Ƿ�֧��RATE����
		nicBool		UnUsed:7;						//ռλ
	}nicAnaAlarmMask;

////ȫ�ֺ�������
//�ڲ�����ʹ�õ�

	typedef  nicAPIStatus  (*TagCBFunc) (nicVoid*,nicUInt32 unTagId) ;


	typedef  nicAPIStatus  (*RealDataNotifyCBFunc) (nicVoid *pList) ;
	typedef  nicAPIStatus  (*AddTagNotifyCBFunc) (nicUInt32 unTagId,nicPropValueList *valList) ;
	typedef  nicAPIStatus  (*DelTagNotifyCBFunc) (nicUInt32 unTagId) ;
	typedef  nicAPIStatus  (*ModifyTagNotifyCBFunc) (nicUInt32 unTagId, nicPropValueList *propValueList) ;


	typedef struct
	{
		RealDataNotifyCBFunc func_notify;
		AddTagNotifyCBFunc   func_addtag;
		DelTagNotifyCBFunc   func_deltag;
		ModifyTagNotifyCBFunc func_modtag;
	}nicOnlineConfigProc;
//////////////////////////////////////////////////////////////////////////

CDECL_END
#pragma pack( pop  , NicDefs )
#endif ///  __Nicsys_DEFS_H__
