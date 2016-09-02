#include "precomp.h"

#include <string>
using namespace std;

#undef DEBUG_PRINTF
#define DEBUG_PRINTF(__x__)\
	if(debug){printf __x__;}

static std::string texttime()
{
#ifdef _WIN32
	#define localtime(a,b) *(b)=*localtime(a)
#else	
	#define localtime(a,b) localtime_r(a,b)
#endif
	time_t tmt;
	struct tm ltm;
	time(&tmt);
	localtime(&tmt, &ltm);
	char buf[128];	  
	sprintf(
		buf,
		"%d年%d月%d日 %02d:%02d:%02d",
		ltm.tm_year+1900,
		ltm.tm_mon+1,
		ltm.tm_mday,
		ltm.tm_hour,
		ltm.tm_min,
		ltm.tm_sec
		);
#ifndef _WIN32
	#undef localtime
#endif	  
	return std::string(buf);
}

modicon::modicon()
{
#define sss(m) memset(&m, 0, sizeof(m))
	// 寄存器0区，1区，2区，3区，4区
	sss(r);
	sss(c);
	sss(valid_flags);
	vf = 0;
	ok =false;
	sss(connections);
	curr_connection = &connections[1];
#undef sss
	m_event	= new CEvent(0, true);
	m_ReadTimeout = DEFAULT_READ_TIMEOUT;
	m_ConTimeout = DEFAULT_CONN_TIMEOUT;
	m_MaxFails = DEFAULT_MAX_FAILS;
	failCount = 0;
	debug = false;
	int i;
	for(i=0; i<2; i++){
		connections[i].e = CreateEvent(NULL, TRUE, FALSE, NULL);
		connections[i].s = INVALID_SOCKET;
		connections[i].f = 0;
	}
	m_batchCount = DEFAULT_BATCH_COUNT;
}

modicon::~modicon()
{
	destroy();
}

void modicon::destroy()
{
	int	i;
	if(rheap){
		delete rheap;
		rheap = 0;
	}
	if(vf){
		delete vf;
		vf=0;
	}
	ok = __false;
	for(i=0; i<2; i++){
		invalidate_connection(&connections[i]);
		CloseHandle(connections[i].e);
		connections[i].e = 0;
	}
}

__bool modicon::construct(char *addr1, char *addr2, int c0, int c1, int c3, int c4)
{	
	ok = __false;

	strncpy(connections[0].ip, addr1, sizeof(connections[0].ip));
	connections[0].ip[sizeof(connections[0].ip)	- 1] = 0;
	strncpy(connections[1].ip, addr2, sizeof(connections[1].ip));
	connections[1].ip[sizeof(connections[1].ip)	- 1] = 0;

	m_curr_section = 0;
	int totalRegs = c0 + c1 + c3 + c4;
	if(!totalRegs){
		return __false;
	}
	rheap = new mreg[totalRegs];
	vf = new __bool[totalRegs];
	if(!rheap || !vf){
		return __false;
	}
	r[0] = rheap;
	r[1] = r[0] + c0;
	r[2] = r[1] + c1;
	r[3] = r[2] + c3;
	valid_flags[0] = vf;
	valid_flags[1] = valid_flags[0]	+ c0;
	valid_flags[2] = valid_flags[1]	+ c1;
	valid_flags[3] = valid_flags[2]	+ c3;
	
	m_regs_left = c0;
	this->c[0] = c0;
	this->c[1] = c1;
	this->c[2] = c3;
	this->c[3] = c4;
	memset(vf, false, sizeof(vf[0])	* (c0 +	c1 + c3	+ c4));

	ok = __true;

	return ok;
}

/*
	fabricate a read-buffer packet

	see ModiBus manual for detail.
*/
int generateReadPacket(
	char pkt[261],	
	int region,
	__u16 offset,
	__u16 count
	)
{
	memset(pkt, 0, 5);

	// packet size not including the path field
	pkt[5] = 6;
	pkt[6] = 0;
	
	switch(region){
	case 0:	
		// read coil status
		pkt[7] = 1;
		break;
		
	case 1:	
		// read input status
		pkt[7] = 2;
		break;
		
	case 2:
		// read input registers
		pkt[7] = 4;
		break;
		
	case 3:	
		// read holding registers
		pkt[7] = 3;
		break;
	default:
		assert(0);
	}

	offset--;
	
	pkt[8] = offset >> 8;
	pkt[9] = offset & 0xff;
	pkt[10] = count >> 8;
	pkt[11] = count & 0xff;
	
	return 12;
}

void modicon::invalidate_cache()
{
	int i;
	for(i=0; i<4; i++){
		memset(valid_flags[i], false, c[i] * sizeof(valid_flags[0][0]));
	}
}

