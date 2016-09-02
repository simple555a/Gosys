unit librtdb;

interface
uses
  librtk,
  rtkdefs;
  
const
//tag's general flag (TF_ constants)
TF_HiHi	= $00000001 shl 8;
TF_Hi	 = $00000001 shl 9;
TF_Lo					=$00000001 shl 10;
TF_LoLo					=$00000001 shl 11;
TF_On2Off				=$00000001 shl 12;
TF_Off2On				=$00000001 shl 13;
TF_Rate					=$00000001 shl 14;
TF_Alarm				=$00000001 shl 15;
TF_Valid				=$00000001 shl 16;
TF_Expired = $00000001 shl 17;
TF_SharpChange			=$00000001 shl 18;	//
TF_ReadOnly = $00000001 shl 19;
TF_Translated = $00000001 shl 20;
TF_SaveToHistory = $00000001 shl 21;
TF_ConfigOk = $00000001 shl 22;
TSF_System				=$00000001 shl 23;

// tag types
dt_null = 0;
dt_bool = 1;
dt_real4 = 2;
dt_int32 = 3;	
dt_real8 = 4;
dt_date = 5;
	
dt_int8 = 6;
dt_int16 = 7;
dt_int64 = 8;
	
dt_uint8 = 9;
dt_uint16 = 10;
dt_uint32 = 11;
dt_uint64 = 12;

dt_string = 13;
dt_bstr = 14;

dt_MaxType = 15;

// alarm masks;
AC_Alarm	=	0;
AC_AckAlarm	=	1;
AC_CancelAlarm =	2;
AC_WriteDevice =	3;
AC_Online	 =	4;
AC_Offline	=	5;

AT_NoAlarm	 = 0;
AT_HiHi			=	1;
AT_Hi			=	2;
AT_Lo			=	3;
AT_LoLo		 =		4;
AT_On2Off		=	5;
AT_Off2On		 =	6;
AT_VConflict	=	7;
AT_DFault		 =	8;
AT_SharpChange	=	9;
AT_Asynchronized =	10;
AT_SystemUpdate	 =	11;
AT_WriteDevice	 =	12;

GF_Dirty	= ($00000001 shl 0);
GF_System	= ($00000001 shl 1);

rtkm_node_key_length = 16;
rtkm_group_key_length = 16;
rtkm_tag_key_length = 32;
rtkm_description_length = 64;
rtkm_onmsg_length = 64;
rtkm_eu_length = 64;
rtkm_tag_address_length = 128;

type
TNodeKey = array[0..rtkm_node_key_length-1] of char;
TGroupKey = array[0..rtkm_group_key_length-1] of char;
TTagKey = array[0..rtkm_tag_key_length-1] of char;
PGroupKey = ^TGroupKey;
PNodeKey = ^TNodeKey;
PTagKey = ^TTagKey;
TDeviceKey = array[0..15] of char;
PDeviceKey = ^TDeviceKey;
TShortTagName = packed record
  group : TGroupKey;
  tag : TTagKey;
end;
TTagName = packed record
  node : TNodeKey;
  sname: TShortTagName;
end;
PTagName = ^TTagName;
PShortTagName = ^TShortTagName;
TTagType = __u32;
TTagFlags = __u32;

TTagValue = packed record
  case integer of
  dt_bool  : (b : __bool);
  dt_real4 : (flt : __r4;);
  dt_real8 : (dbl : __r8);
  dt_int8  : (i8 : __i8);
  dt_int16 : (i16 : __i16);
  dt_int32 : (i32 : __i32);
  dt_int64 : (i64 : __i64);
  dt_uint8  : (u8 : __u8);
  dt_uint16 : (u16 : __u16);
  dt_uint32 : (u32 : __u32);
  dt_date   : (date : __r8);
  dt_string : (str : pchar);
end;
PTagValue = ^TTagValue;

TAlarmClass = Integer;
TAlarmType = Integer;
TGroupFlags = Integer;

TRtkGroup = packed record
  node : TNodeKey;
  key : TGroupKey;
  description : array[0..rtkm_description_length-1] of char;
  period : __u32;
  reserved_0 : array[0..12] of __u32;
  itemtable : pointer;
  refcount : __uint;
  flags : TGroupFlags;
  context : __uint;
end;
PRtkGroup = ^TRtkGroup;

TRtkNode = packed record
  key : TNodeKey;
  itemtable : pointer;
  refcount : __uint;
  context : __uint;
  reserved : array[0..15] of __u32;
end;
PRtkNode = ^TRtkNode;

TPackedTag = packed record
  value : TTagValue;
  flags : TTagFlags;
end;
PPackedTag = ^TPackedTag;

TTagMsg = packed record
  case integer of
  0:(
    OnMsg : array[0..rtkm_onmsg_length-1] of char;
    OffMsg : array[0..rtkm_onmsg_length-1] of char;
  );
  1:(EU : array[0..rtkm_eu_length-1] of char;);
end;

TStaticTagData = packed record
  Flags : __u32;
  AlarmRank : __i32;
  u_deadband : r4i32_t;
  u_hihi, u_hi, u_lo, u_lolo : r4i32_t;
  fltRate : __r4;
  CutOffTagName : TShortTagName;
  CutOffMask : TTagFlags;
  u_min, u_max : r4i32_t;
  Msg : TTagMsg;
  Description : packed array[0..rtkm_description_length-1] of char;
  Device : TDeviceKey;
  Address : packed array[0..rtkm_tag_address_length-1] of char;

	ExcDev : __r4;
	ExcMax : __u16;	//
	Access : __u16;		// reserved
	Owner : __u32;		// reserved
	DisplayDigits : __i8;
	Compressing : __u8;
	CompMax : __u16;
	CompDev : __r4;
