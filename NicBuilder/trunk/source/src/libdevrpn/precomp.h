#include <pmc.h>
#include <stdarg.h>
#include <direct.h>
#include <string.h>
#include <locale.h>


#define rtk_serial_recv_len    8096

#define RPN_PTCL_LEN 100   //RPNЭ���ܳ�

#define RPN_HEADER_MAGIC_LEN    4 //���볤��

#define RPN_S_RANGE  0x01     //Դ����
#define RPN_M_RANGE   0x02		//�м�����
#define RPN_P_RANGE   0x03		//��������