unit untCopyPaste;

interface
uses
  windows,
  f8,
  rtkdefs,
  libcobj,
  classes,
  math,
  libutils,
  forms,
  blklayout,
  controls;

procedure CopyItemsToClipboard(h : HWND; layout : TBlkLayout);
function PasteItemsFromClipboard(h : HWND; target : Pblk):pointer;
function CanPaste(layout : TBlkLayout):boolean;

implementation
uses
  sysutils;

procedure CopyItemsToClipboard(h : HWND; layout : TBlkLayout);
var
  it : TUiItem;
  pp : Ppointer;
  hMem : THandle;
  pMem : pointer;
  sz : Integer;
  ps : Pf8_u16;
  fps : Pf8_u16;
  fpp : Ppointer;
begin
  if layout.selCount=0 then
    exit;
  sz := layout.SelCount * sizeof(pointer);
  sz := sz + layout.selCount * sizeof(f8_u16);

  hMem := GlobalAlloc(GMEM_MOVEABLE, sz);
  pMem := GlobalLock(hMem);

  pp := Ppointer(pMem);
  ps := Pf8_u16(Integer(pp) + sizeof(pointer)*layout.selCount);
  fps := ps;
  fpp := pp;

  it := layout.getFirstSelection;
  while assigned(it) do begin
    if it is TBlk then begin
      pp^ := TBlk(it).blk;
      ps^ := blk_magic;
    end else if it is TLink then begin
      pp^ := TLink(it).Flink;
      ps^ := lnk_magic;
    end else begin
      ps^ := 0;
    end;
    Inc(pp);
    inc(ps);
    it := layout.getNextSelection(it);
  end;

  kproxy_copy(@layout.block.uuid, fps, fpp^, layout.SelCount);

  globalUnlock(hMem);
  globalFree(hMem);
end;

function PasteItemsFromClipboard(h : HWND; target : PBlk):pointer;
begin
  result := kproxy_paste(target);
end;

function CanPaste(layout : TBlkLayout):boolean;
begin
  result := false;
  if not isClipboardFormatAvailable(kproxy_get_clipboard_format) then
    exit;
  if (layout.Block.h.flags and blk_f_readonly)<>0 then
    exit;
  result := true;
end;

end.

