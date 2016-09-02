{
  F8 Field bus controller project
  the User Interface Module
  This is the header file.
}
unit f8;

interface
uses
  windows, rtkdefs, libcobj, libutils;
type
f8_bool = __bool;
f8_u8 = __u8;
f8_u16 = __u16;
f8_u32 = __u32;
// f8_u64 = __u64;
f8_i8 = __i8;
f8_i16 = __i16;
f8_i32 = __i32;
f8_i64 = __i64;
f8_status = __i16;
f8_byte = __u8;
f8_char = __i8;
f8_short = __i16;
f8_word = __u16;
f8_uint = __u32;
f8_dword = __u32;
f8_int = __i32;
f8_r4 = __r4;
f8_r8 = __r8;
f8_uuid = __i64;

ktime_t = f8_i64;
f8_event = f8_i32;

pf8_int = ^f8_int;
pf8_u8 = ^f8_u8;
pf8_u16 = ^f8_u16;
Pf8_u32 = ^f8_u32;
pf8_uuid = ^f8_uuid;

const
{ configuration items }
  F8_MAX_PATH = 256;
  MAX_BBLK_ITEMS = 256;
  F8_MAX_KEY_LENGTH = 128;
  KERN_STATUS_LENGTH = 64;
  F8_MAX_VAR_NAME_LEN = 32;

{error numbers }
  F8_SUCCESS = 0;
  F8_PATH_NOT_FOUND = -1;
  F8_NAME_DUPLICATE = -2;
  F8_LOW_MEMORY = -3;
  F8_INVALID_DATA = -4;
  F8_PERMISSION = -5;
  F8_INVALID_OPERATION = -6;
  F8_TOO_MANY_TIMERS = -7;
  F8_ACCESS = -8;
  F8_CONFIG = -9;
  F8_BUSY = -10;
  F8_TIMEOUT = -11;
  F8_SYNTAX = -12;
  F8_FILE_NOT_FOUND = -13;
  F8_ADAPTER_OFFLINE = -14;
  F8_BUFFER_TOO_SMALL = -15;
  F8_CONNECTION_LOST = -16;
  F8_NO_SCHEDULER = -17;
  F8_CONNECTION_REFUSED = -18;
  F8_VERSION_MISMATCH = -19;
  F8_NOT_A_CBLK = -20;
  F8_NOT_A_BBLK = -21;
  F8_ADD_BLOCK_FAILED = -22;
  F8_OBJECT_NOT_FOUND = -23;
  F8_INTERNAL_ERROR = -24;
  F8_ALREADY_ONLINE = -25;
  F8_ALREADY_OFFLINE = -26;
  F8_RES_NOT_FOUND = -27;
  F8_INVALID_NAME = -28;
  F8_PIN_INCOMPATIBLE = -29;
  F8_INVALID_ADDRESS = -30;
  F8_UNIMPLEMENTED = -31;
  F8_TYPE_MISMATCH = -32;
  F8_SIZE_MISMATCH = -33;
  F8_BOUND=-34;
  F8_UNBOUND=-35;
	F8_GENERAL_CONFIG_ERR=-36;
	F8_ADDRESS_OCCUPIED=-37;
  F8_INVALID_AUTOSCAN	=	-38;
  F8_X_PROG_NOT_FOUND	=	-39;
  F8_LOW_REGISTERS	=	-40;
  F8_UNKNOWN_COMMAND =		-41;
  F8_ALREADY_STARTED	=	-42;
  F8_ALREADY_STOPPED	 =	-43;
  F8_VAR_ALREADY_DEFINED = -44;
  F8_UNRESOLVED_REFERENCES = -45;
  F8_PENDING_REFERENCES = -46;
  F8_PIN_NOT_EXPORTABLE	= -47;
  F8_PIN_NOT_BINDABLE	=	-48;
  F8_NO_SOCKET = -49;
  F8_NO_CONNECTION=-50;
  F8_CANCELLED=-51;
  F8_TARGET_INCOMPATIBLE=-52;
  F8_DATABASE_LOCKED = -53;

  F8_INVALID_TIMER_ID = -256;
	F8_NO_MEMORY=-257;
	F8_NO_TIMER=-258;
	F8_INCONSISTENCY=-259;
	F8_FIRE_NON_EVENT=-260;
	F8_TIMER_TYPE=-261;

{ f8 binary specification constants }
  PIN_CLASS_EI 	= $65;  //'e';
  PIN_CLASS_EO	= $45;  // 'E';
  PIN_CLASS_DI	= $64;  //'d';
  PIN_CLASS_DO	= $44;  //'D';
  PIN_CLASS_EXP = $78;  //'x';

  PIN_T_EVENT = $45;  //'E';
  PIN_T_INTEGER = $49;  //'I';
  PIN_T_BOOL	=	$42;  //'B';
  PIN_T_FLOAT = $46;  //'F';
  PIN_T_TIMER = $54;  //'T';
  PIN_T_BYTE = $59; // 'Y';
  PIN_T_WORD =  $57; // 'W'
  PIN_T_DOUBLE = $44; // 'D'
  PIN_T_DATE = $41; // 'A'
  PIN_T_CHAR = $43; // 'C'
  PIN_T_SHORT = $53; // 'S'
  PIN_T_DWORD = $4c; // 'L'
  PIN_T_ARRAY = $56; // 'V'

  BBLK_MAGIC = $4242;	// 'BB' in little-endian
  CBLK_MAGIC = $4243; // 'CB' in little-endian
  BLK_MAGIC	= $424c;
  PIN_MAGIC	= $5049;
  LNK_MAGIC	= $4c4b;

  KERN_NUM_SECTIONS = 6;

  PIN_F_VALID = (1 SHL 0);
  PIN_F_HIDDEN = (1 SHL 1);
  PIN_F_INVERTED = (1 SHL 2);

  PIN_B_CONST = 0;
  PIN_B_MEMORY = 1;

  BLK_F_READONLY	= $1;
  BLK_F_AUTOSCAN  = $2;
  BLK_F_INPUT = $4;
  BLK_F_OUTPUT = $8;
  BLK_F_DISABLED = $10;

