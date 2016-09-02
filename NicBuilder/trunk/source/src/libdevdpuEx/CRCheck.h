//===================================================================================================================//
//= File Function: CRC Check
//= Author: Zander Sun
//= Date: 2013-10-10
//= Create for: (20-1), v1.0.0.20

//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update    
//===================================================================================================================//
#ifndef _CRCheck_h_
#define _CRCheck_h_

#include "precomp.h"

#define POLY16_CCITT 0x1021

// ��ʼ��CRCУ�����б�
void InitCRCTable(void);
// ��ʼ��CRCУ�����б��ÿһ��Ԫ��
__u16 CalCRCTable(__u16 data, __u16 genpoly);
// �������ݱ���У��
__u16 CalSFCRC(__u8 *pData, __u32 len);

//__u16 UpdateCRC ( __u8 Data, __u16 CRC );

// �ж�ÿһ֡ĩβ��CRCУ�����Ƿ�һ��






#endif