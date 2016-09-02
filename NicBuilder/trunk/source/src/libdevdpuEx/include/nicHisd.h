/*
* �ļ����ƣ�hisAPI.h
* ժ����Ҫ����ʷ������ṩ��API�ӿ�
* ��ǰ�汾��1.0
* �������ߣ���˧
* ������ڣ�2011��7��28��
*
* ��ע��
*
*       
*
* �޸ļ�¼��
* ���ڡ����������汾�����޸��ˡ����޸�����
* 
*
*/
#ifndef _HISAPI_H_
#define _HISAPI_H_

#include "nicDefs.h"


#ifdef _WIN32
#if defined	LIB_HISD_EXPORTS
#define HISD_API	  __declspec(dllexport)
#else
#define HISD_API	  __declspec(dllimport)
#endif
#else
#define HISD_API
#endif

CDECL_BEGIN

/*
* �������ܣ��ͷ���ʷ���ں��������ʷ�����ڴ棬������ָ��ΪNULL
* �����������Ҫ�ͷŵ�ָ��ĵ�ַ
* �����������
* ����ֵ������
*/

NICSYSDLL HISD_API nicVoid  NICSYS_API Nichis_FreeHisdData(nicData **memblock);

/*
* �������ܣ�������ʷ��
* ���������ϵͳ�����ռ�c_workspace
* �����������
* ����ֵ�����������
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysCreate(const nicAStr c_workspace, const nicInt8 c_sysmode);

/*
* �������ܣ���ʼ����ʷ��
* �����������
* �����������
* ����ֵ������ʼ�����
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysInit();

/*
 * �������ܣ�������ʷ��
 * �����������
 * �����������
 * ����ֵ�����������
 */
 NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysStart();

/*
* �������ܣ��˳���ʷ��
* �����������
* �����������
* ����ֵ�����˳����
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SysExit();

/*
* �������ܣ�����ͬ��ϵͳ�ļ���������ǰ�ڴ�ء��洢�ļ�����Ͳ�����
* �����������
* �����������
* ����ֵ����ͬ�����
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_SynHisSystem();

/*
* �������ܣ�����ϵͳ�����ļ�
* �����������
* �����������
* ����ֵ�������ݽ��
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_BakSysParaFiles();

/*
* �������ܣ��趨ϵͳ����ģʽ�������ѭ���滻ģʽ����Ҫ������ʷ�����ļ������
* ���������ö��������ģʽ����c_runmodel����ʷ�����ļ������c_maxhisfilenum
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetRunModel(const nicHisdRunModel c_runmodel, const nicUInt64 c_maxhisfilenum);

/*
* �������ܣ��趨�洢�ļ�����ʱ����������Ϊ��λ���������Ҫ�Զ����棬����<0��ֵ
* ���������ʱ����
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetSavePeriod(const nicUInt32 c_saveinterval);

/*
* �������ܣ��趨ϵͳ���в����ļ��������ڣ�����Ϊ��λ���������Ҫ���ݣ�����<0��ֵ
* �����������������
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetBakPeriod(const nicUInt32 c_bakperiod);

/*
* �������ܣ��趨ϵͳ�ڴ�ش�С�����ֽ�Ϊ��λ
* ����������ڴ�ش�С
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetMPoolSize(const nicInt64 c_mpoolsize);

/*
* �������ܣ��趨����ѹ�����
* ������������ID��tagID��ѹ�����compinterval
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetCompIntervalOfTag(const nicInt32 tagID, nicUInt32 compinterval);

/*
* �������ܣ��趨���ѹ��ƫ������Ŀǰֻ��ת���㷨��Ч
* ������������ID��ѹ��ƫ������ѹ��ƫ������
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetCompOffsetOfTag(const nicInt32 tagID, nicDouble compoffset, nicInt8 compoffset_type);

/*
* �������ܣ��趨ϵͳ�����ļ�����������ҳ��������
* ��������������ļ�ҳ������datafile_pagenumber���趨Ϊ��ֵʱ�����Զ��趨ΪĬ������
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetDatafilePara(nicUInt32 datafile_pagenumber);

/*
* �������ܣ��趨ϵͳ�����ļ�����������ҳ�������������趨Ϊdatafile_pagenumber��1/15����
* ��������������ļ�ҳ������indexfile_pagenumber���趨Ϊ��ֵʱ�����Զ��趨ΪĬ������
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetIndexfilePara(nicUInt32 indexfile_pagenumber);

/*
* �������ܣ��趨ϵͳ�Ƿ����д����ʷ���ݼ�飬������ݰ���д�����ʷ�����Ƿ�����һ�£����Ұ���ʱ��д���
* ����������Ƿ���м�飬Ĭ��Ϊ�����
* �����������
* ����ֵ������
*/
NICSYSDLL HISD_API void NICSYS_API Nichis_SetHisdataCheck(nicBool isCheck);

