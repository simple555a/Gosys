/*
	Modicon PLC communication module.
	
	Created: 
	by Jackie, some time in the past

	Revised:
	by Jackie, 2003/7/4
	fixed bug, the bug may cause failure to read registers beyond 2000 bit variable(zone
	0 or zone 1) or 124 analog value(zone 3 or zone 4)
*/
#include "precomp.h"

#pragma comment (lib, "mbxapi.lib")

modicon::modicon()
{
	memset(this,0,sizeof(*this));
}

int modicon::construct(int did,int c0,int c1,int c3,int c4)
{	
	memset(this,0,sizeof(*this));
	this->did=did;
	cr=0;
	r[0]=new mreg[c0];
	r[1]=new mreg[c1];
	r[2]=new mreg[c3];
	r[3]=new mreg[c4];
	vf=new bool[c0+c1+c3+c4];
	rl=c0;
	if(!r[0] || !r[1] || !r[2] || !r[3] || !vf){
		ok=false;
	}else{
		this->c[0]=c0;
		this->c[1]=c1;
		this->c[2]=c3;
		this->c[3]=c4;
		valid_flags[0]=vf;
		valid_flags[1]=valid_flags[0]+c0;
		valid_flags[2]=valid_flags[1]+c1;
		valid_flags[3]=valid_flags[2]+c3;
		memset(vf,false,sizeof(vf[0])*(c0+c1+c3+c4));
		ok=true;
	}
	return ok? 1 : 0;
}

modicon::~modicon()
{
	destroy();
}

void modicon::destroy()
{
	int i;
	for(i=0;i<3;i++){
		if(r[i]) {delete r[i];c[i]=0;r[i]=0;}
	}
	if(vf){delete vf;vf=0;}
	ok=false;
}

