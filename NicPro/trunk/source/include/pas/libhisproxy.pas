{
	access wrapper layer to the archive subsystem.

	created:
	- jhunter@tsinghua.org.cn, 2004/8/9
}
unit libhisproxy;

interface

uses librtk, librtdb, rtkdefs, windows;

type
	hislog_key_t = __i64;
	hislog_item_t = TPackedTag;
	Phislog_item_t = ^hislog_item_t;
{$A8}
  archive_cache_info_t = record
	  tagCount : __uint;
	  valueCount : __uint;
	  step : __uint;
	  names : PTagName;
	  startTime : hislog_key_t;
	  queryTime : hislog_key_t;
  end;
  Parchive_cache_info_t = ^archive_cache_info_t; 

	procedure initHisProxy; stdcall;external 'libhisproxy.dll';

	procedure uninitHisProxy();stdcall;external 'libhisproxy.dll';

	procedure queryHisData(
		tag : PTagName;
		var startTime : FILETIME;
		interval : integer;
    length : integer
		);stdcall;external 'libhisproxy.dll';

	function getHisData(value : Phislog_item_t):__bool;
  stdcall;external 'libhisproxy.dll';

	function hislogKeyByTime(var tm : FILETIME):hislog_key_t;stdcall;external 'libhisproxy.dll';
	
	function hislogTimeByKey_delphi(t : hislog_key_t):FILETIME;stdcall;
    external 'libhisproxy.dll' name 'hislogTimeByKey';

	function hislogTimeByKey(t : hislog_key_t):FILETIME;stdcall;

	function hislogGetCurrentTime:hislog_key_t;stdcall;external 'libhisproxy.dll';

	function createArchiveCache:TRtkHandle;stdcall;external 'libhisproxy.dll';

	procedure cacheSetTimeSpan(
		cache : TRtkHandle;
		t1 : hislog_key_t;
		step : __uint;
		count : __uint
		);stdcall;external 'libhisproxy.dll';

	procedure cacheScrollTimeSpan(
		cache : TRtkHandle;
		relativeTime : hislog_key_t
		);stdcall;external 'libhisproxy.dll';

	procedure cacheSetNames(
		cache : TRtkHandle;
		names : PTagName;
		count : __uint
		);stdcall;external 'libhisproxy.dll';

	procedure cacheQuery(
		cache : TRtkHandle;
		timeout : __uint;
    info : Parchive_cache_info_t
		);stdcall;external 'libhisproxy.dll';

	function cacheGetFlag(
		cachre : TRtkHandle;
		tagId : __uint
		):__bool;stdcall;external 'libhisproxy.dll';

	function cacheGetValue(
		cache : TRtkHandle;
		tagId : __uint;
		valueId : __uint
		):Phislog_item_t;stdcall;external 'libhisproxy.dll';

  function cacheGetReturnedValueCount(
    cache : TRtkHandle;
    idx : Integer
    ):Integer;stdcall;external 'libhisproxy.dll';

  function timeAsString(t : hislog_key_t):string;

  function hislogOleTimeByKey(k : hislog_key_t):TDateTime;
implementation

uses
  sysUtils;

function timeAsString(t : hislog_key_t):string;
var
  ft : filetime;
  st : systemtime;
begin
  t := __i64(t * 10000000);
  copyMemory(@ft, @t, sizeof(t));
  fileTimeToSystemTime(ft, st);
  result := format(
    '%04d-%02d-%02d %02d:%02d:%02d',
    [st.wyear, st.wmonth, st.wday, st.whour, st.wminute, st.wsecond]
    );
end;

function hislogTimeByKey(t : hislog_key_t):FILETIME;stdcall;
begin
  asm
    mov edx, dword ptr [t+4]
    mov eax, dword ptr [t]
    push edx
    push eax
    call hislogTimeByKey_Delphi
    push ebx
    mov ebx, [result]
    mov dword ptr[ebx], eax
    mov dword ptr[ebx+4], edx
    pop ebx
  end;
end;

function hislogOleTimeByKey(k : hislog_key_t):TDateTime;
var
  st : systemtime;
  ft : filetime;
begin
  ft := hislogTimeByKey(k);
  fileTimeToSystemTime(ft, st);
  result := systemTimeToDateTime(st);
end;

end.
