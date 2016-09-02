#ifndef __modicon_h__
#define __modicon_h__

#include "mbx/mbxapi.h"
#include "mbx/mbxcom.h"
#include "mbx/mbxerror.h"
#include "mbx/mbxgnpkt.h"
#include "mbx/mbxioerr.h"
#include "mbx/netbios.h"
#include "mbx/netlib.h"

struct modicon{
	typedef unsigned short mreg;
	// 寄存器0区，1区，2区，3区，4区
	mreg *r[4];
	int   c[4];
	// 硬件缓冲区寄存器有效性标志
	bool  *vf,*valid_flags[4];
	bool ok;
	// PLC 地址
	int  did;
	// 当前刷新的寄存器区
	int  cr;
	//  当前寄存器区中剩余没读的寄存器个数
	int  rl;
	HANDLE    h_device;
	MBXPATHID path;
	
	modicon();
	virtual ~modicon();

	int   construct(int cid,int c0,int c1,int c3,int cr);
	void  destroy();

	// 刷新硬件数据库
	void  flush_h_buffer();
	int   get_bool_value(int region_id,int bit_id,int * value);
	int   get_float_value(int region_id,int reg_offset,float * value,char at=0);
	int   write_bool_value(int region_id,int bit_id,int value,MBXPATHID path);
	int   write_float_value(int region_id,int reg_offset,float value,MBXPATHID path,char at);
protected:
	int   generate_packet(MBXPKT * pkt,int z,bool is_read, int p1=0,int p2=0,float p3=0,char at=-1);
};

#endif