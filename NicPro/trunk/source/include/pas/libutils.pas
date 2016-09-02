unit libutils;

interface
uses
  windows;
const
	MAX_ARGC=128;
	MAX_LINE_LENGTH=2048;
  dbg_level_none =0;
  dbg_level_fatal =    1;
  dbg_level_error     = 2;
  dbg_level_trace     = 3;
  dbg_level_verbose   = 5;
  msg_debug = 0;
  msg_trace = 1;
  msg_error = 2;
type
UTILS_TIME = packed record
	tmhigh : cardinal;
  tmlow  : cardinal;
end;

TArgs = packed record
	argc : integer;
	argv : array [0..MAX_ARGC-1] of pchar;
	arg_idx : array[0..MAX_ARGC-1] of integer;
	args : array [0..MAX_LINE_LENGTH-1] of char;
end;

CArgs = TArgs;

TCmdLineArg = packed record
	flag : PChar;
  value : pointer;
  mask : pchar;
  hint : pchar;
end;

TUtilsPutsProc = function (level : Integer; s : pchar):integer;stdcall;

	procedure time_mark(var tm : UTILS_TIME);stdcall;external 'libutils.dll';
  function time_diff(var t2,t1:UTILS_TIME):double;stdcall;external 'libutils.dll';
  {$IFDEF USE_TRACE}
  function trace(s : pchar;args : array of const):integer;stdcall;
    external 'libutils.dll' name 'delphi_trace';
  {$ELSE}
	function trace(s : pchar;args : array of const):integer;
  {$ENDIF}
	procedure SetDebug;
  function GetDebug:boolean;
  function check_running(szProgramName : pchar):longbool;stdcall;external 'libutils.dll' name 'check_runningA';
  function set_running(szProgramName : pchar):longbool;stdcall;external 'libutils.dll' name 'set_runningA';
  function iconize_window(hwnd:integer;szTip : pchar;hIcon:integer):integer;stdcall;external 'libutils.dll';
  function deiconize_window(hwnd:integer):integer;stdcall;external 'libutils.dll';
  function parse_arg_ex(arg_str:Pchar;var args : TArgs;delimiter:PChar):integer;stdcall;external 'libutils.dll' name 'parse_arg_exA';
  function parse_arg_ey(arg_str:Pchar;var args : TArgs;delimiter:PChar):integer;stdcall;external 'libutils.dll' name 'parse_arg_eyA';
  function i64Format(value : int64):string;
  function get_file_version(szFile:PChar;liVersion : Pointer):PChar;stdcall;external 'libutils.dll' name 'get_file_versionA';
  function get_file_description(szFile,szDescription:Pchar;len:integer):pchar;stdcall;external 'libutils.dll' name 'get_file_descriptionA';
	{this function was contributed by SPace, may god bless her}
  function pattern_match(pattern,expression:PChar):longbool;stdcall;external 'libutils.dll' name 'pattern_matchA';
	function FileInfo(fl :PChar ):string;
  function GetMangledFileName(fn : string;maxlen : integer):string;
  function running_as_administrator:longbool;stdcall;external 'libutils.dll';
	function browse_for_folder(hParent:Cardinal;szTitle:PChar):PChar;stdcall;external 'libutils.dll';
  function utils_puts:pdword;external 'libutils.dll' name 'utils_putsA';
  function delphi_puts(level : integer; s : pchar):integer;
  function byte_array_by_hex_string(szArray:Pchar;pbArray:Pchar;length:pointer{of integer}):integer;stdcall;external 'libutils.dll';
  function key_to_string(k : Pointer; length : integer):string;

  function my_wsprintf(Output: PChar; Format: PChar; a1,a2,a3,a4,a5,a6:Integer):Integer;cdecl;external user32 name 'wsprintfA';
  function file_exists(name : pchar):longbool;stdcall;external 'libutils.dll' name 'file_existsA';
implementation
uses
  tlhelp32,
	sysutils;
var
	g_bDebug : boolean;
  g_bDebugSet : boolean;
  g_szSystemProcesses : array[0..5] of pchar =
  (
  	'system',
    'services.exe',
    'svchost.exe',
    'lsass.exe',
    'spoolsv.exe',
    'winlogon.exe'
  );

  function setup_console_for_log:integer;external 'libutils.dll';

{$IFNDEF USE_TRACE}
function trace(s : pchar;args : array of const):integer;
begin
	result := 0;
end;
{$ENDIF}

procedure SetDebug;
var
	i : integer;
begin
	if g_bDebugSet then exit;
	g_bDebug:=false;
  g_bDebugSet := true;
	for i:=1 to paramcount do
  begin
  	if lowercase(paramstr(i))='--debug' then
    	g_bDebug:=true;
  end;
  if g_bDebug then setup_console_for_log;
end;

function GetDebug:boolean;
begin
	result := g_bDebug;
end;

function i64Format(value : int64):string;
var
  s : string;
  len,x,y:integer;