end;
PStaticTagData = ^TStaticTagData;

TDynamicTagData = packed record
  Value : TPackedTag;
  Time : TRtkTime;
  CachedTime : TRtkTime;
  AlarmStatus : TAlarmType;
  DeviceObj : Pointer;
  BinaryAddress : array[0..31] of __u8;
  BroadcastedValue : TTagValue;
  Reserved : array[0..15] of __u8;
  RefreshColor : __u16;
end;
PDynamicTagData = ^TDynamicTagData;

TRtkTag = packed record
  node : TNodeKey;
  group : TGroupKey;
  key : TTagKey;
  refcount : __uint;
  s : TStaticTagData;
  d : TDynamicTagData;
end;
PRtkTag = ^TRtkTag;

TRtkCursor = TRtkHandle;

  function lock_rtdb(exclusive:__bool;time_out:__uint):__bool;stdcall;
    external 'librtdb.dll' ;
  procedure unlock_rtdb();stdcall;
    external 'librtdb.dll' ;
  function cursor_move_next(cursor:TRtkCursor):__bool;stdcall;
    external 'librtdb.dll' ;
  function cursor_is_end(cursor:TRtkCursor):__bool;stdcall;
    external 'librtdb.dll' ;
  function cursor_move_end(cursor:TRtkCursor):__bool;stdcall;
    external 'librtdb.dll' ;
  function cursor_open_first_subitem(cursor:TRtkCursor):integer;stdcall;
    external 'librtdb.dll' ;
  function cursor_delete(cursor:TRtkCursor):__bool;stdcall;
    external 'librtdb.dll' ;
  function cursor_clear_subitems(cursor:TRtkCursor):__bool;stdcall;
    external 'librtdb.dll' ;
  function cursor_get_item(cursor:TRtkCursor):pointer;stdcall;
    external 'librtdb.dll' ;
  function cursor_get_subitem_count(cursor:TRtkCursor):__bool;stdcall;
    external 'librtdb.dll' ;
  function open_node(key:PNodeKey):integer;stdcall;
    external 'librtdb.dll' ;
  function open_first_node():integer;stdcall;
    external 'librtdb.dll' ;
  function open_local_node():integer;stdcall;
    external 'librtdb.dll' ;
  function open_group(node:integer; gk:PGroupKey):integer;stdcall;
    external 'librtdb.dll' ;
  function query_tag_f(tag : PTagName):PRtkTag;stdcall;
    external 'librtdb.dll';
  function query_tag_g(group : TRtkCursor; tagname : PShortTagName):PRtkTag;stdcall;
    external 'librtdb.dll';
  function tagname_by_text(var tn : TTagName; name : Pchar):__bool;stdcall;
    external 'librtdb.dll' name 'tagname_by_textA';
  function open_tag_f(var tn : TTagName):TRtkCursor;stdcall;
    external 'librtdb.dll';
  function string_by_mask(flags : TTagFlags; str : Pchar):Integer;stdcall;
    external 'librtdb.dll';
  function tagname_to_text(var tn : TTagName; name : pchar; len : integer):integer;stdcall;
    external 'librtdb.dll' name 'tagname_to_textA';
  function pmc_type_cast(var s, d : TPackedTag):__bool;stdcall;
    external 'librtdb.dll';
  procedure pmc_clear(var v : TPackedTag);stdcall;
    external 'librtdb.dll';
  procedure pmc_empty(var v : TPackedTag);stdcall;
    external 'librtdb.dll';
  function pmc_value_string(var v : TPackedTag):pchar;stdcall;
    external 'librtdb.dll';

  // delphi's private functions
  function query_tag_ft(tag : pchar):PRtkTag;stdcall;
  function tag_mask(mask : TTagFlags):string;
  function get_tag_type(flags : __u32):__u8;
  procedure set_tag_type(var flags : __u32; tp : __u8);
  function get_tag_name(var tn : TTagName):string;
  function value_to_string(var p : TPackedTag):string;
  {
RTDB_API PRTK_NODE PMC_API query_node(
	PCNODE_KEY node
	);
RTDB_API PRTK_GROUP PMC_API query_group(
	RTK_CURSOR node,
	const GROUP_KEY *key
	);
RTDB_API PRTK_TAG PMC_API query_tag(
	RTK_CURSOR node,
	const SHORT_TAG_NAME * tag
	);
RTDB_API PRTK_TAG PMC_API query_tag_g(
	RTK_CURSOR group,
	const SHORT_TAG_NAME * tag
	);
RTDB_API PRTK_TAG PMC_API query_tag_f(
	const TAG_NAME * tag
	);

    }
implementation

function query_tag_ft(tag : pchar):PRtkTag;stdcall;
var
  tn : TTagName;
begin
  Result := nil;
  if tagname_by_text(tn, tag)=0 then
    Exit;
  Result := query_tag_f(@tn);
end;

function tag_mask(mask : TTagFlags):string;
var
  m : array[0..63] of char;
begin
  string_by_mask(mask, @m[0]);
  Result := m;
end;

function get_tag_type(flags : __u32):__u8;
begin
  result := (Flags and $ff);
end;

procedure set_tag_type(var flags : __u32; tp : __u8);
begin
  flags := (flags and $ffffff00) or tp;
end;

function get_tag_name(var tn : TTagName):string;
var
  buf : array[0..255] of char;
begin
  tagname_to_text(tn, buf, length(buf));
  result := buf;
end;

function value_to_string(var p : TPackedTag):string;
var
  val : TPackedTag;
begin
  set_tag_type(val.flags, dt_string);
  pmc_type_cast(p, val);
  result := val.value.str;
  pmc_clear(val);
end;

end.
