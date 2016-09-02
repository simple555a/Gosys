//===================================================================================================================//
//= File Function: CRC Check
//= Author: Zander Sun
//= Date: 2013-10-10
//= Create for: (20-1), v1.0.0.20

//= UPDATE RECORD
//= Update for     Version       Update by       Date            Update (function names modified)     
//===================================================================================================================//

#include "CRCheck.h"

__u16 g_CRCTable[256];

void InitCRCTable(void)
{
	__u16	i;

	for (i=0; i<256; i++)
	{
		g_CRCTable[i] = CalCRCTable(i, POLY16_CCITT);
	}
}

__u16 CalCRCTable(__u16 data, __u16 genpoly)
{
	int 	i;
	__u16	accum;	 

	data <<= 8;
	accum = 0;
	for (i=8; i>0; i--)
	{
		if ((data^accum) & 0x8000)    
		{
			accum = (accum << 1) ^ genpoly;
		}
		else
		{
			accum <<= 1;
		}
		data <<= 1;
	}
	return (accum);
}

__u16 CalSFCRC(__u8 *pData, __u32 len)
{
	__u16 	crc;
	__u32	i;

	crc = 0;
	for (i=0; i<len; i++)
	{
		crc = (crc << 8) ^ g_CRCTable[(crc >> 8) ^ *(pData + i)];
	}
	return 	crc;
}