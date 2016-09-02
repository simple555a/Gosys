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

#include "hisdcomm.h"

#ifdef _WIN32
#include <direct.h>
#define HISD_Export_Flag __declspec (dllexport)
#define HISD_Import_Flag __declspec (dllimport) 
#else 

#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
* �������ܣ��ͷ���ʷ���ں��������ʷ�����ڴ棬������ָ��ΪNULL
* �����������Ҫ�ͷŵ�ָ��ĵ�ַ
* �����������
* ����ֵ������
*/
HISD_Export_Flag hisdVoid Nichis_FreeHisdData(HisdData **memblock);

/*
* �������ܣ�������ʷ��
* ���������ϵͳ�����ռ�c_workspace
* �����������
* ����ֵ�����������
*/
HISD_Export_Flag hisdInt16 Nichis_SysCreate(const hisdAStr c_workspace, const hisdInt8 c_sysmode);

/*
* �������ܣ���ʼ����ʷ��
* �����������
* �����������
* ����ֵ������ʼ�����
*/
HISD_Export_Flag hisdInt16 Nichis_SysInit();

/*
 * �������ܣ�������ʷ��
 * �����������
 * �����������
 * ����ֵ�����������
 */
 HISD_Export_Flag hisdInt16 Nichis_SysStart();

/*
* �������ܣ��˳���ʷ��
* �����������
* �����������
* ����ֵ�����˳����
*/
HISD_Export_Flag hisdInt16 Nichis_SysExit();

/*
* �������ܣ�����ͬ��ϵͳ�ļ���������ǰ�ڴ�ء��洢�ļ�����Ͳ�����
* �����������
* �����������
* ����ֵ����ͬ�����
*/
HISD_Export_Flag hisdInt16 Nichis_SynHisSystem();

/*
* �������ܣ�����ϵͳ�����ļ�
* �����������
* �����������
* ����ֵ�������ݽ��
*/
HISD_Export_Flag hisdInt16 Nichis_BakSysParaFiles();

/*
* �������ܣ��趨ϵͳ����ģʽ�������ѭ���滻ģʽ����Ҫ������ʷ�����ļ������
* ���������ö��������ģʽ����c_runmodel����ʷ�����ļ������c_maxhisfilenum
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetRunModel(const HisdRunModel c_runmodel, const hisdUInt64 c_maxhisfilenum);

/*
* �������ܣ��趨�洢�ļ�����ʱ����������Ϊ��λ���������Ҫ�Զ����棬����<0��ֵ
* ���������ʱ����
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetSavePeriod(const hisdUInt32 c_saveinterval);

/*
* �������ܣ��趨ϵͳ���в����ļ��������ڣ�����Ϊ��λ���������Ҫ���ݣ�����<0��ֵ
* �����������������
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetBakPeriod(const hisdUInt32 c_bakperiod);

/*
* �������ܣ��趨ϵͳ�ڴ�ش�С�����ֽ�Ϊ��λ
* ����������ڴ�ش�С
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetMPoolSize(const hisdInt64 c_mpoolsize);

/*
* �������ܣ��趨����ѹ�����
* ������������ID��tagID��ѹ�����compinterval
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetCompIntervalOfTag(const hisdInt32 tagID, hisdUInt32 compinterval);

/*
* �������ܣ��趨���ѹ��ƫ������Ŀǰֻ��ת���㷨��Ч
* ������������ID��ѹ��ƫ������ѹ��ƫ������
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetCompOffsetOfTag(const hisdInt32 tagID, hisdDouble compoffset, hisdInt8 compoffset_type);

/*
* �������ܣ��趨ϵͳ�����ļ�����������ҳ��������
* ��������������ļ�ҳ������datafile_pagenumber���趨Ϊ��ֵʱ�����Զ��趨ΪĬ������
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetDatafilePara(hisdUInt32 datafile_pagenumber);

/*
* �������ܣ��趨ϵͳ�����ļ�����������ҳ�������������趨Ϊdatafile_pagenumber��1/15����
* ��������������ļ�ҳ������indexfile_pagenumber���趨Ϊ��ֵʱ�����Զ��趨ΪĬ������
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetIndexfilePara(hisdUInt32 indexfile_pagenumber);

/*
* �������ܣ��趨ϵͳ�Ƿ����д����ʷ���ݼ�飬������ݰ���д�����ʷ�����Ƿ�����һ�£����Ұ���ʱ��д���
* ����������Ƿ���м�飬Ĭ��Ϊ�����
* �����������
* ����ֵ������
*/
HISD_Export_Flag void Nichis_SetHisdataCheck(hisdBool isCheck);