{F8 config protocol command enumerations}  
  F8_NULL_COMMAND	=		0;
  F8_MOUNT_BLOCK		=	1;
  F8_UMOUNT_BLOCK		=	2;
  F8_START_BLOCK		=	3;
  F8_STOP_BLOCK		=	4;
  F8_PAUSE_BLOCK		=	5;
  F8_RESUME_BLOCK	=		6;
  F8_GET_CONF			=	7;
  F8_PUT_CONF			=	8;

  F8_GET_VOLATILE	=		9;		//only valid between adapters;
  F8_PUT_VOLATILE	=		10;		//only valid between adapters;

  F8_LINK				=	11;
  F8_UNLINK			=	12;
  F8_RESET			=	13;		//deletealltasks;

  F8_EXPORT		=		14;
  F8_UNEXPORT	=			15;

  F8_MODIFY_PIN	=		16;
  F8_SET_PIN_VALUE	=	17;

  F8_GET_VERSION		=	18;
  F8_SET_VERSION	=	19;


  F8_SINGLE_STEP		 =	21;
  F8_CONTINUE			 =	22;
  F8_BREAK			 =	23;
  F8_BREAKPOINT	=		24;
  F8_ENABLE_BP		=	25;
  F8_DISABLE_BP	 =		26;
  F8_DEL_BREAK		 =	27;

  F8_GET_SYS_CONFIG =		28;
  F8_SET_SYS_CONFIG	=	29;

  F8_GET_LOG_BUF		=	30;
  F8_CLEAR_LOG_BUF	 =	31;

  F8_SET_INVERT		=	32;

  F8_SET_MEM_VALUE	=	33;
  F8_GET_MEM_VALUE	=	34;

  F8_GET_X_MEMORY		=	35;
  F8_SET_X_MEMORY		=	36;

  F8_GET_STATUS		 =	37;
  F8_SET_STATUS			= 38;

{F8 kernel constants}  
  BLK_ID_ANY  : f8_uuid = -1;
  BLK_ID_ROOT : f8_uuid = 1;
  BLK_ID_NONE : f8_uuid = 0;

  TIMER_T_ONESHOT = 1;
  TIMER_T_PERIODIC = 2;

  MAX_EVENTS = 64;
  MAX_TIMERS = 64;

  KERN_MEM_SIZE_0	= 8192;
  KERN_MEM_SIZE_1	= 8192;
  KERN_MEM_SIZE_2	= 8192;
  KERN_MEM_SIZE_3	= 8192;
  KERN_MEM_SIZE_4	= 8192;
  KERN_MEM_SIZE_5	= 8192;
  
type
{types and functions to manipulate the Control Executable file}
PBlk = ^IBlk;
PBBlk = ^IBBlk;
PCBlk = ^ICblk;
pblk_pin_t = ^blk_pin_t;
pcblk_link_t = ^cblk_link_t;
PBlk_vtable = ^IBlk_vtable;
pmem_addr_t = ^mem_addr_t;
pdyn_pin_t = ^dyn_pin_t;
pidata_t = ^idata_t;
pblk_hdr_t = ^blk_hdr_t;
pblk_func_t = ^blk_func_t;
Ppointer = ^pointer;

mem_addr_t = packed record
  addr : f8_u16;
  section : f8_u8;
  padding : f8_u8;
end;

blk_event_t = packed record
  invoke : pointer;
  peer : pblk_pin_t;
  interval : __i32;
  quantum : __i32;
end;

blk_exp_t = packed record
  llink : pblk_pin_t;
  real_pin : pblk_pin_t;
end;

pin_event_t = packed record
  interval : f8_uint;
  quantum  : f8_uint;
end;

pin_dyn_data_t = packed record
case integer of
0:( i8  : f8_i8);
1:( i16 : f8_i16);
2:( i32 : f8_i32);
3:( i64 : f8_i64);
4:( u8 : f8_u8);
5:( u16 : f8_u16);
6:( u32 : f8_u32);
7:( flt : f8_r4);
8:( dbl : f8_r8);
end;

dyn_pin_t = packed record
case integer of
0:(e : pin_event_t);
1:(d : pin_dyn_data_t);
end;

pin_tail_1 = packed record
case integer of
0:(
  link : TRIPLE_LIST_ENTRY;
);
1:(
  llink : pblk_pin_t;
  real_pin : pblk_pin_t;
);
end;

pin_tail_2 = packed record
case integer of
0:(invoke : f8_int);
1:(mem_addr : mem_addr_t);
end;

blk_pin_t = packed record
  _class : __u8;
  _type : __u8;
  flags : __u8;
  binding : __u8;
  blk : PBlk;
  ulink : pblk_pin_t;
  u1 : pin_tail_1;
  u2 : pin_tail_2;
  mdp   : pdyn_pin_t;
  dp : pdyn_pin_t;
end;

idata_t = packed record
  _type : f8_u8;
  basetype : f8_u8;
  offset : f8_u16;
  length : f8_u16;
end;

blk_func_t = packed record
  flags : f8_u16;
  length : f8_u16;
  address : f8_uint;
end;

blk_hdr_t = packed record
  magic : f8_u16;
  n_pins : f8_u16;
  funcs_or_subs : f8_u16;
  codes_or_links : f8_u16;
  flags : f8_u16;
  padding_0 : f8_u16;
  uuid : f8_uuid;
end;

bblk_hdr_t = packed record
  n_data : f8_u16;
  n_bss : f8_u16;
  // 2003/11/11, IEC-1131 fields(autoscan support)
  x_order : f8_int;
  x_link : LIST_ENTRY;
end;

// virtual function types for IBlk
vfunc_load = function (_this : PBlk; s: PStream) : boolean;cdecl;
vfunc_save = function (_this : PBlk; s: PStream) : boolean;cdecl;
vfunc_pin_by_id = function (_this : PBLk; name : Pchar) : pblk_pin_t;cdecl;
vfunc_id_by_pin = function (p : pblk_pin_t):__u16; cdecl;
vfunc_first_pin = function (_this : PBlk) : pblk_pin_t;cdecl;
vfunc_next_pin = function (p : pblk_pin_t) : pblk_pin_t;cdecl;

IBlk_vtable = packed record
  destroy : vfunc_destroy;
  __inherited_ITreeNode_vtable : ITreeNode_vtable;
  load : vfunc_load;
  save : vfunc_save;
  pin_by_id : vfunc_pin_by_id;
  id_by_pin : vfunc_id_by_pin;
  first_pin : vfunc_first_pin;
  next_pin : vfunc_next_pin;
end;

IBlk = packed record
  vtbl : PBlk_vtable;
  __inherited_ITreeNode : ITreeNode;
  h : blk_hdr_t;
  uuid : f8_uuid;
end;

IBBlk_vtable = packed record
  destroy : vfunc_destroy;
  __inherited_IBlk_vtable : IBlk_vtable;