void modicon::comm_fault_handler()
{
	failCount++;
	if(failCount == m_MaxFails){
		// failCount = 0;
		invalidate_cache();
	}
}

/* 
  刷新内存数据库
  任务：
  从当前区读取一定数量的寄存器到硬件缓冲区
  问：该数量C应该如何确定呢？
  聪明的小朋友应该已经想到了...
  设：如果Modicon一次能够读取的最大寄存器个数A
	  当前区剩余寄存器个数B
  则：C=MIN(A,B)
  对了，奖颗糖！
*/
void modicon::update_h_cache()
{
	int	rd,	starting_reg;
	char obuf[BUFSIZE], ibuf[BUFSIZE];
	int	length,	expected, ret_code;
	int	p1,	p2;
	int	i;

	if(!get_connection()){
		memset(vf, false, sizeof(vf[0])*(c[0] +	c[1] + c[2]	+ c[3]));
		return;
	}

	if(!m_regs_left){
		for(i=0; i<4; i++){
			m_curr_section++;
			if(m_curr_section == 4){
				m_curr_section = 0;
			}
			m_regs_left = c[m_curr_section];
			if(m_regs_left){
				break;
			}
		}
	}

	// we can read upto	128	registers once
	// this	must be	an even	number，为了保证浮点数是一次读出的
#if	0
	if(m_curr_section <	2){
		rd = 60;
	}else{
		rd = 124;
	}
#endif
	rd = m_batchCount;
	
	if(m_regs_left < rd) {
		rd = m_regs_left;
	}
	
	if(rd){	
		DEBUG_PRINTF((
			">> Modicon : Reading section %d, %d requested, %d registers left.\n", 
			m_curr_section, rd, m_regs_left
			));
		
		starting_reg = c[m_curr_section] - m_regs_left;	
		if(m_curr_section <	2){	
			p1 = (starting_reg << 4) + 1;
		}else{
			p1 = starting_reg +	1;
		}

		if(m_curr_section < 2){
			p2 = rd << 4;
		}else{
			p2 = rd;
		}

		// 读这些数
		length = generateReadPacket(obuf, m_curr_section, p1, p2);
		expected = 9 + rd * 2;
		ret_code = lockSendRead(obuf, length, ibuf,	BUFSIZE, expected);

		// refresh buffer
		
		if(0 == ret_code){
			// yeah, got a valid response
			memset(	
				valid_flags[m_curr_section] + starting_reg,	
				true, 
				rd * sizeof(valid_flags[0][0])
				);
			memcpy(	
				r[m_curr_section] +	starting_reg, 
				&ibuf[9], 
				sizeof(r[m_curr_section][0]) * rd
				);
			failCount = 0;
		}else{
			comm_fault_handler();
		}
		m_regs_left -= rd;		
	}
}

/**************************************************************************
	从硬件缓冲区中读开关量
	参数：
	region_id : 区号 0,1,3,4
	bit_id : 位号，从1开始编号
	value:返回数据
	返回值：0数据无效，1数据有效
***************************************************************************/
__bool modicon::read_digital(__uint region_id, __uint bit_id, __bool * value)
{
	if(0 == region_id || 1 == region_id ){	// 0x or 1x
		if(bit_id < 1 || bit_id > c[region_id] * 16){
			return 0;
		}
		bit_id--;
		int reg_offset = bit_id >> 4;
		int bit_offset = bit_id & 15;
		// is this register valid?
		if(!valid_flags[region_id][reg_offset]){
			return 0;
		}
		*value = r[region_id][reg_offset] & (1<<bit_offset)? __true : __false;
	}else if( 3 == region_id || 4 == region_id ){ 
		// 3x or 4x
		region_id--;
		if(bit_id	< 1	|| bit_id > c[region_id] ){
			return 0;
		}
		bit_id--;
		if(!valid_flags[region_id][bit_id]){
			return 0;
		}
		*value = r[region_id][bit_id]? __true : __false;
	}else{
		return 0;
	}

	return 1;
}

