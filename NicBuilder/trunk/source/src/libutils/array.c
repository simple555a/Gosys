#pragma warning(disable:4996)  // Zander 2013-1-22

#include "precomp.h"
#pragma hdrstop

/*************************************************************************
	Make byte array from a text string
	for example:
		"35aab4d5" --> {0x35,0xaa,0xb4,0xd5}
	parameters:
		szArray : <in> the text string
		pbArray : <out> the receiving buffer
		length  : <in/out>, on input, specify max length of pbArray, on return
		, contains actual bytes produced, or 0 if failed.
	returns:
		total length of the szArray.
*************************************************************************/
int UTILS_API byte_array_by_hex_string(const char * szArray,unsigned char * pbArray,int * length)
{
	int len=(strlen(szArray)>>1);
	int i;
	char * p = (char*)szArray;
	for(i=0;i<len && i<*length;i++,p+=2)
	{
		char byte_save=p[2];
		int tmp;
		p[2]=0;
		sscanf(p,"%x",&tmp);
		pbArray[i]=(unsigned char)tmp;
		p[2]=byte_save;
	}
	*length=i;
	return len;
}
