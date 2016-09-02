unit libcobj;

interface
uses
  rtkdefs;

type

PStream = ^IF8Stream;
PMemStream = ^IMemStream;
PTreeNode = ^ITreeNode;
PF8Stream_vtable = ^IF8Streamvtable;

vfunc_destroy = function (_this : pointer) : pointer;cdecl;

// virtual functions for IF8Stream
vfunc_get = function(s : PStream; buf : pointer; bytes : __uint) : __uint;cdecl;
vfunc_put = function(s : PStream; buf : pointer; bytes : __uint) : __uint;cdecl;
vfunc_tell = function (s : PStream) : __uint;cdecl;
vfunc_seek = function(s : PStream; offset,whence:__i32):__uint; cdecl;
vfunc_align = function(s : PStream; boundary : __int):__uint; cdecl;
vfunc_left = function(s : PStream):__uint;cdecl;
vfunc_eos = function(s : PStream):__bool;cdecl;

vfunc_on_mount = procedure(nd : PTreeNode);
vfunc_before_mount = procedure(nd : PTreeNode);
vfunc_after_mount = procedure(nd : PTreeNode);

IF8Streamvtable = packed record
  destroy : vfunc_destroy;
  get : vfunc_get;
  put : vfunc_put;
  tell : vfunc_tell;
  seek : vfunc_seek;
  align : vfunc_align;
  left : vfunc_left;
  eos : vfunc_eos;
end;

IF8Stream = packed record
  vtbl : PF8Stream_vtable;
end;

IMemStream = packed record
  vtbl : pointer;
  __inherited_IF8Stream : IF8Stream;
  buffer:PChar;
  length,_pointer:__uint;
end;

ITreeNode_vtable = packed record
  destroy : vfunc_destroy;
  on_mount : vfunc_on_mount;
  before_mount : vfunc_before_mount;
  after_mount : vfunc_after_mount;
end;

ITreeNode = packed record
  vtbl : pointer;
	uplink : PTreeNode;
	first_child : PTreeNode;
	sibling : LIST_ENTRY;
end;

tr_TraverseMethod = (
	ITreeNode_pre_order = 0,
	ITreeNode_post_order = 1,
	ITreeNode_exclude = 2,
  ITreeNode_both_order = 3
);

PTreeNodeVisitor = function (nd : PTreeNode; firstVisit : __bool; context : __uint):__bool;cdecl;
procedure ITreeNode_traverse(
  root : PTreeNode;
  proc : PTreeNodeVisitor;
  recursive : __bool;
  method : tr_TraverseMethod;
  context : __uint
  );
  cdecl; external 'libcobj.dll';
  
function create_mem_stream(size : integer):PStream;
  cdecl;external 'libcobj.dll';
procedure delete_mem_stream(p : Pstream);
  cdecl;external 'libcobj.dll';
function create_temp_stream:PStream;
  cdecl;external 'libcobj.dll';
procedure delete_temp_stream(p : Pstream);
  cdecl;external 'libcobj.dll';

function MStream2Stream(p : Pmemstream):Pstream;
function Stream2MStream(p:Pstream):Pmemstream;

implementation
var
  MemStreamOffset : integer;

procedure InitOffsets;
var
  ms : IMemStream;
begin
  MemStreamOffset := pchar(@ms.__inherited_IF8Stream) - pchar(@ms);
end;

function MStream2Stream(p : Pmemstream):Pstream;
begin
  result := @p.__inherited_IF8Stream;
end;

function Stream2MStream(p:Pstream):Pmemstream;
begin
  result := PMemStream(pchar(p) - MemStreamOffset);
end;

initialization
  InitOffsets;
end.
