/*
	F8 error codes.
*/
#ifndef __f8_statuss_h__
#define __f8_statuss_h__

#define MODNUM_NONE				-1
#define MODNUM_ADAPTER			1
#define MODNUM_PROXY			2
#define MODNUM_KERNEL			3
#define MODNUM_BLKLIB			4
#define MODNUM_FCC				5

#define F8_SUCCESS				0
#define F8_PATH_NOT_FOUND		-1
#define F8_NAME_DUPLICATE		-2
#define F8_LOW_MEMORY			-3
#define F8_INVALID_DATA			-4
#define F8_PERMISSION			-5
#define F8_INVALID_OPERATION	-6
#define F8_TOO_MANY_TIMERS		-7
#define F8_ACCESS				-8
#define F8_CONFIG				-9
#define F8_BUSY					-10
#define F8_TIMEOUT				-11
#define F8_SYNTAX				-12
#define F8_FILE_NOT_FOUND		-13
#define F8_ADAPTER_OFFLINE		-14
#define F8_BUFFER_TOO_SMALL		-15
#define F8_CONNECTION_LOST		-16
#define F8_NO_SCHEDULER			-17
#define F8_CONNECTION_REFUSED	-18
#define F8_VERSION_MISMATCH		-19
#define F8_NOT_A_CBLK			-20
#define F8_NOT_A_BBLK			-21
#define F8_ADD_BLOCK_FAILED		-22
#define F8_OBJECT_NOT_FOUND		-23
#define F8_INTERNAL_ERROR		-24
#define F8_ALREADY_ONLINE		-25
#define F8_ALREADY_OFFLINE		-26
#define F8_RES_NOT_FOUND		-27
#define F8_INVALID_NAME			-28
#define F8_PIN_INCOMPATIBLE		-29
#define F8_INVALID_ADDRESS		-30
#define F8_UNIMPLEMENTED		-31
#define F8_TYPE_MISMATCH		-32
#define F8_SIZE_MISMATCH		-33
#define F8_BOUND				-34
#define F8_UNBOUND				-35
#define F8_GENERAL_CONFIG_ERR	-36
#define F8_ADDRESS_OCCUPIED		-37
#define F8_INVALID_AUTOSCAN		-38
#define F8_X_PROG_NOT_FOUND		-39
#define F8_LOW_REGISTERS		-40
#define F8_UNKNOWN_COMMAND		-41
#define F8_ALREADY_STARTED		-42
#define F8_ALREADY_STOPPED		-43
#define F8_VAR_ALREADY_DEFINED	-44
#define F8_UNRESOLVED_REFERENCES	-45
#define F8_PENDING_REFERENCES	-46
#define F8_PIN_NOT_EXPORTABLE	-47
#define F8_PIN_NOT_BINDABLE		-48
#define F8_NO_SOCKET			-49
#define F8_NO_CONNECTION		-50
#define F8_CANCELLED			-51
#define F8_TARGET_INCOMPATIBLE	-52
#define F8_DATABASE_LOCKED -53
#define F8_OUT_OF_SYNCHRONIZATION -54
#define F8_COLLISION -55
#define F8_INVALID_COMPRESSED_DATA -56

typedef __i16	f8_status;
typedef __i8	f8_evtclass;
typedef __i8	f8_evtmodule;
typedef __i32	f8_event;

#define EVT_CLASS_NONE			0
#define EVT_CLASS_WARNING		-1
#define EVT_CLASS_ERROR			-2
#define EVT_CLASS_FATAL			-3

#define event_code(e)	((f8_status)((e) & 0xffff))
#define event_class(e)	((f8_evtclass)(((unsigned)(e) >> 16) & 0xff))
#define event_module(e)	((f8_evtmodule)(((unsigned)(e) >> 24) & 0xff))

#define F8_EVENT(module, cls, code)	(((module) << 24) | ((cls)<<16) | (code))

#define F8_FAILED(code)  (code & 0x8000)
#define F8_SUCCESSED(code) !(F8_FAILED(code))

#endif

