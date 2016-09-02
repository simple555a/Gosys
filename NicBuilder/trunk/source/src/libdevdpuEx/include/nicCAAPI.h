#ifndef __NICSYS_COMPUTEALARM_H__
#define __NICSYS_COMPUTEALARM_H__

#include "nicDefs.h"
#include <vector>
#include <string>

#ifdef _WIN32
#if defined LIB_CA_EXPORTS
#define CA_API __declspec(dllexport)
#else
#define CA_API __declspec(dllimport)
#endif
#else
#define CA_API
#endif

NICSYSDLL CA_API nicInt32 NICSYS_API CA_Start(nicVoid);

NICSYSDLL CA_API nicInt32 NICSYS_API CA_Stop(nicVoid);

NICSYSDLL CA_API nicInt32 NICSYS_API CA_AddTag(nicIN nicPropValueList* pPropValueList);

NICSYSDLL CA_API nicInt32 NICSYS_API CA_DelTag(nicUInt32 unTagId);

NICSYSDLL CA_API nicInt32 NICSYS_API CA_UpdateTag(nicIN nicUInt32 unTagId, nicIN nicData* newData, nicIN nicTime* pTime);

NICSYSDLL CA_API nicInt32 NICSYS_API CA_WriteDevice(nicIN nicUInt32 unTagId, nicIN nicVariant* pValue);

NICSYSDLL CA_API nicInt32 NICSYS_API CA_WriteDevice_Ex(nicTagIdList* pTagIdList, nicVariant* pValueList);

//splitstring������nicArgs.h�е�parase_arg_exA���������棬���ú������ڸ��ӣ���Ե����������͵������
//����ǰ�����һ��������ڵ��ַ��������жϣ��ʲ�ʹ��parase_arg_exA����
NICSYSDLL CA_API nicInt32 NICSYS_API CA_SplitString(const nicChar* src, nicChar token, std::vector<std::string> &vec_result);

NICSYSDLL CA_API nicInt32 NICSYS_API CA_AlarmTranslate(nicChar* src, nicData* pData);

#endif