end;

IBBlk = packed record
  vtbl : pointer;
	__inherited_IBlk : IBlk;

  bh : bblk_hdr_t;

  data_section : pchar;
  code_section : pchar;
  bss_section : pchar;
  init_data : pchar;

  pins : pblk_pin_t;
  dpins : pdyn_pin_t;
  funcs : pblk_func_t;
end;

cblk_link_t = packed record
  sibling : LIST_ENTRY;
  uuid : f8_uuid;
  s : pblk_pin_t;
	t : pblk_pin_t;
  blk : PCblk;
  tag : packed array [0..3] of __uint;
end;

Pcblk_pin_t = ^cblk_pin_t;
cblk_pin_t = packed record
	sibling : LIST_ENTRY;
	uuid : f8_uuid;
  p : blk_pin_t;
end;

ICBlk = packed record
  vtbl : pointer;
  __inherited_IBlk : IBlk;
  links : LIST_ENTRY;
  pins : LIST_ENTRY;
end;

Pkern_conf_cmd = ^kern_conf_cmd;
kern_conf_cmd = packed record
  vesion : f8_u16;
  flags : f8_u16;
  cmd : f8_u16;
  retcode : f8_status;
  uuids : array[0..1] of f8_uuid;
  isize, osize : f8_u32;
  ibuffer, obuffer : pointer;
  reserved : array[0..3] of f8_u32;
end;

const
  IO_SIZE_PER_SLOT	= 32;
  IO_MAX_DEVICES	 =	128;
  IO_MAX_CHANNELS	 =	32;
  IO_POOL_SIZE		 = (IO_SIZE_PER_SLOT*IO_MAX_DEVICES);

  IO_DEVICE_PRESENT	= $10;

  DEV_CONFIG_LEN = 24;
type

Pio_device=^io_device;

io_device = packed record
	clsid : f8_uuid;
  addr : f8_u16;
	features : f8_u16;
	revision : f8_u16;
  n_di : f8_u8;
  n_do : f8_u8;
  n_ai : f8_u8;
  ai_width : f8_u8;
  n_ao : f8_u8;
  ao_width : f8_u8;
  configData : array[0..DEV_CONFIG_LEN-1] of f8_u8;
end;

Pctrller_config = ^ctrller_config;
ctrller_config = packed record
  uuid : f8_uuid;
	size : f8_u16;
  deviceCount : f8_u16;
	cpuType : f8_u32;
  poolSize : f8_u32;
  x_mem_sizes : array[0..KERN_NUM_SECTIONS-1] of f8_u32;
  devices : array[0..IO_MAX_DEVICES-1] of io_device;
end;

const
  KERN_STATUS_STOPPED	 =	0;
  KERN_STATUS_RUNNING	 =	1;
  KERN_STATUS_BREAK	 =	2;
  KERN_STATUS_HALTED	=	3;

type
Pkernel_t = pointer;
Pkstatus_t = ^kstatus_t;
kstatus_t = packed record
  length : f8_u16;
  status : f8_u8;
  padding : f8_u8;
  flags : f8_u32;
  curr_time : ktime_t;
  prog_id : f8_uuid;
  timer_q_len : f8_u16;
  event_q_len : f8_u16;
  n_blocks : f8_u16;
  n_memory : f8_u16;
  n_cycleTime : f8_u16;

	n_phaseTimes : array[0..4] of f8_u16;
	upTime : ktime_t;
  n_cycles : f8_r4;
	reserved : array[0..127] of f8_u8;
end;

Pdevlib_ent_t = ^devlib_ent_t;
dev_config_proc_t = function(parentWnd : HWND;data : Pf8_u8; len:f8_uint;reserved:f8_uint):f8_bool;cdecl;
devlib_ent_t=packed record
	name : Pchar;
	description : Pchar;
  dev : io_device;
	configUiDll : Pointer;
	configProc : dev_config_proc_t;
end;

// IBlk apis
function IBlk_subblk(_this : PBlk; uuid : pf8_uuid):PBlk;cdecl;external 'libfbd.dll';
function IBlk_blk_by_id(_this : PBlk; id:__u16):PBlk;cdecl;external 'libfbd.dll';
function IBlk_id_by_blk(_this : PBlk):__u16;cdecl;external 'libfbd.dll';
function IBlk_level(b : Pblk):__uint;cdecl;external 'libfbd.dll';

function IBlk_linkable(s, t: pblk_pin_t):__bool;cdecl;external 'libfbd.dll';
function IBlk_exportable(s : pblk_pin_t):__bool;cdecl;external 'libfbd.dll';
function IBlk_real_pin(p:pblk_pin_t):pblk_pin_t;cdecl;external 'libfbd.dll';
function IBlk_peer(p : Pblk_pin_t):pblk_pin_t;cdecl;external 'libfbd.dll';
function IBlk_is_output(p : Pblk_pin_t):__bool;cdecl;external 'libfbd.dll';
function IBlk_is_multiple(p : Pblk_pin_t):__bool;cdecl;external 'libfbd.dll';
function IBlk_connectable(s : pblk_pin_t):__bool;cdecl;external 'libfbd.dll';

function delphi_parent(b : pblk):pblk;cdecl;external 'libfbd.dll';
function delphi_firstsub(b : pblk):pblk;cdecl;external 'libfbd.dll';
function delphi_next(b : pblk):pblk;cdecl;external 'libfbd.dll';

function ICBlk_first_link(blk : PCblk):pcblk_link_t;cdecl;external 'libfbd.dll';
function ICBlk_next_link(p : pcblk_link_t):pcblk_link_t;cdecl;external 'libfbd.dll';
function ICBlk_id_by_link(lk : pcblk_link_t):f8_u16;cdecl;external 'libfbd.dll';
function ICBlk_link_by_id(cb : Pcblk; id : f8_u16):pcblk_link_t;cdecl;external 'libfbd.dll';

function ICBlk_first_connection(p : pblk_pin_t):pcblk_link_t;cdecl;external 'libfbd.dll';
function ICBlk_next_connection(p : pblk_pin_t;l : pcblk_link_t):pcblk_link_t;cdecl;external 'libfbd.dll';

procedure delete_blk(blk : PBlk);cdecl;external 'libfbd.dll';

