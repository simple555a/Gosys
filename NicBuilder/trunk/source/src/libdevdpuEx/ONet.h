#ifndef __NICSYS_ONET_H_
#define __NICSYS_ONET_H_
//ONet协议的通用接口协议文件，可作为调试界面或者第三方应用客户端的底层封装接口
//支持标准C接口



/** @brief 初始化环境 ，在建立连接前调用*/
//需要设置一个模式，一个动态库只支持一个控制器(默认)，一个控制器可以支持多个连接

int init_connect();

/** @brief 建立DPU连接*/
int connect_dpu(char *ip1,char *ip2,int connnect_style);

/** @brief 建立DPU连接*/
int disconnect_dpu(char *ip1);

//设置召唤的模式
//10H 总召唤，总召唤是对所有DPU的所有数据进行查询，其实是个串口协议，在socket的TCP协议下是不行的。
//		因为串口连接下，串口链路上所有DPU都可以接收到总召唤命令，而TCP下还是得一个一个的发送，如果是UDP协议,则也可以有效
//11H 召唤某个DPU的所有数据，不包括DPU状态和IO状态
//12H 召唤某个DPU的所有数据，包括DPU状态和IO状态
//13H 召唤所有DPU的所有数据，不包括DPU状态和IO状态
//14H 召唤所有DPU的所有数据，包括DPU状态和IO状态
int set_data_cmd();


//得到一个DPU的系统状态，不包括IO状态
int get_DPU_status(int dpuAddr);


//得到一个IO点的数据，无论是模拟量还是开关量，也包括 (虚拟IO卡)VDI的数据
int get_IO_value(void *IOstruct);


//透传得到一个IO点的数据，无论是模拟量还是开关量，不包括 (虚拟IO卡)VDI的数据
int get_transIO_value(void *IOstruct);


//得到一个IO点的数据，无论是模拟量还是开关量，也包括 (虚拟IO卡)VDI的数据
int set_IO_value(void *IOstruct);

//透传设置一个IO点的数据，无论是模拟量还是开关量，不包括 (虚拟IO卡)VDI的数据
int set_transIO_value(void *IOstruct);


//检查一个DPU网络通信状态
bool get_online_status(int dpuAddr);

//检查一个DPU的协议链接状态
bool get_link_status(int dpuAddr);

// 检查是否能ping通
bool check_ping(char* IP, int timeout);



#endif