/*
  根据不同情况生成命令包
  参数：
  pkt : IN/OUT, 命令包
  z   : 操作区号
  is_read : 读还是写操作
  p1  : 参数1,根据不同情况，可以是寄存器地址，或位地址
  p2  : 参数2,是寄存器个数
*/
int modicon::generate_packet(MBXPKT * _pkt, int z,bool is_read, int p1, int p2,float p3,char at)
{
	int ret;
	MBXGENPKT * pkt=&_pkt->GenMb;
    p1--;

    unsigned short usv;
    short sv;

    switch(z){
	case 0:
		if(is_read){
			/*
			gaReceiveCells(lReceiveCount).ReplyPktSize = MAXPKTSIZE
			gaReceiveCells(lReceiveCount).SendPkt.Fnc = 1      '开关量输出
			gaReceiveCells(lReceiveCount).SendPkt.Data(0) = NodeCells(i).Cell0(j).Offset \ &H100   '偏移高位
			gaReceiveCells(lReceiveCount).SendPkt.Data(1) = NodeCells(i).Cell0(j).Offset And &HFF  '偏移低位
			gaReceiveCells(lReceiveCount).SendPkt.Data(2) = NodeCells(i).Cell0(j).Length \ &H100   '长度高位
			gaReceiveCells(lReceiveCount).SendPkt.Data(3) = NodeCells(i).Cell0(j).Length And &HFF  '长度低位
			*/
			p1 &= 0xffff;
			pkt->Fnc=1;
			pkt->Data[0]=p1>>8;
			pkt->Data[1]=p1&0xff;
            //p2*=8; why???
			pkt->Data[2]=p2>>8;
			pkt->Data[3]=p2&0xff;
			ret=5;
		}else{
			/*
            gaCommandCells(tagFound.PLCIndex).SendPkt.Fnc = &HF
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(0) = tagFound.Offset \ &H100     '偏移高位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(1) = tagFound.Offset And &HFF    '偏移低位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(2) = 0                           '长度高位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(3) = 1                           '长度低位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(4) = 1                           '1byte
            If Value > 0 Then
                gaCommandCells(tagFound.PLCIndex).SendPkt.Data(5) = 1                       '开
            Else
                gaCommandCells(tagFound.PLCIndex).SendPkt.Data(5) = 0                       '关
            End If
            gaCommandCells(tagFound.PLCIndex).SendPktSize = gaCommandCells(tagFound.PLCIndex).SendPkt.Data(4) + 6
			*/
			p1 &= 0xffff;
			pkt->Fnc=0xf;
			pkt->Data[0]=p1>>8;
			pkt->Data[1]=p1&0xff;
			pkt->Data[2]=0;
			pkt->Data[3]=1;
			pkt->Data[4]=1;
			pkt->Data[5]=p2? 1:0;
			ret=7;
		}
		break;
	case 1:
		/*
        gaReceiveCells(lReceiveCount).SendPkt.Fnc = 2      '开关量输入
        gaReceiveCells(lReceiveCount).SendPkt.Data(0) = NodeCells(i).Cell1(j).Offset \ &H100   '偏移高位
        gaReceiveCells(lReceiveCount).SendPkt.Data(1) = NodeCells(i).Cell1(j).Offset And &HFF  '偏移低位
        gaReceiveCells(lReceiveCount).SendPkt.Data(2) = NodeCells(i).Cell1(j).Length \ &H100   '长度高位
        gaReceiveCells(lReceiveCount).SendPkt.Data(3) = NodeCells(i).Cell1(j).Length And &HFF  '长度低位
		*/
		p1 &= 0xffff;
		pkt->Fnc=2;
		pkt->Data[0]=p1>>8;
		pkt->Data[1]=p1&0xff;
        //p2*=8;
		pkt->Data[2]=p2>>8;
		pkt->Data[3]=p2&0xff;
		ret=5;
		break;
	case 2:
		/*
        gaReceiveCells(lReceiveCount).SendPkt.Fnc = 4      '寄存器输入
        gaReceiveCells(lReceiveCount).SendPkt.Data(0) = NodeCells(i).Cell3(j).Offset \ &H100   '偏移高位
        gaReceiveCells(lReceiveCount).SendPkt.Data(1) = NodeCells(i).Cell3(j).Offset And &HFF  '偏移低位
        gaReceiveCells(lReceiveCount).SendPkt.Data(2) = NodeCells(i).Cell3(j).Length \ &H100   '长度高位
        gaReceiveCells(lReceiveCount).SendPkt.Data(3) = NodeCells(i).Cell3(j).Length And &HFF  '长度低位
		*/        
		p1 &= 0xffff;
		pkt->Fnc=4;
		pkt->Data[0]=p1>>8;
		pkt->Data[1]=p1&0xff;
		pkt->Data[2]=p2>>8;
		pkt->Data[3]=p2&0xff;
		ret=5;
		break;
	case 3:
		if(is_read){
			/*
			gaReceiveCells(lReceiveCount).SendPkt.Fnc = 3      '寄存器输出
			gaReceiveCells(lReceiveCount).SendPkt.Data(0) = NodeCells(i).Cell3(j).Offset \ &H100   '偏移高位
			gaReceiveCells(lReceiveCount).SendPkt.Data(1) = NodeCells(i).Cell3(j).Offset And &HFF  '偏移低位
			gaReceiveCells(lReceiveCount).SendPkt.Data(2) = NodeCells(i).Cell3(j).Length \ &H100   '长度高位
			gaReceiveCells(lReceiveCount).SendPkt.Data(3) = NodeCells(i).Cell3(j).Length And &HFF  '长度低位
			*/
			p1 &= 0xffff;
			pkt->Fnc=3;
			pkt->Data[0]=p1>>8;
			pkt->Data[1]=p1&0xff;
			pkt->Data[2]=p2>>8;
			pkt->Data[3]=p2&0xff;
			ret=5;
		}else{
			/*
            gaCommandCells(tagFound.PLCIndex).SendPkt.Fnc = &H10
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(0) = tagFound.Offset \ &H100     '偏移高位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(1) = tagFound.Offset And &HFF    '偏移低位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(2) = 0                           '长度高位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(3) = 2                           '长度低位
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(4) = 4                           '4byte
            '转换字节顺序
            Dim abytTemp(0 To 3) As Byte
            CopyMemory abytTemp(0), Value, 4
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(5 + 1) = abytTemp(0)
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(5 + 0) = abytTemp(1)
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(5 + 3) = abytTemp(2)
            gaCommandCells(tagFound.PLCIndex).SendPkt.Data(5 + 2) = abytTemp(3)
            gaCommandCells(tagFound.PLCIndex).SendPktSize = gaCommandCells(tagFound.PLCIndex).SendPkt.Data(4) + 6
			*/
            if( -1 == at ){
			    p1 &= 0xffff;
			    pkt->Fnc=0x10;
			    pkt->Data[0]=p1>>8;
			    pkt->Data[1]=p1&0xff;
			    pkt->Data[2]=0;
			    pkt->Data[3]=1;
			    pkt->Data[4]=2;
                usv = 1;
                char * p = (char*)&usv;
			    pkt->Data[5+1]=p[0];
			    pkt->Data[5+0]=p[1];
			    ret=8;
            }else if( 0 == at ){ // ui2
			    p1 &= 0xffff;
			    pkt->Fnc=0x10;
			    pkt->Data[0]=p1>>8;
			    pkt->Data[1]=p1&0xff;
			    pkt->Data[2]=0;
			    pkt->Data[3]=1;
			    pkt->Data[4]=2;
                usv = p3;
                char * p = (char*)&usv;
			    pkt->Data[5+1]=p[0];
			    pkt->Data[5+0]=p[1];
			    ret=8;
            }else if( 1 == at ){ // i2
			    p1 &= 0xffff;
			    pkt->Fnc=0x10;
			    pkt->Data[0]=p1>>8;
			    pkt->Data[1]=p1&0xff;
			    pkt->Data[2]=0;
			    pkt->Data[3]=1;
			    pkt->Data[4]=2;
                sv = p3;
                char * p = (char*)&sv;
			    pkt->Data[5+1]=p[0];
			    pkt->Data[5+0]=p[1];
			    ret=8;
            }else{  // r4
			    p1 &= 0xffff;
			    pkt->Fnc=0x10;
			    pkt->Data[0]=p1>>8;
			    pkt->Data[1]=p1&0xff;
			    pkt->Data[2]=0;
			    pkt->Data[3]=2;
			    pkt->Data[4]=4;
			    char * p=(char*)&p3;
			    pkt->Data[5+1]=p[0];
			    pkt->Data[5+0]=p[1];
			    pkt->Data[5+3]=p[2];
			    pkt->Data[5+2]=p[3];
			    ret=10;
            }
		}
		break;
	default:
		assert(0);
	}
	return ret;
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
void  modicon::flush_h_buffer()
{
	int rd,starting_register;
#define BUFSIZE 1024
	static char buf[BUFSIZE],rbuf[BUFSIZE];
	USHORT routing[5];
	MBXPKT *p,*rp;
	int length,response_length,ret_code;

	memset(routing,0,sizeof(routing));
	routing[0]=did;	

	p=(MBXPKT*)buf,rp=(MBXPKT*)rbuf;
	
    if(0 == c[cr]){
        cr++;
		if(cr == 4) cr = 0;
		rl = c[cr];
    }
    // for 0x and 1x, we can read upto 2000 once
    // for 3x and 4x, we can read upto 125 once
    // infact we read 124 register once
    if(cr < 2){
        rd = 2000;
    }else{
        rd = 124;
    }

	if(rl<rd) rd=rl;
	if(rd){
		// 读这些数
		length = generate_packet(p, cr, true, c[cr] - rl + 1, rd);
		response_length=BUFSIZE;
		ret_code=MbxWriteMpCmdWait(
			h_device,
			path,
			routing,
			sizeof(routing),
			p,
			length,
			rp,
			(DWORD*)&response_length,
			1000
		);
        // refresh buffer
		/*
			2003/7/3, found bug here,
			previous : starting_register=c[cr]-rd;
		*/
		starting_register=c[cr]-rl;
        if(cr < 2){
            starting_register /= 16;
        }
		if(0 == ret_code){
			// yeah, got a valid response
			memset(valid_flags[cr]+starting_register,true,rd*sizeof(valid_flags[0][0]));
            if(cr < 2){
    			memcpy(r[cr]+starting_register,rp->GenMb.Data+1,sizeof(r[cr][0])*rd/16);
            }else{
    			memcpy(r[cr]+starting_register,rp->GenMb.Data+1,sizeof(r[cr][0])*rd);
            }
		}else{
			memset(valid_flags[cr]+starting_register,false,rd*sizeof(valid_flags[0][0]));
		}
		rl -= rd;		
	}
	if(!rl){
		// 当前区读完了，到下一区
		cr++;
		if(cr == 4) cr=0;
		rl=c[cr];
	}
}

/**************************************************************************
	从硬件缓冲区中读开关量
	参数：
	region_id:区号0,1
	bit_id:位号，从1开始编号
	value:返回数据
	返回值：0数据无效，1数据有效
***************************************************************************/
int modicon::get_bool_value(int region_id,int reg_id,int * value)
{
    if( 0 == region_id || 1 == region_id ){ // 0x or 1x
        if(reg_id < 1 || reg_id > c[region_id] * 16){
            return 0;
        }
	    reg_id--;
	    int reg_offset=reg_id >> 4;
	    int bit_offset=reg_id &  15;
	    // is this register valid?
	    if( !valid_flags[region_id][reg_offset] ){
		    return 0;
	    }
	    *value = r[region_id][reg_offset] & (1<<bit_offset);
    }else if( 3 == region_id || 4 == region_id ){ // 3x or 4x
        if(  reg_id < 1 || reg_id > c[region_id] ){
            return 0;
        }
        reg_id--;
        if( !valid_flags[region_id][reg_id] ){
            return 0;
        }
        *value = r[region_id][reg_id];
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
int modicon::get_float_value(int region_id,int reg_offset,float * value,char at)
{
    float fv;
    short sv;
    unsigned short usv;
    char * tmp;

	if(region_id !=3 && region_id !=4 ){
		//utils_error(">> modicon, shit!%d, no such region.\n",region_id);
		return 0;
	}
	region_id--;
	/*
		registers in the h_buffer are indexed with base-0, so we must
		decrement the reg_offset to access the h_buffer
	*/
	reg_offset--;
	if(reg_offset<0 || reg_offset>=c[region_id]){
		//utils_error(">> modicon, shit!%d, no such register.\n",reg_offset+1);
		return 0;
	}
	// is this register valid?
	if(!valid_flags[region_id][reg_offset]){
		return 0;
	}
    // get original value
	char * swap = (char*)&r[region_id][reg_offset];
    if(3 == region_id && 2 == at){ // real4
	    tmp = (char*)&fv;
	    tmp[0] = swap[1];
	    tmp[1] = swap[0];
	    tmp[2] = swap[3];
	    tmp[3] = swap[2];
    }else if(3 == region_id && 1 == at){ //int2
        tmp = (char*)&sv;
	    tmp[0] = swap[1];
	    tmp[1] = swap[0];
        fv = sv;        
    }else{
        tmp = (char*)&usv;
	    tmp[0] = swap[1];
	    tmp[1] = swap[0];
        fv = usv;        
    }

    *value = fv;
    return 1;
}
/**************************************************************************
	向PLC写入开关量
	参数：
	region_id:区号0
	bit_id:从1开始的位号
	value:要写入的数值（true或false)
	返回值：1写入成功，0写入失败
***************************************************************************/
int   modicon::write_bool_value(int region_id,int reg_id,int value,MBXPATHID path)
{
    // is it a valid tag? check!!!
    if( 0 == region_id ){
        if( reg_id <1 || reg_id > c[region_id] * 16){
    		utils_error(">> modicon, shit!%d, no such bit.\n",reg_id);
            return 0;
        }
    }else if( 4 == region_id ){
        if( reg_id <1 || reg_id > c[region_id]){
    		utils_error(">> modicon, shit!%d, no such bit.\n",reg_id);
            return 0;
        }
    }else{
		utils_error(">> modicon, shit!%d, no such region.\n",region_id);
        return 0;
    }

	static char buf[BUFSIZE],rbuf[BUFSIZE];
	USHORT routing[5];
	MBXPKT *p,*rp;
	int length,response_length,ret_code;

	memset(routing,0,sizeof(routing));
	routing[0]=did;	

	p=(MBXPKT*)buf,rp=(MBXPKT*)rbuf;

	// 写数据	
	length=generate_packet(p,region_id,false,reg_id,value);
	response_length=BUFSIZE;
	ret_code=MbxWriteMpCmdWait(
		h_device,
		path,
		routing,
		sizeof(routing),
		p,
		length,
		rp,
		(DWORD*)&response_length,
		1000
	);
	if(0 == ret_code){
		// yeah, the write is succesuful
		return 1;
	}
	return 0;
}

/**************************************************************************
	向PLC写入模拟量
	参数：
	region_id:4
	reg_offset:起始寄存器地址，从1开始编号
	value:要写入的数值
	返回值：1写入成功，0写入失败
***************************************************************************/
int modicon::write_float_value(int region_id,int reg_offset,float value,MBXPATHID path,char at)
{
	if(region_id!=4){
		utils_error(">> modicon, shit!%d, no such region.\n",region_id);
		return 0;
	}
	region_id--;
	/*
		when talking to Modicon PLC, no need to decrement the reg_offset
		as we do in get_float_value, because Modicon PLC expects a 1-based
		offset.
	*/
	reg_offset;
	if(reg_offset<1 || reg_offset>c[region_id]){
		utils_error(">> modicon, shit!%d, no such register.\n",reg_offset);
		return 0;
	}
	char buf[BUFSIZE],rbuf[BUFSIZE];
	USHORT routing[5];
	MBXPKT *p,*rp;
	int length,response_length,ret_code;

	memset(routing,0,sizeof(routing));
	routing[0]=did;	

	p=(MBXPKT*)buf,rp=(MBXPKT*)rbuf;

	// 写数据
    if( 0 == at){ // ui2 limit "0 to 65535"
        if( value < 0 ){
            value = 0;
        }
        if( value > 65535 ){
            value = 65535;
        }
    }else if(1 == at){ // i2 limit "-32768 to 32767"
        if( value < -32768 ){
            value = -32768;
        }
        if( value > 32767 ){
            value = 32767;
        }
    }

    length=generate_packet(p,region_id,false,reg_offset,0,value,at);
	response_length=BUFSIZE;
	ret_code=MbxWriteMpCmdWait(
		h_device,
		path,
		routing,
		sizeof(routing),
		p,
		length,
		rp,
		(DWORD*)&response_length,
		1000
	);
	if(0 == ret_code){
		// yeah, the write is succesuful
		return 1;
	}
	return 0;
}