(*
  Loader support routines
*)
function IBlk_subblk_by_name(peer : PBlk; name : pchar):PBlk;cdecl;external 'libfbd.dll';
function IBlk_pin_by_path(cur : PBlk; path : pchar):pblk_pin_t;cdecl;external 'libfbd.dll';
function IBlk_blk_by_path(cur : PBlk; path : pchar):PBlk;cdecl;external 'libfbd.dll';
function IBlk_pin_name(p : pblk_pin_t):pchar;cdecl;external 'libfbd.dll';
function IBlk_pin_by_name(blk : PBlk;name : pchar):pblk_pin_t;cdecl;external 'libfbd.dll';
function IBlk_path_by_pin(p : pblk_pin_t;buf:pchar;len:f8_int):pchar;cdecl;external 'libfbd.dll';
function IBlk_class_name(blk : PBlk):pchar;cdecl;external 'libfbd.dll';
function IBlk_name(blk : PBlk):pchar;cdecl;external 'libfbd.dll';
function IBlk_path_by_blk(blk : PBlk;buf:pchar;len:f8_int):pchar;cdecl;external 'libfbd.dll';
function IBlk_load_resource(blk : PBLk):f8_bool;cdecl;external 'libfbd.dll';
function IBlk_unload_resource(blk : PBLk):f8_bool;cdecl;external 'libfbd.dll';
function IBlk_get_data(blk : Pblk; name : pchar;len : Pf8_int):pchar;cdecl;external 'libfbd.dll';
function IBlk_set_data(blk : Pblk; name : pchar;buf : pointer;len : f8_int):f8_bool;cdecl;external 'libfbd.dll';

procedure ke_init;cdecl;external 'libfkern.dll';
procedure ke_uninit;cdecl;external 'libfkern.dll';
function is_address_valid(k : Pkernel_t; p : Pblk_pin_t; addr : Pmem_addr_t):boolean;cdecl;external 'libfkern.dll';
function ke_config(k : Pkernel_t; cmd : Pkern_conf_cmd; isConfigurator : __bool):f8_status;cdecl;external 'libfkern.dll';
procedure ke_lock(k : Pkernel_t);cdecl;external 'libfkern.dll';
procedure ke_unlock(k : Pkernel_t);cdecl;external 'libfkern.dll';
function ke_get_blk(k : Pkernel_t; id : pf8_uuid):PBlk;cdecl;external 'libfkern.dll';
function ke_alloc_kernel:Pkernel_t;cdecl;external 'libfkern.dll';
procedure ke_free_kernel(k:Pkernel_t);cdecl;external 'libfkern.dll';
function ke_get_default_kernel:Pkernel_t;cdecl;external 'libfkern.dll';
function ex_start(flags : f8_int):f8_status;cdecl;external 'libfkern.dll';
function ex_stop():f8_status;cdecl;external 'libfkern.dll';
function ke_get_type_size(t : f8_byte):f8_u16;cdecl;external 'libfkern.dll';
function ke_get_x_memory(k : Pkernel_t):Pf8_u8;cdecl;external 'libfkern.dll';
function ke_get_section(k : Pkernel_t; sec : f8_u8):Pf8_u8;cdecl;external 'libfkern.dll';
function ke_get_section_size(k:Pkernel_t; sec : f8_u8):f8_uint;cdecl;external 'libfkern.dll';
function ke_get_io_memory(k : Pkernel_t; d : f8_u16):Pf8_u8;cdecl;external 'libfkern.dll';
function ke_get_flag(idx:integer):f8_bool;cdecl;external 'libfkern.dll';
function ke_set_flag(idx:integer;bit:f8_bool):f8_u8;cdecl;external 'libfkern.dll';
function ke_toggle_flag(idx:integer):f8_bool;cdecl;external 'libfkern.dll';
function ke_get_device_list(k : Pkernel_t):Pio_device;cdecl;external 'libfkern.dll';
procedure ke_get_mem_sizes(k : Pkernel_t;sizes : array of f8_u32);cdecl;external 'libfkern.dll';
function ke_time_to_string(kt : ktime_t;buf:pchar;len:integer):integer;cdecl;external 'libfkern.dll';
function ke_reltime_to_string(kt : ktime_t;buf:pchar;len:integer):integer;cdecl;external 'libfkern.dll';
function ke_iotime_to_ktime(iotime : double):ktime_t;cdecl;external 'libfkern.dll';
// function ke_get_hw_memory(_kernel : Pkernel_t; device : f8_u16):Pf8_u8;cdecl;external 'libfkern.dll';

type
Phash_val_t = ^hash_val_t;
hash_val_t = pointer;
Phash_key_t = ^hash_key_t;
hash_key_t = f8_uuid;

function ke_init_hash:pointer;cdecl;external 'libfkern.dll';
procedure ke_uninit_hash(buckets : pointer);cdecl;external 'libfkern.dll';
function ke_set_hash(h : pointer; t : Phash_key_t; v : hash_val_t):__bool;cdecl;external 'libfkern.dll';
function ke_get_hash(h : pointer; t : Phash_key_t):hash_val_t;cdecl;external 'libfkern.dll';
function ke_del_hash(h : pointer; t : Phash_key_t):__bool;cdecl;external 'libfkern.dll';
procedure ke_clear_hash(buckets : pointer);cdecl;external 'libfkern.dll';

const
  ADAPTER_F_ONLINE	= 1 shl 0;
  ADAPTER_F_BOUND	= 1 shl 1;

  AE_STATUS_CHANGED	= -1;
  AE_IN_PROGRESS = -2;
  AE_CONNECTION_LOST = -3;

type
PKAdapter = ^IKAdapter;
KAdapterEventProc	= procedure(a : PKAdapter; event, p0, p1 : f8_int);cdecl;
vfunc_online = function(_this : PKAdapter; url : pchar; timeout : __uint):f8_status;cdecl;
vfunc_offline = function(_this : PKAdapter):f8_status;cdecl;
vfunc_exec = procedure(_this : PKadapter; cmd : Pkern_conf_cmd; timeout : __uint);cdecl;
vfunc_exec_local = procedure(_this : PKadapter; cmd : Pkern_conf_cmd);cdecl;
vfunc_exec_remote = procedure(_this : PKadapter; cmd : Pkern_conf_cmd; timeout : __uint);cdecl;
vfunc_exec0 = procedure(_this : PKadapter; cmd : __u16; timeout : __uint);cdecl;
PKadapter_vtbl = ^IKAdapter_vtbl;
IKAdapter_vtbl = packed record
  destroy : vfunc_destroy;
  online : vfunc_online;
  offline : vfunc_offline;
  exec : vfunc_exec;
  exec_local : vfunc_exec_local;
  exec_remote : vfunc_exec_remote;
  exec0 : vfunc_exec0;