/**************************************************************************
	从硬件缓冲区中读模拟量
	参数：
	region_id:区号3或4
	reg_offset:起始寄存器地址，从1开始编号
	value:返回数据
	返回值：0数据无效，1数据有效
***************************************************************************/
__bool modicon::read_analog(
	__uint region_id,
	__uint reg_offset,
	TAG_TYPE tp,
	PTAG_VALUE value
	)
{
	char * tmp;

	if(region_id !=3 && region_id !=4 ){
		DEBUG_PRINTF((
			">> modicon, shit!%d, no such region.\n",
			region_id
			));
		return 0;
	}
	region_id--;
	reg_offset--;
	if(reg_offset<0 || reg_offset>=c[region_id]){
		DEBUG_PRINTF((
			">> modicon, shit!%d, no such register.\n",
			reg_offset+1
			));
		return 0;
	}

	// is this register valid?
	if(!valid_flags[region_id][reg_offset]){
		DEBUG_PRINTF((
			">> modicon, shit!%d, register invalid.\n",	
			reg_offset+1
			));	
		return 0;
	}

	if(tp == dt_real4 || tp == dt_int32){
		// check the additional register
		if(reg_offset > c[region_id] - 1){
			return __false;
		}
		
		if(!valid_flags[region_id][reg_offset + 1]){
			DEBUG_PRINTF((
				">> modicon, shit!%d, register invalid.\n", 
				reg_offset+1
				)); 
			return 0;
		}
	}
	
	// get original	value
	char * swap = (char*)&r[region_id][reg_offset];
	switch(tp){
	case dt_real4:
		tmp	= (char*)&value->flt;
		tmp[0] = swap[1];
		tmp[1] = swap[0];
		tmp[2] = swap[3];
		tmp[3] = swap[2];
		break;
	case dt_int16:
		tmp	= (char*)&value->i16;
		tmp[0] = swap[1];
		tmp[1] = swap[0];
		break;
	case dt_int32:
		tmp	= (char*)&value->i32;
		tmp[0] = swap[1];
		tmp[1] = swap[0];
		tmp[2] = swap[3];
		tmp[3] = swap[2];
		break;
	default:
		return __false;
	}

	return __true;
}

/**************************************************************************
	向PLC写入coil
	参数：
	bit_id : 起始寄存器地址，从1开始编号
	bit : 要写入的数值
	返回值：1写入成功，0写入失败
***************************************************************************/
__bool modicon::write_coil(
	__uint bit_id,
	__bool bit
	)
{
	char obuf[64];
	char ibuf[32];
	memset(obuf, 0, 12);

	bit_id--;

	obuf[5] = 6;
	obuf[6] = 0;
	/* force single coil */
	obuf[7] = 5;
	obuf[8] = bit_id >> 8;
	obuf[9] = bit_id & 0xff;
	obuf[10] = bit? 0xff : 0;
	obuf[11] = 0;

	int rr = 12;
	int ret_code = lockSendRead(obuf, 12, ibuf, BUFSIZE, 12);

	if(0 == ret_code){
		// yeah, the write is succesuful
		return __true;
	}
	
	return __false;
}

/**************************************************************************
	向PLC写入保持寄存器
	参数：
	reg_offset:起始寄存器地址，从1开始编号
	value:要写入的数值
	返回值：1写入成功，0写入失败
***************************************************************************/
__bool modicon::write_register(
	__uint reg_offset,
	TAG_TYPE tp, 
	TAG_VALUE v
	)
{
	char obuf[BUFSIZE], ibuf[BUFSIZE];
	int ret_code;
	__uint size;
	
	// 写数据
	memset(obuf, 0, sizeof(obuf));
	
	obuf[6] = 0;
	switch(tp){
	case dt_bool:
	case dt_uint16:
	case dt_int16:
		size = 1;
		break;
	case dt_int32:
	case dt_uint32:
	case dt_real4:
		size = 2;
		break;
	case dt_int64:
	case dt_uint64:
	case dt_real8:
	case dt_date:
		size = 4;
		break;
	default:
		assert(0);
		size = 1;
		break;
	}
	
	// preset multiple registers
	obuf[7] = 0x10;

	// N.B. although concept use 1-based address, the address embedded
	// in modbus protocol is 0-based
	reg_offset--;
	obuf[8] = reg_offset >> 8;
	obuf[9] = reg_offset & 0xff;
	obuf[10] = 0;
	obuf[11] = size;
	obuf[12] = size * 2;

	{
		char * s, * d;
		int i;
		d = obuf + 13;
		/* copying and swapping */
#if HOST_IS_SMALL_ENDIAN
		for(i=0, s = (char *)&v; i<size*2; i++){
			*d++ = s[1];
			*d++ = s[0];
			s += 2;
		}
#else
		for(i=size*2, s = (char *)&v;; i; i--){
			*d++ = *s++;
		}
#endif
	}

	// packet data size
	obuf[5] = size * 2 + 7;
	int rr = 12;
	ret_code = lockSendRead(obuf, size * 2 + 13, ibuf, BUFSIZE, rr);
	
	if(0 == ret_code){
		// yeah, the write is succesuful
		return __true;
	}
	
	return __false;
}

