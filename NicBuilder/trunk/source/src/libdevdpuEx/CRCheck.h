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

// 初始化CRC校验码列表
void InitCRCTable(void);
// 初始化CRC校验码列表的每一个元素
__u16 CalCRCTable(__u16 data, __u16 genpoly);
// 计算数据报的校验
__u16 CalSFCRC(__u8 *pData, __u32 len);

//__u16 UpdateCRC ( __u8 Data, __u16 CRC );

// 判断每一帧末尾的CRC校验码是否一致






#endif