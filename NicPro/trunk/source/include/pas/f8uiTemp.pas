unit f8ui;

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

type
{$IFDEF NOTHING}
  TSelectedItemType = (si_None, si_Block, si_Link, si_ExtPin);
  TSelectedItem = record
    itemType : TSelectedItemType;
    case tag : TSelectedItemType of
    si_Block:(block : PBlk);
    si_Link :(link : Pcblk_link_t);
    si_ExtPin:(extPin : Pcblk_pin_t);
  end;

  {
    TSelectedItems tracks a list of current copied objects, it stores
    a string moniker to the copied objects, thus when the selected object
    is later deleted, TSelectedItems need not be notified to update
    its list, because when the contained string moniker is read, the
    discrepency can be detected.

    this string moniker is coded as following:
    <stringMoniker> ::= <objType> ':' <objPath>
    <objType> ::= 'Blk' | 'Lk' | 'Exp'
    <objPath> ::= a standard path to a block or link object
  }
  TSelectedItems = class
  private
    FItemList : TStringList;
    FRootBlk : PBlk;

  protected
    function GetItemCount : Integer;
    function GetItem(idx : Integer) : TSelectedItem;

  public
    constructor Create;
    destructor  Destroy; override;

    {copy current selected objects}
    procedure CopyItems(layout : TBlkLayout);
    {paste selected objects into new position}
    procedure PasteItems(layout : TBlkLayout);
    procedure Clear;

    property  RootBlk : PBlk read FRootBlk;
    property  ItemCount:Integer read GetItemCount;
    property  Items[idx : Integer] : TSelectedItem read GetItem;
  end;
  
var
  g_Layout : TBlkLayout;
  g_bOnline : boolean;

procedure Unimplemented(h : hwnd);

implementation
uses
  sysutils;

procedure Unimplemented(h : hwnd);
begin
  messagebox(h, 'unimplemented', 'error', mb_ok);
end;

{ TSelectedItems }

procedure TSelectedItems.Clear;
begin
  FItemList.Clear;
end;

procedure TSelectedItems.CopyItems(layout: TBlkLayout);
var
  it : TUiItem;
  s : string;
  lk : Pcblk_link_t;
begin
  {for each selected object, generate a moniker and store
  it into the list}
  Clear;
  it := layout.GetFirstSelection;
  while assigned(it) do
  begin
    s := '';
    if it is TBlk then
      s := 'Blk:' + GetBlkPath((TBlk(it)).blk)
    else if it is TLink then
    begin
      lk := TLink(it).Flink;
      s := 'Lk:' + GetPinPath(lk.t);
    end;
    if s<>'' then
      FItemList.Add(s);
    it := layout.GetNextSelection(it);
  end;
end;

constructor TSelectedItems.Create;
begin
  FItemList := TStringList.Create;
end;

destructor TSelectedItems.Destroy;
begin
  Clear;
end;

function TSelectedItems.GetItem(idx : Integer): TSelectedItem;
var
  s : String;
  i : Integer;
  objType : string;
  objPath : string;
  p : Pblk_pin_t;
begin
  s := FItemList[idx];
  i := Pos(':', s);
  objType := Copy(s, 1, i-1);
  objPath := Copy(s, i+1, Length(s));

  Result.itemType := si_None;

  if objType = 'Blk' then
  begin
    Result.itemType := si_Block;
    Result.block := Iblk_blk_by_path(nil, pchar(objPath))
  end else if objType = 'Lk' then
  begin
    Result.itemType := si_Link;
    p := Iblk_pin_by_path(nil, pchar(objPath));
    if not assigned(p) then
      Exit;
    Result.link := Icblk_first_connection(p);
  end else begin
    assert(false);
  end;
end;

function TSelectedItems.GetItemCount: Integer;
begin
  Result := FItemList.Count;
end;

procedure TSelectedItems.PasteItems(layout: TBlkLayout);
begin

end;
{$ENDIF}

initialization

end.

