/********************************************************************
	created:	2002:10:12   15:23
	author:		Jackie, jhunter@tsinghua.org.cn
	
	purpose:	F8 kernel adapter
*********************************************************************/
#ifndef __f8kadapter_h__
#define __f8kadapter_h__

#include <f8_kernel.h>
#include <pshpack1.h>

#ifdef _WIN32
#ifdef LIBKADAPTER_EXPORTS
#define KADAPTER_API __declspec(dllexport)
#else
#define KADAPTER_API __declspec(dllimport)
#endif
#else
#define KADAPTER_API
#endif

#define F8_ADAPTER_NO_LOCK	F8_EVENT(MODNUM_ADAPTER, EVT_CLASS_ERROR, -1)

CDECL_BEGIN

#define ADAPTER_F_ONLINE	(1<<0)
#define ADAPTER_F_BOUND	(1<<1)
#define ADAPTER_F_PAUSED (1<<2)

#define AT_FILE_ADAPTER	'FILE'
#define AT_SHM_ADAPTER	'SHM'
#define AT_TCP_ADAPTER	'TCP'

/* forward declaration */
struct __tag_IKAdapter;

typedef void (*KAdapterEventProc)(
	struct __tag_IKAdapter * a, 
	f8_int event, 
	f8_int param0, 
	f8_int param1
	);

#define AE_STATUS_CHANGED	-1
#define AE_IN_PROGRESS		-2
#define AE_CONNECTION_LOST	-3

__class__(IKAdapter)
	__uint	type;
	int		flags;
	KAdapterEventProc	eventSink;
	HF8_KERNEL	kernel;
	void	* msgQueue;
	void * hImportPinThread;
	__uint reserved[11];
__class_end__(IKAdapter)

__vtable__(IKAdapter)
	__vmethod__(
		f8_status, 
		online, 
		(IKAdapter * _this, const char * url, __uint timeout)
		);

	__vmethod__(
		f8_status, 
		offline, 
		(IKAdapter * _this)
		);

	__vmethod__(
		void,
		exec, 
		(IKAdapter * _this, struct kern_conf_cmd *, __uint timeout)
		);
	
	__vmethod__(
		void,
		exec_local, 
		(IKAdapter * _this, struct kern_conf_cmd *)
		);
	
	__vmethod__(
		void,
		exec_remote, 
		(IKAdapter * _this, struct kern_conf_cmd *, __uint timeout)
		);
	
	__vmethod__(
		f8_status,
		exec0, 
		(IKAdapter * _this, __u16 cmd, __uint timeout)
		);

__vtable_end__(IKAdapter)

KADAPTER_API __bool IKAdapter_queue_event(IKAdapter * __this, int event, int p0, int p1);

KADAPTER_API __bool IKAdapter_begin_transfer(IKAdapter * adapter);

KADAPTER_API __bool IKAdapter_end_transfer(IKAdapter * adapter);

KADAPTER_API __bool IKAdapter_pause_transfer(IKAdapter * adapter);

KADAPTER_API __bool IKAdapter_resume_transfer(IKAdapter * adapter);
/*
	several implementation of adapters
*/

/*
	In-process implementation
	Target is a DLL
*/
__class__(IFileAdapter)
	__inherit__(IKAdapter)
__class_end__(IFileAdapter)

__vtable__(IFileAdapter)
	__inherit__(IKAdapter_vtbl)
__vtable_end__(IFileAdapter)

/*
	Target is a process listening on Tcp port
*/
__class__(ITcpAdapter)
	__inherit__(IKAdapter)
	F8_SOCKET	m_socket;
#ifdef _WIN32
	void	* m_event;
#endif
	void	* m_lock;
__class_end__(ITcpAdapter)

__vtable__(ITcpAdapter)
	__inherit__(IKAdapter_vtbl)
__vtable_end__(ITcpAdapter)

/*
	Target is listening on shared memory
*/
__class__(IShmAdapter)
	__inherit__(IKAdapter)	
__class_end__(IShmAdapter)

__vtable__(IShmAdapter)
	__inherit__(IKAdapter_vtbl)
__vtable_end__(IShmAdapter)

KADAPTER_API void delete_adapter(IKAdapter * a);

CDECL_END

#include <poppack.h>

#endif	/**/