end;

IKAdapter = packed record
  vtbl : PKAdapter_vtbl;
	_type : f8_uint;
	flags : integer;
	eventSink : KAdapterEventProc;
	kernel : Pkernel_t;
  hImportPinThread : Pointer;
  reserved : array[0..10] of f8_uint;
end;

function kproxy_init(leakDetect : integer):__bool;cdecl;external 'libkproxy.dll';
procedure kproxy_uninit;cdecl;external 'libkproxy.dll';
function kproxy_get_kernel:Pkernel_t;cdecl;external 'libkproxy.dll';
function kproxy_shell(cmd : pchar):f8_status;cdecl;external 'libkproxy.dll';
function kproxy_shell_ex(cmd : pchar;timeout : integer;reserved : integer):f8_status;cdecl;external 'libkproxy.dll';
function kproxy_get_adapter:PKAdapter;cdecl;external 'libkproxy.dll';
procedure kproxy_get_mem_usage(sizes : array of f8_u32);cdecl;external 'libkproxy.dll';

type
Ppasted_obj_t = ^pasted_obj_t;
pasted_obj_t = packed record
  obj : pointer;
  magic : f8_u16;
end;
function kproxy_copy(sid : Pf8_uuid; magics : Pf8_u16;var pp : pointer; count : integer):integer;cdecl;external 'libkproxy.dll';
function kproxy_paste(target : PBlk):pointer;cdecl;external 'libkproxy.dll';
procedure kproxy_delete_paste(h : pointer);cdecl;external 'libkproxy.dll';
function kproxy_get_pasted_object(h : pointer; i : integer):Ppasted_obj_t;cdecl;external 'libkproxy.dll';
function kproxy_get_clipboard_format:f8_u16;cdecl;external 'libkproxy.dll';

function blklib_get_dev_by_id(id : Pf8_uuid):Pdevlib_ent_t;cdecl;external 'libfbdar.dll';
function blklib_get_dev_by_name(name : pchar):Pdevlib_ent_t;cdecl;external 'libfbdar.dll';
procedure init_blklib;cdecl;external 'libfbdar.dll';
procedure uninit_blklib;cdecl;external 'libfbdar.dll';
function blklib_load(_type : pchar; var blk : pblk; reserved : integer):integer;
  cdecl;external 'libfbdar.dll';
function blklib_save(_type : pchar; blk : pblk; reserved : integer):integer;
  cdecl;external 'libfbdar.dll';
procedure reload_blklibs;cdecl;external 'libfbdar.dll';
procedure set_blklib_project_path(path : pchar);cdecl;external 'libfbdar.dll';
procedure probe_blk_resources(blk : Pblk);cdecl;external 'libfbdar.dll';

type
ENUM_AVAILABLE_DEV_PROC = function (dev : Pdevlib_ent_t; con : f8_uint):f8_bool;stdcall;
procedure blklib_enum_available_devices(p : ENUM_AVAILABLE_DEV_PROC; con : f8_uint);cdecl;external 'libfbdar.dll';

type
  Pblk_var_t = ^blk_var_t;
  blk_var_t = packed record
    magic : f8_u16;
    size : f8_u16;
    blk : Pblk;
    padding_1 : f8_dword;
    _type : f8_byte;
    padding_0 : array[0..2] of f8_byte;
    name : array[0..F8_MAX_VAR_NAME_LEN-1] of char;
    uuid : f8_uuid;
    ref_count : f8_int;
    addr : mem_addr_t;
    comment : pchar;
    initVal : dyn_pin_t;
    reserved : array [0..11] of f8_u8;
  end;

  ENUM_VAR_PROC = function(v : Pblk_var_t; context : f8_uint):__bool;cdecl;
  
function query_var_list(
	blk : PBlk;
	var buffer : blk_var_t;
	var items : f8_uint
	):f8_uint;cdecl;external 'libkproxy.dll';

function query_var(
	blk : Pblk;
	name : pchar
  ): Pblk_var_t;cdecl;external 'libkproxy.dll';

function query_var_by_pin(
	p : Pblk_pin_t
	) : Pblk_var_t;cdecl;external 'libkproxy.dll';

function enum_variables(
	blk : PBlk;
	p : ENUM_VAR_PROC;
	context : f8_uint
	):f8_uint;cdecl;external 'libkproxy.dll';

type
  Pblk_dir_t = ^blk_dir_t;
  blk_dir_t = packed record
    name : pchar;
  end;
  Pblk_ent_t = ^blk_ent_t;
  blk_ent_t = packed record
    magic : f8_u16;
    pad : f8_u16;
    name : pchar;
    fileName : pchar;
    dir : Pchar;
  end;
  ENUM_BLKDIR_PROC = function(ent : Pblk_dir_t;context : __uint):__bool;cdecl;
  ENUM_BLKENT_PROC = function(ent : Pblk_ent_t;context : __uint):__bool;cdecl;

procedure set_blklib_paths(paths : pchar;bReload : boolean);
  cdecl;external 'libfbdar.dll';
function get_blklib_paths():pchar;
  cdecl;external 'libfbdar.dll';
procedure enumerate_blk_dir(
	proc : ENUM_BLKDIR_PROC; context : __uint
	);cdecl;external 'libfbdar.dll';
procedure enumerate_blk_ent(
	proc : ENUM_BLKENT_PROC;
	dirName : pchar;
	context : __uint
	);cdecl;external 'libfbdar.dll';
function blklib_query(
	id : Pf8_uuid;
  ent : Pblk_ent_t
	):__bool;cdecl;external 'libfbdar.dll';

function is_valid_blk_name(name : pchar) : boolean;
  cdecl;external 'libfbdar.dll';
function is_valid_pin_name(name : pchar) : boolean;
  cdecl;external 'libfbdar.dll';
function blk_from_file(f : pchar):pblk;cdecl;external 'libfbdar.dll';
function blk_to_file(b : PBlk;f : pchar):f8_bool;cdecl;external 'libfbdar.dll';

function kserver_main(cmdline : pchar):f8_status;cdecl;external 'libkserver.dll';
procedure kserver_trivial;cdecl;external 'libkserver.dll';
function start_kserver:boolean;cdecl;external 'libkserver.dll';
function stop_kserver:boolean;cdecl;external 'libkserver.dll';

procedure create_f8_uuid(id : pf8_uuid);cdecl; external 'libf8res.dll';

procedure f8_uuid_to_string(
	id : pf8_uuid;
	buf : pchar;
	length : f8_int
	);cdecl; external 'libf8res.dll';

