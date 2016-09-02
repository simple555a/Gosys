#include <pmc.h>
#include <stdarg.h>
#include <direct.h>
#include <string.h>
#include <locale.h>


#define rtk_serial_recv_len    8096

#define RPN_PTCL_LEN 100   //RPN协议总长

#define RPN_HEADER_MAGIC_LEN    4 //掩码长度

#define RPN_S_RANGE  0x01     //源量程
#define RPN_M_RANGE   0x02		//中间量程
#define RPN_P_RANGE   0x03		//功率量程