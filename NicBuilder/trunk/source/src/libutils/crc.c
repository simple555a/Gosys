#include "precomp.h"

static int CRCTable[256];

void initCRCTable()
{
  int crc, poly;
  int i, j;
  poly = 0xEDB88320L;
  for(i=0;i<256;i++){ 
    crc = i;
    for(j=8;j>0;j-- ){
      if(crc&1) crc = (crc>>1)^poly;
      else crc >>= 1;
     }
    CRCTable[i] = crc;
   }
 }

LIBUTILS_API int UTILS_API utils_crc32(const char * buffer,int length)
{
  int crc = 0xFFFFFFFF;
  char * ptr = (char *)buffer;
  while(length-->0) crc = ((crc>>8)&0x00FFFFFF)^CRCTable[(crc^*ptr++)&0xFF];
  return(crc);
}

