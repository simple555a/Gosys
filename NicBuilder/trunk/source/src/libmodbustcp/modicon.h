#ifndef __modbus_tcp_h__
#define __modbus_tcp_h__

#ifdef _WIN32
#include <windows.h>
#include <winsock.h>
#endif
#include <librtk.h>

const int BUFSIZE = 261;

#define DEFAULT_ACQ_DELAY		150
#define DEFAULT_READ_TIMEOUT	1000
#define DEFAULT_CONN_TIMEOUT	1000
#define DEFAULT_MAX_FAILS		3
#define DEFAULT_BATCH_COUNT	124

struct connection_t{
	char	ip[16];	// ip addres
	SOCKET	s;		// socket handle
	int		f;		// fail count
	HANDLE	e;		// win32 event
};

struct modicon{
	typedef unsigned short mreg;
	// �Ĵ���0����1����2����3����4��
	mreg *r[4];
	mreg * rheap;
	int   c[4];
	// Ӳ���������Ĵ�����Ч�Ա�־
	__bool  *vf,*valid_flags[4];
	__bool ok;
	// PLC ��ַ
	connection_t * curr_connection;
	int m_batchCount;

    // ��д���ʵĻ��⴦��
	CLock lock;

	int  m_curr_section;		// ��ǰˢ�µļĴ�����(0,1,2,3)
	int  m_regs_left;	/*  ��ǰ�Ĵ�������ʣ��û���ļĴ������� */
	
	// event handle for async-mode socket io
	CEvent *m_event;
	connection_t connections[2];
	
	int m_MaxFails, failCount;
	int m_ConTimeout;
	int m_ReadTimeout;
	bool debug;

	modicon();
	virtual ~modicon();

	__bool construct(char *addr1, char*addr2, int c0, int c1, int c3, int c4);
	void  destroy();

	// ������������
	void  resetNet();
	// ˢ��Ӳ������
	void update_h_cache();
	void invalidate_cache();
	
	__bool read_analog(
		__uint region_id, 
		__uint reg_offset,
		TAG_TYPE tp,
		PTAG_VALUE v
		);

	__bool read_digital(
		__uint region_id,
		__uint reg_bit_id,
		__bool * v
		);
	
	__bool write_register(
		__uint reg_offset,
		TAG_TYPE tp, 
		TAG_VALUE v
		);

	__bool write_coil(
		__uint bit_id,
		__bool bit
		);

	void comm_fault_handler();
	void invalidate_connection(connection_t * c);
	connection_t * get_connection();
	bool build_connection(connection_t * c);
	void build_conections();
protected:
	int sendRead(connection_t * c, char obuf[], int olen, char ibuf[], int ilen, int rlen);
	int   lockSendRead(char *obuf, int olen, char *ibuf, int ilen, int rlen);
};

#endif
