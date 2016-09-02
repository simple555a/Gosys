/*
	tty based ICommPeer implementation
*/
#ifdef __VXWORKS__
#include <vxworks.h>
#include <taskLib.h>
#include <timers.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

__class__(ICommPeer)
	__inherit__(IPeer)
	int	hTty;
__class_end__(ICommPeer)

__vtable__(ICommPeer)
	__inherit__(IPeer_vtbl)
__vtable_end__(ICommPeer)

static int foox;

static f8_status ICommPeer_find(IPeer * _this, struct kstatus_t * t)
{
	return F8_UNIMPLEMENTED;
}

static int ICommPeer_write(IPeer * _this, const void * buf, int len)
{
	ICommPeer * __this;
	__this = __dcast__(IPeer, ICommPeer, _this);
	if(__this->hTty == ERROR){
		return F8_INVALID_OPERATION;
	}
	return write(__this->hTty, (char*)buf, len);
}

static int ICommPeer_read(IPeer * _this, void * buf, int len, int to)
{
	ICommPeer * __this;
	fd_set rset;
	struct timeval timeout;
	int ret;

	__this = __dcast__(IPeer, ICommPeer, _this);
	if(__this->hTty == ERROR){
		return F8_INVALID_OPERATION;
	}
	FD_ZERO(&rset);
	FD_SET(__this->hTty, &rset);
	timeout.tv_sec = 0;
	timeout.tv_usec = to * 1000;
	ret = select(__this->hTty + 1, &rset, NULL, NULL, &timeout);
	if(FD_ISSET(__this->hTty, &rset)){
		ret = read(__this->hTty, (char*)buf, len);
	}else{
		ret = -1;
	}
	return ret;
}

__global_constructor__(ICommPeer)
{
	__v_copy__(ICommPeer, IPeer);
	__override__(ICommPeer, IPeer, find);
	__override__(ICommPeer, IPeer, read);
	__override__(ICommPeer, IPeer, write);
}

__constructor__(ICommPeer)
{
	__touch__();
	__create_base__(ICommPeer, IPeer, __this);
	/*
		on ETX  board, the second serial port is used as
		hot-standby communication channel
	*/
	__this->hTty = open("/tyCo/1", O_RDWR, 0);
	return __this;
}

__destructor__(ICommPeer)
{
	if(ERROR != __this->hTty){
		close(__this->hTty);
		__this->hTty = ERROR;
	}
	__destroy_base__(ICommPeer, IPeer, __this);
	return __this;
}

__define_vtbl__(ICommPeer)
__define_vtbl_end__(ICommPeer)

#endif
