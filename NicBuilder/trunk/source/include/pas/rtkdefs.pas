unit rtkdefs;

interface
uses
  Windows;
const
  __true = 1;
  __false = 0;
  
type
__i8  = shortint;
__i16 = smallint;
__i32  = integer;
__int = integer;
__u8  = byte;
__u16 = word;
__uint = dword;
__u32  = dword;
__bool = byte;
__r4 = single;
__r8 = double;
__i64 = int64;

r4i32_t = packed record
  case integer of
  0: (flt : __r4;);
  1: (i : integer;);
end;

TRtkTime = packed record
  Hi : __u32;
  Lo : __u32;
end;

PRtkTime = ^TRtkTime;

PLIST_ENTRY = ^LIST_ENTRY;
PTRIPLE_LIST_ENTRY = ^TRIPLE_LIST_ENTRY;

LIST_ENTRY = packed record
  Flink : PLIST_ENTRY;
  BLink : PLIST_ENTRY;
end;

TRIPLE_LIST_ENTRY = packed record
  Flink : PTRIPLE_LIST_ENTRY;
  BLink : PTRIPLE_LIST_ENTRY;
  Head : PTRIPLE_LIST_ENTRY;
end;

procedure InitializeListHead(l : PLIST_ENTRY);
function IsListEmpty(l : PLIST_ENTRY):boolean;
function RemoveHeadList(l : PLIST_ENTRY):PLIST_ENTRY;
function RemoveTailList(l : plist_entry):plist_entry;
procedure RemoveEntryList(l : plist_entry);
procedure InsertTailList(h, l : plist_entry);
procedure InsertHeadList(h, l : plist_entry);

implementation

procedure InitializeListHead(l : PLIST_ENTRY);
begin
  l.Flink := l;
  l.BLink := l;
end;

function IsListEmpty(l : PLIST_ENTRY):boolean;
begin
  result := l.Flink=l;
end;

function RemoveHeadList(l : PLIST_ENTRY):PLIST_ENTRY;
begin
  Result := l.Flink;
  RemoveEntryList(l);
end;

function RemoveTailList(l : plist_entry):plist_entry;
begin
  result := l.BLink;
  removeentryList(l);
end;

procedure RemoveEntryList(l : plist_entry);
var
  exb, exf : plist_entry;
begin
  exb := l.BLink;
  exf := l.flink;
  exb.Flink := l.Flink;
  exf.BLink := l.blink;
  l.Flink := l;
  l.BLink := l;
end;

procedure InsertTailList(h, l : plist_entry);
var
  exb: plist_entry;
begin
  exb := h.BLink;
  l.Flink := h;
  l.BLink := exb;
  exb.Flink := l;
  h.BLink := l;
end;

procedure InsertHeadList(h, l : plist_entry);
var
  exf: plist_entry;
begin
  exf := h.Flink;
  exf.BLink := l;
  l.Flink := exf;
  l.BLink := h;
  h.Flink := l;
end;

end.
 