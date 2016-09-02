unit ExpFunctions;

interface
uses
  ComCtrls,
  rtkdefs,
  librtdb,
  rtkproxy,
  tagattrs,
  windows,
  Unit1;

function GetFirstTag(var tag:TRtkTag):__bool;stdcall;
function GetNextTag(var tag:TRtkTag):__bool;stdcall;
function Discover:__bool;stdcall;
procedure LoadSeeker(h : hwnd);stdcall;
procedure UnloadSeeker;stdcall;

var
  idFind:WORD;

implementation

function GetSelection(var t : TRtkTag; idx : integer):__bool;
var
  i : integer;
  seli : integer;
  it : TListItem;
  pAttrs : TTagAttrs;
begin
  Result := __false;
  ZeroMemory(@t, sizeof(t));
  it := nil;
  if (form1.lv.Items.Count=1) and (idx=1) then
  begin
    // always return the entry
    // regardless of whether that entry is selected
    it := form1.lv.Items[0];
  end else if idx > form1.lv.SelCount then
    Exit
  else begin
    seli := 0;
    with form1.lv do
    begin
      for i:=0 to Items.Count-1 do
      begin
        it := Items.Item[i];
        if not it.Selected then
          Continue;
        Inc(seli);
        if (seli=idx) then
          Break;
      end;
    end;
  end;
  if assigned(it) then
  begin
    assert(assigned(it.Data));
    pAttrs := TTagAttrs(it.Data);
    t := pAttrs.attrs;
    Result := __true;
  end;
end;

function GetFirstTag(var tag:TRtkTag):__bool;stdcall;
begin
  idFind := 1;
  Result := GetSelection(tag, idFind);
end;

function GetNextTag(var tag:TRtkTag):__bool;stdcall;
begin
  Inc(idFind);
  Result := GetSelection(tag, idFind);
end;

function Discover:__bool;stdcall;
begin
  if not Assigned(form1) then
    begin
//      init_proxy();
      Form1 := TForm1.Create(nil);
      RefreshPmcDB(node_time_out);
    end;

  Form1.ShowModal;

  if Form1.ModalResult=IDOK then
  	Result := __true
  else
  	result := __false;
end;

procedure LoadSeeker(h : hwnd);
begin
  init_proxy(h, PROXY_ENABLE_BGLOADER);
end;

procedure UnloadSeeker;
begin
  if assigned(Form1) then
  begin
    Form1.Free;
    Form1 := nil;
  end;
  uninit_proxy;
end;

end.