/*
* �������ܣ�дtagID������ʷ����
* ������������ID��tagID����д�����ʷ����pdataitems����д����ʷ���ݵĸ���datacount
* �����������
* ����ֵ����д����
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_WriteData(const nicInt32 tagID, nicData *pdataitems, nicInt32 datacount);

/*
* �������ܣ���ȡtagID���ѹ�������ʷ����
* ������������ID��tagID�������ؼ�ʱ���ptimes
* �����������������ʷ����out_pdataitems��������ʷ���ݵĸ���out_datacount
* ����ֵ������ȡ���
* ��    ע��ptimes����Ҫ���ٳ�ʼ��5����С��ptimes[0]��ʾ��ȡʱ��ε���ʼʱ�̣�ptimes[1]��ʾʱ��ε���ֹʱ�̡�
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadCompressDataItem(const nicInt32 tagID, nicTime *ptimes, nicData **out_pdataitems, nicInt32 *out_datacount);

/*
* �������ܣ���ȡtagID���ĳһ��ʱ�̵���ʷ���ݣ�����ֵ�ɲ�ֵ������
* ������������ID��tagID��ʱ�̵�ptime
* �����������������ʷ����out_pdataitem
* ����ֵ������ȡ���
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadOneDataItem(const nicInt32 tagID, nicTime *ptime, nicData **out_pdataitem);

/*
* �������ܣ����ո�������ȡtagID�������ݡ�������ʱ�̵��Ǹ�������ʱ���Ŀ�����ƽ���ֲ��ģ�ֵ�ɲ�ֵ������
* ������������ID��tagID���ؼ�ʱ���ptimes�����ݵĸ���datacount
* ���������������ѹ������out_pdataitems
* ����ֵ������ȡ���
* ��    ע��ptimes����Ҫ���ٳ�ʼ��5����С��ptimes[0]��ʾ��ȡʱ��ε���ʼʱ�̣�ptimes[1]��ʾʱ��ε���ֹʱ�̣��������ڲ���ʹ��ptimes[2]
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadDataItemsByCount(const nicInt32 tagID, nicTime *ptimes, nicInt32 datacount, nicData **out_pdataitems);

/*
* �������ܣ������ض���ʱ��������ȡtagID�������ݡ�����ֵ�ɲ�ֵ������
* ������������ID��tagID���ؼ�ʱ���ptimes
* ���������������ѹ������out_pdataitems���������ݵĸ���out_datacount
* ����ֵ������ȡ���
* ��    ע��ptimes����Ҫ���ٳ�ʼ��5����С��ptimes[0]��ʾ��ȡʱ��ε���ʼʱ�̣�ptimes[1]��ʾʱ��ε���ֹʱ�̣�ptimes[2]��ʾ�ض���ʱ������
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_ReadDataItemsByInterval(const nicInt32 tagID, nicTime *ptimes, nicData **out_pdataitems, nicInt32 *out_datacount);

/*
* �������ܣ�����mode�����ã�ͳ�Ƽ���tagID���ĳʱ����ڵ������ص�
* ������������ID��tagID���ؼ�ʱ���ptimes��ͳ�Ƽ�������summarymode
* �������������ͳ�Ƽ�����out_datasummary����������ռ�ٷֱ�out_pctgooddata�����Ϊ1��
* ����ֵ����ͳ�Ƽ�����
* ��    ע��1�����κ�ģʽ�£�ptimes����Ҫ���ٳ�ʼ��5����С��ptimes[0]��ʾ��ȡʱ��ε���ʼʱ�̣�ptimes[1]��ʾʱ��ε���ֹʱ�̣��������ڷ���ֵʹ��
*           2��NICHIS_DATASUMMARY_MAXIMUMʱ��ptimes[3]�������ֵ��Ӧ��ʱ��
*           3��NICHIS_DATASUMMARY_MAXIMUMʱ��ptimes[3]������Сֵ��Ӧ��ʱ��
*           4��NICHIS_DATASUMMARY_RANGEʱ��ptimes[3]������Сֵ��Ӧ��ʱ�̣�ptimes[4]�������ֵ��Ӧ��ʱ��
*/
NICSYSDLL HISD_API nicInt16 NICSYS_API Nichis_HisdataSummary(const nicInt32 tagID, nicTime *ptimes, nicInt8 summarymode, nicDouble *out_pctgooddata, nicDouble *out_datasummary);

CDECL_END

#endif