procedure f8_uuid_from_string(id:pf8_uuid; buf : pchar);
  cdecl;external 'libf8res.dll';

function load_resource(id : pf8_uuid):f8_bool;
  cdecl;external 'libf8res.dll';

function load_res_stream(
	s : PStream
	):f8_bool;cdecl;external 'libf8res.dll';

function save_res_stream(
	id : pf8_uuid;
	s : PStream
	):f8_bool;cdecl;external 'libf8res.dll';

function save_volatile_res(
	s : PStream
	):f8_bool;cdecl;external 'libf8res.dll';

procedure reset_resources();
  cdecl;external 'libf8res.dll';

function unload_resource(
	id : pf8_uuid
	):f8_bool;cdecl;external 'libf8res.dll';

function create_resource(
	id : pf8_uuid
	):f8_bool;cdecl;external 'libf8res.dll';

function del_resource(
	id : pf8_uuid
	):f8_bool;cdecl;external 'libf8res.dll';

function set_res_buf(
	id : pf8_uuid;
	buf : pointer;
	length : f8_int;
	key : pchar
	):f8_bool;cdecl;external 'libf8res.dll';

function set_res_name(
	id : pf8_uuid;
  name : pchar
	):f8_bool;cdecl;external 'libf8res.dll';

function get_res_name(
	id : pf8_uuid
	):pchar;cdecl;external 'libf8res.dll';

function get_res_buf(
	id : pf8_uuid;
	length : pf8_int;
	key : pchar
	):pchar;cdecl;external 'libf8res.dll';

function del_res_item(
	id : pf8_uuid;
	key : pchar
	):f8_bool;cdecl;external 'libf8res.dll';

function get_res_int(
	id : pf8_uuid;
	key : pchar;
	defval : f8_int
	):f8_int;cdecl;external 'libf8res.dll';

function set_res_int(
	id : pf8_uuid;
	key : pchar;
	val : f8_int
	):f8_bool;cdecl;external 'libf8res.dll';

function query_resource(id : Pf8_uuid):f8_int;cdecl;external 'libf8res.dll';

type

_ERES_PROC = function(
	id : pf8_uuid;
	context : f8_int
	):f8_bool;cdecl;

_EITEM_PROC = function(
	key : pchar;
	buf : pointer;
	length : f8_int;
	context : f8_int
	):f8_bool;cdecl;

function enum_resources(
	p : _ERES_PROC;
	context : f8_int
	):f8_int;cdecl;external 'libf8res.dll';

function enum_res_items(
  id : pf8_uuid;
  p : _EITEM_PROC;
  context : f8_int
  ):f8_int;cdecl;external 'libf8res.dll';

function GetBlkPos(blk : PBlk):TPoint;
procedure SetBlkPos(blk : PBlk; p:TPoint);

function GetBlkPath(b : pblk):string;

function Blk2Cblk(b : PBlk):PCblk;
function Bblk2Blk(b : PBBlk):Pblk;
function Cblk2Blk(b : PCBlk):Pblk;
function Blk2TreeNode(b : PBlk):PTreeNode;
function TreeNode2Blk(p : PTreeNode):PBlk;

function F8UuidToString(id : f8_uuid):string;

function GetPinPath(p : Pblk_pin_t):string;
function GETPinName(p : pblk_pin_t):string;

function FormatF8Error(code : f8_status):string;
function getLocalStatus(var st : kstatus_t):f8_status;
function getRemoteStatus(var st : kstatus_t):f8_status;

function UniqueName(blk : PBlk;tp :string):string;
function _TypeName(t : f8_byte):string;

function AddressStrByPin(addr : mem_addr_t):string;
function valueByDpin(tp : f8_u8;dp : Pdyn_pin_t;inverted:boolean):string;
function ValueByPin(p : pblk_pin_t):string;
function valueByVar(k : Pkernel_t; v : Pblk_var_t) : string;
function GetBindingStr(rp : Pblk_pin_t): string;
function Pin2Cpin(p : Pblk_pin_t):Pcblk_pin_t;

procedure initKProxy;

var
  g_sections : PBlk;
  g_kernel : Pkernel_t;
  g_adapter : PKAdapter;

implementation
uses
  forms,
  sysutils;
  
var
  BblkOffset, CblkOffset, BlkTreeNodeOffset : Integer;

function GetBlkIdAndPrefix(blk : PBlk;var id:f8_uuid; var pfix : string):boolean;
var
  pb : PBlk;
  bid : f8_u16;
begin
  result := false;
  pb := delphi_parent(blk);
  if not assigned(pb) then
    Exit;
  if ((pb.h.flags and BLK_F_READONLY)<>0) then
  begin
    id := pb.h.uuid;
    bid := IBlk_id_by_blk(blk);
    pfix := string('!b') + format('%d', [bid]) + '!';
  end else begin
    id := blk.uuid;
  end;
  result := true;
end;

function GetBlkPos(blk : PBlk):TPoint;
var
  id : f8_uuid;
  p : TPoint;
  k : string;
begin
  result.x := 0;
  result.y := 0;
  if not getblkidandprefix(blk, id, k) then
    Exit;
  p.X := get_res_int(@id, pchar(k + 'x'), 0);
  p.y := get_res_int(@id, pchar(k + 'y'), 0);
  result := p;
end;

procedure SetBlkPos(blk : PBlk; p:TPoint);
var
  id : f8_uuid;
  k : string;
begin
  getblkidandprefix(blk, id, k);
  set_res_int(@id, pchar(k + 'x'), p.x);
  set_res_int(@id, pchar(k + 'y'), p.y);
end;

function GetBlkPath(b : pblk):string;
var
  path : array[0..F8_MAX_PATH-1] of char;
begin
  IBlk_path_by_blk(b, path, F8_MAX_PATH);
  result := path;
end;

function GETPinName(p : pblk_pin_t):string;
begin
  result := Iblk_pin_name(p);
end;

function GetPinPath(p : Pblk_pin_t):string;
var
  path : array[0..F8_MAX_PATH-1] of char;
begin
  Iblk_path_by_pin(p, path, sizeof(path));
  result := path;
end;

function Blk2Cblk(b : PBlk):PCblk;
begin
  Result := PCblk(PChar(b) - CblkOffset);
end;

function Blk2Bblk(b : PBlk):PBblk;
begin
  Result := PBblk(PChar(b) - BblkOffset);
end;

function Bblk2Blk(b : PBBlk):Pblk;
begin
  Result := @b.__inherited_IBlk;