/*
	一次读写交易
	返回0表示成功，<0表示需要复位, >0不需要复位
*/
int modicon::sendRead(
	connection_t * c,
	char obuf[], 
	int olen,
	char	ibuf[], 
	int ilen,
	int rlen
	)
{
	int	i;
	WSANETWORKEVENTS events;
	
	if(!c){
		return -1;
	}
	// reset event
	WSAEnumNetworkEvents(c->s, c->e, &events);
	i = send(c->s, obuf, olen, 0);
	if(i != olen){
		return -1;
	}

	WaitForSingleObject(c->e, m_ReadTimeout);
	// read response
	i = recv(c->s, ibuf, ilen, 0);
	DEBUG_PRINTF((
		"%d bytes from modicon got(requested %d).\n",
		i,
		rlen
		));
	if(i < 0){
		return i;
	}
	if(i != rlen){
		return 1;
	}
	if(ilen >= 9){
		if (ibuf[7] & 0x80){
			DEBUG_PRINTF((
				"MODBUS	exception response - type %d\n", ibuf[8]
				));
			return -1;
		}
	}

	return 0;
}

/*
  加了锁的读写交易
  socket并非线程安全，所以要加互斥处理
*/
int modicon::lockSendRead(char *obuf, int olen, char *ibuf, int ilen, int rlen)
{
	connection_t * c;

	c = get_connection();
	if(!c){
		return 0;
	}
	if(INVALID_SOCKET == c->s){
		return 0;
	}

	int ret;

	lock.Lock();
	if(INVALID_SOCKET == c->s){
		lock.UnLock();
		return 0;
	}
	ret	= sendRead(c, obuf, olen, ibuf, ilen, rlen);
	DEBUG_PRINTF(("sendRead = %d\n", ret));
	if(ret < 0){
		invalidate_connection(c);
	}
	lock.UnLock();

	return ret;
}

/*
	marks an interface as unavailable, thus	
	1) the interface will not be chosen for next transaction;
	2) when CModiconDevice's connection watch'dog expires, a 
	re-connection will be made on this interface;
	3) if all other interfaces are down, then on subsequent
	transactions all interfaces	will be checked	for connection
	in a round-robin manner.
*/
void modicon::invalidate_connection(connection_t * c)
{
	string t;
	
	if(INVALID_SOCKET != c->s){
		t = texttime();
		utils_error(
			">> MDTCP : %s, connection to %s:%d is broken.\n",
			t.c_str(),
			c->ip, 502
			);
		closesocket(c->s);
	}
	c->s = INVALID_SOCKET;
}

/*
	build a connection on interface	indicated by 'c'.
*/
bool modicon::build_connection(connection_t * c)
{
	struct sockaddr_in server;
	int ret;
	WSANETWORKEVENTS events;

	if(c->s != INVALID_SOCKET){
		return true;
	}

	if(!c->ip[0]){
		return false;
	}

	c->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	if(INVALID_SOCKET == c->s){
		wperror(">> Modicon, socket().");
		return false;
	}

	server.sin_family = AF_INET;
	server.sin_port	= htons(502); // ASA standard port
	server.sin_addr.s_addr = inet_addr(c->ip);

	utils_debug("MBTCP - connecting to %s\n", c->ip);
	memset(&events, 0, sizeof(events));
	WSAEventSelect(c->s, c->e, FD_CONNECT | FD_READ);
	WSAEnumNetworkEvents(c->s, c->e, &events);
	ret = connect(c->s, (sockaddr *)&server, sizeof(sockaddr_in));
	WaitForSingleObject(c->e, m_ConTimeout);
	ret = WSAEnumNetworkEvents(c->s, (*m_event).Handle(), &events);
	if(
		!ret &&	
		(events.lNetworkEvents & FD_CONNECT) &&	
		(!events.iErrorCode[FD_CONNECT_BIT])
	){
		utils_debug(
			"Modicon device connected on %s:%d\n",
			c->ip,
			502
			);
	}else{
		closesocket(c->s);	
		c->s = INVALID_SOCKET;
	}
	return c->s == INVALID_SOCKET? false : true;
}

/*
	select an available connection,	if both interfaces are down,
	do a re-connect on the least-recently-used interface.
*/
connection_t * modicon::get_connection()
{	
	connection_t * c;

	if(INVALID_SOCKET != connections[0].s){
		return &connections[0];
	}
	if(INVALID_SOCKET != connections[1].s){
		return &connections[1];
	}	

	lock.Lock();

	c =	curr_connection;
	if(c == &connections[0]){
		c = connections[1].ip[0]? &connections[1] : &connections[0];
	}else{
		c = &connections[0];
	}
	curr_connection = c;
	build_connection(c);

	lock.UnLock();

	return c->s == INVALID_SOCKET? NULL : c;
}

void modicon::build_conections()
{
	lock.Lock();
	build_connection(&connections[0]);
	build_connection(&connections[1]);
	lock.UnLock();
}