/*
* �������ܣ�дtagID������ʷ����
* ������������ID��tagID����д�����ʷ����pdataitems����д����ʷ���ݵĸ���datacount
* �����������
* ����ֵ����д����
*/
HISD_Export_Flag hisdInt16 Nichis_WriteData(const hisdInt32 tagID, HisdData *pdataitems, hisdInt32 datacount);

/*
* �������ܣ���ȡtagID���ѹ�������ʷ����
* ������������ID��tagID�������ؼ�ʱ���ptimes
* �����������������ʷ����out_pdataitems��������ʷ���ݵĸ���out_datacount
* ����ֵ������ȡ���
* ��    ע��ptimes����Ҫ���ٳ�ʼ��5����С��ptimes[0]��ʾ��ȡʱ��ε���ʼʱ�̣�ptimes[1]��ʾʱ��ε���ֹʱ�̡�
*/
HISD_Export_Flag hisdInt16 Nichis_ReadCompressDataItem(const hisdInt32 tagID, hisdTime *ptimes, HisdData **out_pdataitems, hisdInt32 *out_datacount);

/*
* �������ܣ���ȡtagID���ĳһ��ʱ�̵���ʷ���ݣ�����ֵ�ɲ�ֵ������
* ������������ID��tagID��ʱ�̵�ptime
* �����������������ʷ����out_pdataitem
* ����ֵ������ȡ���
*/
HISD_Export_Flag hisdInt16 Nichis_ReadOneDataItem(const hisdInt32 tagID, hisdTime *ptime, HisdData **out_pdataitem);

/*
* �������ܣ����ո�������ȡtagID�������ݡ�������ʱ�̵��Ǹ�������ʱ���Ŀ�����ƽ���ֲ��ģ�ֵ�ɲ�ֵ������
* ������������ID��tagID���ؼ�ʱ���ptimes�����ݵĸ���datacount
* ���������������ѹ������out_pdataitems
* ����ֵ������ȡ���
* ��    ע��ptimes����Ҫ���ٳ�ʼ��5����С��ptimes[0]��ʾ��ȡʱ��ε���ʼʱ�̣�ptimes[1]��ʾʱ��ε���ֹʱ�̣��������ڲ���ʹ��ptimes[2]
*/
HISD_Export_Flag hisdInt16 Nichis_ReadDataItemsByCount(const hisdInt32 tagID, hisdTime *ptimes, hisdInt32 datacount, HisdData **out_pdataitems);

/*
* �������ܣ������ض���ʱ��������ȡtagID�������ݡ�����ֵ�ɲ�ֵ������
* ������������ID��tagID���ؼ�ʱ���ptimes
* ���������������ѹ������out_pdataitems���������ݵĸ���out_datacount
* ����ֵ������ȡ���
* ��    ע��ptimes����Ҫ���ٳ�ʼ��5����С��ptimes[0]��ʾ��ȡʱ��ε���ʼʱ�̣�ptimes[1]��ʾʱ��ε���ֹʱ�̣�ptimes[2]��ʾ�ض���ʱ������
*/
HISD_Export_Flag hisdInt16 Nichis_ReadDataItemsByInterval(const hisdInt32 tagID, hisdTime *ptimes, HisdData **out_pdataitems, hisdInt32 *out_datacount);

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
HISD_Export_Flag hisdInt16 Nichis_HisdataSummary(const hisdInt32 tagID, hisdTime *ptimes, hisdInt8 summarymode, hisdDouble *out_pctgooddata, hisdDouble *out_datasummary);

#ifdef __cplusplus
};
#endif

#endif