end;

function Cblk2Blk(b : PCBlk):Pblk;
begin
  Result := @b.__inherited_IBlk;
end;

function Blk2TreeNode(b : PBlk):PTreeNode;
begin
  Result := @b.__inherited_ITreeNode;
end;

function TreeNode2Blk(p : PTreeNode):PBlk;
begin
  Result := PBlk(Pchar(p) - BlkTreeNodeOffset);
end;

procedure InitOffsets;
var
  cb : ICblk;
  bb : IBBlk;
  b : IBlk;
begin
  CblkOffset := Pchar(@cb.__inherited_IBlk) - Pchar(@cb);
  BblkOffset := Pchar(@bb.__inherited_IBlk) - pchar(@bb);
  BlkTreeNodeOffset := Pchar(@b.__inherited_ITreeNode) - pchar(@b);
end;

function F8UuidToString(id : f8_uuid):string;
var
  p16 : pf8_u16;
  buf : array [0..1023] of char;
  a,b,c,d:f8_u32;
  fmt : pchar;
  cbuf : pchar;
begin
  p16 := pf8_u16(@id);
  a := p16^;
  inc(p16);
  b := p16^;
  inc(p16);
  c := p16^;
  inc(p16);
  d := p16^;
  fmt := '{%04x-%04x-%04x-%04x}';
  cbuf := @buf[0];
  // wsprintf(@buf[0], '{%04x-%04x-%04x-%04x}', p16^, (p16+1)^, (p16+2)^, (p16+3)^);
  asm
    push d
    push c
    push b
    push a
    push fmt
    push cbuf
    call wsprintf
    add esp, 24
  end;
  Result := buf;
end;

function FormatF8Error(code : f8_status):string;
var
  s : string;
begin
	case code of
	F8_PATH_NOT_FOUND:
  begin
    s := '没有找到指定的路径';
  end;
	F8_NAME_DUPLICATE:
  begin
    s := '名字冲突';
  end;
	F8_LOW_MEMORY:
  begin
    s := '内存不足以完成指定的操作';
  end;
	F8_INVALID_DATA:
  begin
    s := '提供了无效的输入数据';
  end;
	F8_PERMISSION:
  begin
    s := '对对象的访问被拒绝';
  end;
	F8_INVALID_OPERATION:
  begin
    s := '对象不支持指定的操作';
  end;
	F8_TOO_MANY_TIMERS:
  begin
    s := '无法分配足够的定时器';
  end;
	F8_ACCESS:
  begin
    s := '对对象的访问被拒绝';
  end;
	F8_CONFIG:
  begin
    s := '配置错误';
  end;
	F8_BUSY:
  begin
    s := '指定的对象暂时无法访问';
  end;
	F8_TIMEOUT:
  begin
    s := '超时错误';
  end;
	F8_SYNTAX:
  begin
    s := '命令行语法错误';
  end;
	F8_FILE_NOT_FOUND:
  begin
    s := '指定的文件没有找到';
  end;
	F8_ADAPTER_OFFLINE:
  begin
    s := '适配器处于离线状态';
  end;
	F8_BUFFER_TOO_SMALL:
  begin
    s := '提供的缓冲区太小，不足以完成指定操作';
  end;
	F8_CONNECTION_LOST:
  begin
    s := '到目标系统的连接丢失';
  end;
	F8_NO_SCHEDULER:
  begin
    s := '没有调度程序';
  end;
	F8_CONNECTION_REFUSED:
  begin
    s := '到目标系统的连接被拒绝';
  end;
	F8_VERSION_MISMATCH:
  begin
    s := '本地、目标系统的程序或者配置版本不匹配';
  end;
	F8_NOT_A_BBLK:
  begin
    s := '指定的功能块不是一个基本功能块';
  end;
	F8_NOT_A_CBLK:
  begin
    s := '指定的功能块不是一个组合功能块';
  end;
	F8_ADD_BLOCK_FAILED:
  begin
    s := '插入功能块失败';
  end;
	F8_OBJECT_NOT_FOUND:
  begin
    s := '指定的对象没有找到';
  end;
	F8_INTERNAL_ERROR:
  begin
    s := '内部错误';
  end;
	F8_ALREADY_ONLINE:
  begin
    s := '已处于在线状态';
  end;
	F8_ALREADY_OFFLINE:
  begin
    s := '已处于离线状态';
  end;
	F8_RES_NOT_FOUND:
  begin
    s := '没有找到相关的资源';
  end;
	F8_INVALID_NAME:
  begin
    s := '名字非法';
  end;
	F8_PIN_INCOMPATIBLE:
  begin
    s := '引脚类型不匹配';
  end;
	F8_INVALID_ADDRESS:
  begin
    s := '指定的地址非法';
  end;
	F8_UNIMPLEMENTED:
  begin
    s := '不支持请求的功能';
  end;
	F8_TYPE_MISMATCH:
  begin
    s := '对象的类型不匹配';
  end;
	F8_SIZE_MISMATCH:
  begin
    s := '对象的大小不匹配';
  end;
	F8_INVALID_TIMER_ID:
  begin
    s := '错误的定时器ID';
  end;
	F8_NO_MEMORY:
  begin
    s := '内存不足';
  end;
	F8_NO_TIMER:
  begin
    s := '没有足够数量的定时器';
  end;
	F8_INCONSISTENCY:
  begin
    s := '一致性错误';
  end;
	F8_FIRE_NON_EVENT:
  begin
    s := '请求调度非事件引脚';
  end;
	F8_TIMER_TYPE:
  begin
    s := '定时器类型错误';
  end;
	F8_GENERAL_CONFIG_ERR:
  begin
    s := '通用配置错误';
  end;
	F8_ADDRESS_OCCUPIED:
  begin
    s := '指定的地址已被占用';
  end;
  F8_INVALID_AUTOSCAN:
  begin
    s := '功能块不能被指定成自动扫描';
  end;
  F8_X_PROG_NOT_FOUND:
  begin
    s := '外部程序没有找到.';
  end;
  F8_LOW_REGISTERS:
  begin
    s := '寄存器数量不足';
  end;
  F8_UNKNOWN_COMMAND:
  begin
    s := '未知的配置命令';
  end;
  F8_ALREADY_STARTED:
  begin
    s := '控制器已经启动';
  end;
  F8_ALREADY_STOPPED:
  begin
    s := '控制器已停止';
  end;
  F8_VAR_ALREADY_DEFINED:
  begin
    s := '变量已经被定义';
  end;
  F8_UNRESOLVED_REFERENCES:
  begin
    s := '存在没有解决的外部引用.';
  end;
  F8_PENDING_REFERENCES:
  begin
    s := '存在到变量的引用的情况下不能执行当前操作';
  end;
  F8_PIN_NOT_EXPORTABLE:
  begin
    s := '当前引脚不能被导出';
  end;
  F8_PIN_NOT_BINDABLE:
  begin
    s := '不能修改当前引脚的绑定状态';
  end;

  F8_NO_SOCKET:
  begin
    s := '不能创建Socket';
  end;

  F8_NO_CONNECTION:
  begin
    s := '连接超时';
  end;

  F8_DATABASE_LOCKED:
    s := '控制器被锁定，请检查钥匙位置，并检查是否有其他conductor程序正在组态。';

  F8_TARGET_INCOMPATIBLE:
    s := 'F8系统版本不匹配';

	end;  // case ... of
  s := Format('操作错误:%d,', [code]) + s;
  Result := s;