begin
  s:=inttostr(value);
	len := length(s);
	y := len mod 3;
  result := '';
  for x:=1 to len do
  begin
		if y=0 then
		begin
			if x<>1 then
				result := result+',';
			y := 3;
		end;
		result := result + s[x];
		dec(y);
	end;
end;

function FileInfo(fl :PChar ):string;
var
  fi  : WIN32_FIND_DATA;
  h   : DWORD;
  s   : string;
  t2 : SYSTEMTIME;
  buf : array [0..255] of char;
begin
  if fl='' then
  begin
  	result := '文件信息不可访问。';
    exit;
  end;
  h := FindFirstFile(pchar(fl),fi);
//  s := '路    径: '+fl;
	s :=  '名    称: '+get_file_description(fl,buf,sizeof(buf));
  s := s+#13+('版    本: '+get_file_version(fl,nil));
  if h=INVALID_HANDLE_VALUE then
  begin
  	s := s+#13+'修改时间: <未知>';
    s := s+#13+'文件大小: <未知>';
    s := s+#13+'文件属性: <未知>';
  end else begin
    FileTimeToSystemTime(fi.ftLastWriteTime,t2);
    s := s + #13+'修改时间: ';
  	s := s + IntToStr(t2.wYear)+'年'+IntToStr(t2.wMonth)+
      '月'+IntToStr(t2.wDay)+'日';
//    s := s + ' '+ Format('%02d',[t2.wHour])+':'+
//    Format('%02d',[t2.wMinute])+':'+Format('%02d',[t2.wSecond]);
		s := s + ' '+ IntToStr(t2.wHour)+':'+IntToStr(t2.wMinute)+
      ':'+IntToStr(t2.wSecond);
    s := s + #13+'文件大小: '+IntToStr(fi.nFileSizeLow);
    s := s+#13+'文件属性: ';
    if (fi.dwFileAttributes and FILE_ATTRIBUTE_SYSTEM)<>0 then
    	s:=s+'系统 ';
    if (fi.dwFileAttributes and FILE_ATTRIBUTE_HIDDEN)<>0 then
    	s:=s+'隐藏 ';
    if (fi.dwFileAttributes and FILE_ATTRIBUTE_ARCHIVE)<>0 then
    	s:=s+'归档 ';
    if (fi.dwFileAttributes and FILE_ATTRIBUTE_NORMAL)<>0 then
    	s:=s+'普通 ';
    windows.FindClose(h);
  end;
  Result := s;
end;

function _GetFileTime(fl : PChar):string;
var
	t1 : FILETIME;
  t2 : SYSTEMTIME;
  h : DWORD;
begin
	h:=CreateFile(PChar(fl),0,0,nil,OPEN_EXISTING,0,0);
  if h=INVALID_HANDLE_VALUE then
  	Result := '<未知>'
  else begin
  	GetFileTime(h,nil,nil,@t1);
    FileTimeToSystemTime(t1,t2);
  	Result := IntToStr(t2.wYear)+'年'+IntToStr(t2.wMonth)+
      '月'+IntToStr(t2.wDay)+'日';
    Result := Result + ' '+ IntToStr(t2.wHour)+':'+
      IntToStr(t2.wMinute)+':'+IntToStr(t2.wSecond);
    CloseHandle(h);
  end;
end;

{你知道，有些时候文件名太长，界面上显示不下，我们把他搞一搞
缩短一下显示出来}
{你知道，有些时候文件名太长，界面上显示不下，我们把他搞一搞
缩短一下显示出来}
function GetMangledFileName(fn : string;maxlen : integer):string;
var
  header_length : integer;
  tail_length : integer;
begin
	if length(fn)<=maxlen then
  begin
  	result := fn;
    exit;
  end;
  tail_length := length(extractfilename(fn))+1;
  header_length := maxlen-tail_length-3;
  if header_length<3 then
  begin
  	result := fn;
    exit;
  end;
  result := copy(fn,1,header_length)+'...'+
    copy(fn,length(fn)-tail_length+1,tail_length);
end;

function get_system_process_file_name(fl : string):string;
var
	i : integer;
  buf : array [0..256] of char;
begin
	result := lowercase(fl);
	if win32platform=VER_PLATFORM_WIN32_NT then
  begin
  	for i:=low(g_szSystemProcesses) to High(g_szSystemProcesses) do
    begin
    	if g_szSystemProcesses[i]=result then
      begin
      	GetSystemDirectory(buf,sizeof(buf));
        result := string(buf)+'\'+result;
      end;
    end;
  end;
end;

function key_to_string(k : Pointer; length :integer):string;
var
  buf : array[0..1024] of char;
begin
  zeromemory(@buf[0], sizeof(buf));
  if length >= sizeof(buf) then
    length := sizeof(buf) - 1;
  strlcopy(@buf[0], pchar(k), length);
  Result := buf;
end;

function delphi_puts(level : integer; s : pchar):integer;
var
  proc : TUtilsPutsProc;
begin
  proc := TUtilsPutsProc(pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^);
  result := proc(level, s);
end;

initialization
  SetDebug;

end.

