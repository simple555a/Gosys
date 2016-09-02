#ifndef __NICSYS_ONET_H_
#define __NICSYS_ONET_H_
//ONetЭ���ͨ�ýӿ�Э���ļ�������Ϊ���Խ�����ߵ�����Ӧ�ÿͻ��˵ĵײ��װ�ӿ�
//֧�ֱ�׼C�ӿ�



/** @brief ��ʼ������ ���ڽ�������ǰ����*/
//��Ҫ����һ��ģʽ��һ����̬��ֻ֧��һ��������(Ĭ��)��һ������������֧�ֶ������

int init_connect();

/** @brief ����DPU����*/
int connect_dpu(char *ip1,char *ip2,int connnect_style);

/** @brief ����DPU����*/
int disconnect_dpu(char *ip1);

//�����ٻ���ģʽ
//10H ���ٻ������ٻ��Ƕ�����DPU���������ݽ��в�ѯ����ʵ�Ǹ�����Э�飬��socket��TCPЭ�����ǲ��еġ�
//		��Ϊ���������£�������·������DPU�����Խ��յ����ٻ������TCP�»��ǵ�һ��һ���ķ��ͣ������UDPЭ��,��Ҳ������Ч
//11H �ٻ�ĳ��DPU���������ݣ�������DPU״̬��IO״̬
//12H �ٻ�ĳ��DPU���������ݣ�����DPU״̬��IO״̬
//13H �ٻ�����DPU���������ݣ�������DPU״̬��IO״̬
//14H �ٻ�����DPU���������ݣ�����DPU״̬��IO״̬
int set_data_cmd();


//�õ�һ��DPU��ϵͳ״̬��������IO״̬
int get_DPU_status(int dpuAddr);


//�õ�һ��IO������ݣ�������ģ�������ǿ�������Ҳ���� (����IO��)VDI������
int get_IO_value(void *IOstruct);


//͸���õ�һ��IO������ݣ�������ģ�������ǿ������������� (����IO��)VDI������
int get_transIO_value(void *IOstruct);


//�õ�һ��IO������ݣ�������ģ�������ǿ�������Ҳ���� (����IO��)VDI������
int set_IO_value(void *IOstruct);

//͸������һ��IO������ݣ�������ģ�������ǿ������������� (����IO��)VDI������
int set_transIO_value(void *IOstruct);


//���һ��DPU����ͨ��״̬
bool get_online_status(int dpuAddr);

//���һ��DPU��Э������״̬
bool get_link_status(int dpuAddr);

// ����Ƿ���pingͨ
bool check_ping(char* IP, int timeout);



#endif