end;

function getRemoteStatus(var st : kstatus_t):f8_status;
var
  cmd : kern_conf_cmd;
begin
  zeroMemory(@cmd, sizeof(cmd));
	cmd.cmd := F8_GET_STATUS;
	cmd.obuffer := @st;
	cmd.osize := sizeof(st);
	g_adapter.vtbl.exec_remote(g_adapter, @cmd, 1000);
  result := cmd.retcode;
end;

function getLocalStatus(var st : kstatus_t):f8_status;
var
  cmd : kern_conf_cmd;
begin
  zeroMemory(@cmd, sizeof(cmd));
	cmd.cmd := F8_GET_STATUS;
	cmd.obuffer := @st;
	cmd.osize := sizeof(st);
	g_adapter.vtbl.exec_local(g_adapter, @cmd);
  result := cmd.retcode;
end;

(*
  make a uniqueue block name
*)
function UniqueName(blk : PBlk;tp : string):string;
var
  n : integer;
  nm : string;
  sb : PBlk;
begin
  //n := blk.h.funcs_or_subs+1;
  n:=1;
  if tp='' then
    tp := 'B';
  nm := tp + IntToStr(n);
  sb := IBlk_subblk_by_name(blk, pchar(nm));
  while assigned(sb) do
  begin
    Inc(n);
    nm := tp + IntToStr(n);
    sb := IBlk_subblk_by_name(blk, pchar(nm));
  end;
  Result := nm;
end;

function _TypeName(t : f8_byte):string;
begin
  case t of
  pin_t_bool:
    result := '布尔型';
  pin_t_byte:
    result := '字节型';
  pin_t_word:
    result := '无符号短整型';
  pin_t_dword:
    result := '无符号整型';
  pin_t_char:
    result := '字符型';
  pin_t_short:
    result := '短整型';
  pin_t_integer:
    result := '整型';
  pin_t_float:
    result := '短浮点型';
  pin_t_double:
    result := '长浮点型';
  pin_t_date:
    result := '日期型';
  else
    result := '任意型';
  end;
end;

function AddressStrByPin(addr : mem_addr_t):string;
var
  h : string;
  k : string;
begin
  h := '0000000000';
  result := '%'+inttostr(addr.section);
  k := inttostr(addr.addr);
  k := copy(h, 1, 5 - length(k)) + k;
  result := result + k;
end;

function valueByDpin(tp : f8_u8;dp : Pdyn_pin_t;inverted:boolean):string;
var
  bVal : f8_u8;
  kt : ktime_t;
  st : systemtime;
  tbuf : array[0..63] of char;
begin
  result := 'N.A.';
  case tp of
  pin_t_bool:
  begin
    if dp.d.i8<>0 then
      bval := 1
    else
      bval := 0;
    if inverted then
      bval := 1-bval;
    result := intToStr(bval);
  end;
  pin_t_integer:
    result := format('%d', [dp.d.i32]);
  pin_t_short:
    result := format('%d', [dp.d.i16]);
  pin_t_char:
    result := format('%d', [dp.d.i8]);
  pin_t_byte:
    result := format('%d', [dp.d.u8]);
  pin_t_word:
    result := format('%d', [dp.d.u16]);
  pin_t_dword:
    result := format('%d', [dp.d.u32]);
  pin_t_float:
    result := format('%.5g', [dp.d.flt]);
  pin_t_double:
    result := format('%.5g', [dp.d.dbl]);
  pin_t_date:
  {11644473600 is seconds from 1601 to 1900}
    try
    if dp.d.dbl>11644473600.0 then begin
      kt := ke_iotime_to_ktime(dp.d.dbl);
      ke_time_to_string(kt,@tbuf[0],sizeof(tbuf));
      result := tbuf;
    end else if dp.d.dbl<2.001 then
      result := 't#' + format('%f', [dp.d.dbl*1000]) + 'ms'
    elSE IF dp.d.dbl>=2.001 then
      result := 't#' + format('%f', [dp.d.dbl]) + 's'
    else
      result := 't#NA';
    except
      result := 'NAN';
    end;
  end;
end;

function ValueByPin(p : pblk_pin_t):string;
begin
  result := valueByDpin(p^._type,p^.dp,(p.flags and PIN_F_INVERTED)<>0);
end;

function valueByVar(k : Pkernel_t; v : Pblk_var_t) : string;
var
  dp : Pdyn_pin_t;
  p : Pf8_u8;
begin
  p := ke_get_section(k, v.addr.section);
  Inc(p, v.addr.addr);
  dp := Pdyn_pin_t(p);
  result := valueByDpin(v._type,dp,false);
end;

function GetBindingStr(rp : Pblk_pin_t): string;
var
  v : Pblk_var_t;
begin
  Result := '';
  case rp.binding of
  PIN_B_CONST:
    result := valuebypin(rp);
  PIN_B_MEMORY:
  begin
    v := query_var_by_pin(rp);
    if assigned(v) then
      result := v.name;
  end;
  end;
end;

function Pin2Cpin(p : Pblk_pin_t):Pcblk_pin_t;
var
  pp : Pcblk_pin_t;
begin
  pp := nil;
  Result := Pcblk_pin_t(Integer(p) - Integer(@pp.p));
end;

procedure initKProxy;
begin
  kproxy_init(-1);
  g_kernel := kproxy_get_kernel;
  g_adapter := kproxy_get_adapter;
  g_sections := ke_get_blk(g_kernel, nil);
end;

initialization
InitOffsets;

end.
