//=========================================================================
//  Nicsys1000   �汾:  2.0
//-------------------------------------------------------------------------
//  
//  ʵʱ���ͨ�ò����궨�嶨���ļ�
//
//-------------------------------------------------------------------------
//  ��Ȩ (C) 2012 - �����к˿���ϵͳ�������޹�˾(CNCS)
//=========================================================================
// 
//  �Ķ���ʷ��
//  	1. 2012/02/27 : 	CNCS-0136 �����ļ�
//=========================================================================
#ifndef __NICSYS_PROCOMP_H__
#define __NICSYS_PROCOMP_H__

//����C����ע�͸�ʽ��.net��XMLע�Ͳ����Ͼ���
#pragma warning(disable:4635)

//����VSƽ̨�����Դ��İ�ȫC��������
#pragma warning(disable:4996)

#ifdef  __cplusplus
#define CDECL_BEGIN extern "C" {
#define CDECL_END }
#define DEFAULT_PARAM(val) = val
#else
#define CDECL_BEGIN 
#define CDECL_END 
#define DEFAULT_PARAM(val) 
#endif



/// <summary>
///	���������ʽ����
/// </summary>
#ifdef _WIN32
#define NICSYS_API _stdcall
#define nicAPI  _stdcall
#else
#define NICSYS_API
#define nicAPI 
#endif


#define NICSYSDLL	
#define nicRET_OK	0 							// ���سɹ�
#define nicRET_Fail (-1)							// ����ʧ�ܣ���ͬ�ڷ��ش���
#define nicOk(ret)	(!((nicAPIStatus)(ret)))		// �жϷ���ֵ�Ƿ���ȷ
#define nicErr(ret)	(((nicAPIStatus)(ret)))		// �жϷ���ֵ�Ƿ����
#define nicIN								// �������
#define nicOUT								// ��������
#define nicNULL			0					// ��


/// <summary>
///		�ж�ָ���Ƿ�Ϊ��
/// </summary>
#define IsNull(s) (!(s))

/// <summary>
///		�ж�ָ���Ƿ�ǿ�
/// </summary>
#define IsNotNull(s) (s)


/// <summary>
///		������ؽ������nicRET_OK���򷵻�
/// </summary>
#define RET_ERR(s)				\
	nRet = (s);					\
	if (nicErr(nRet))			\
	{							\
	return nRet;			\
	}


#ifdef _WIN32
#define _WIN_HAVE_STRUPR
#define _WIN_HAVE_MILISECOND_SLEEP
#define _WIN_HAVE_SNPRINTF
#define _WIN_HAVE_PROFILE_SUPPORT
#endif


#ifdef __GNUC__
#define strcmpi  strcasecmp
#define strnicmp strncasecmp
#endif

#ifndef CDECL
#define CDECL
#endif

#ifdef __GNUC__
#define PACKED __attribute__((packed))
#define __CONSTRUCTOR__ __attribute__((constructor))
#define __DESTRUCTOR__ __attribute__((destructor))
#define __FASTCALL__(x)	x __attribute__((regparm(3)))
#define __STDCALL__(x) x __attribute__((stdcall))
#else
#define PACKED
#define __CONSTRUCTOR__
#define __DESTRUCTOR__
#define __FASTCALL__(x) __fastcall x
#define __STDCALL__(x) __stdcall x
#endif

/* backward compatible */
#define CONFIG_FILE				"config/NicDB.ini"




//��ǩ������
#define NAME_SIZE 32
//��ǩ��������
#define TAG_DESC_SIZE 64
//�ļ�������
#define MAX_FILE_NAME_LENGTH 64
//�ļ�·������
#define MAX_FILE_PATH_LENGTH 512
//�Զ�����������
//#define DEFAULT_INCREASE_STEP 1024
#define DEFAULT_INCREASE_STEP 1024*512
//����Map�е��������
//#define DEFAULT_NAMEHASH_SIZE (DEFAULT_INCREASE_STEP * 4)
#define DEFAULT_NAMEHASH_SIZE (DEFAULT_INCREASE_STEP )
//�Զ���ĳ���
#define SIZE_LENGTH_16  16
#define SIZE_LENGTH_08  8
#define SIZE_LENGTH_04  4
#define SIZE_LENGTH_32  32
#define SIZE_LENGTH_64  64
#define SIZE_LENGTH_128  128
#define SIZE_LENGTH_256  256
#define SIZE_LENGTH_512  512
#define SIZE_LENGTH_1024  1024
#define SIZE_LENGTH_2048	2048



//������֪ͨ�¼����������������
#define BATCH_NOTIFY_SIZE 512
#endif
