{
  point search/select dialog

  by Chenjun, someday/someyear
  Revision history:
  Created : unknown date
  Revised by Jackie, 2003/7/4, search added, layout modified
}
unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, ExtCtrls, StdCtrls, registry, clipbrd,
  rtkdefs, librtk, libutils, librtdb, rtkproxy, tagAttrs, SeekUtils, Grids, ValEdit, ImgList,
  Menus;

const
  node_time_out = 500;
  group_time_out = 2000;
  tag_time_out = 20000;

type
  TForm1 = class(TForm)
    ImageList1: TImageList;
    Panel2: TPanel;
    tv: TTreeView;
    lv: TListView;
    Panel3: TPanel;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    btnRefresh: TButton;
    btnOK: TButton;
    btnSearch: TButton;
    btnHelp: TButton;
    btnCancel: TButton;
    sb: TStatusBar;
    mnuFields: TPopupMenu;
    m1: TMenuItem;
    N1: TMenuItem;
    N2: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    N6: TMenuItem;
    N7: TMenuItem;
    N8: TMenuItem;
    N9: TMenuItem;
    N10: TMenuItem;
    N11: TMenuItem;
    N12: TMenuItem;
    N13: TMenuItem;
    N14: TMenuItem;
    N15: TMenuItem;
    Label5: TLabel;
    cmbSearchIn: TComboBox;
    btnSelectAll: TButton;
    btnClearAll: TButton;
    btnCopy: TButton;
    cmbName: TComboBox;
    cmbDesc: TComboBox;
    cmbDevice: TComboBox;
    cmbAddress: TComboBox;
    N16: TMenuItem;
    Timer1: TTimer;
    N17: TMenuItem;
    mnuAutoRefresh: TMenuItem;
    procedure btnRefreshClick(Sender: TObject);
    procedure tvExpanding(Sender: TObject; Node: TTreeNode;
      var AllowExpansion: Boolean);
    procedure btnOKClick(Sender: TObject);
    procedure btnSearchClick(Sender: TObject);
    procedure tvChange(Sender: TObject; Node: TTreeNode);
    procedure SelectFields(Sender: TObject);
    procedure btnCancelClick(Sender: TObject);
    procedure btnClearAllClick(Sender: TObject);
    procedure btnSelectAllClick(Sender: TObject);
    procedure lvDeletion(Sender: TObject; Item: TListItem);
    procedure cmbNameExit(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure btnCopyClick(Sender: TObject);
    procedure lvDblClick(Sender: TObject);
    procedure tvDblClick(Sender: TObject);
    procedure cmbNameKeyPress(Sender: TObject; var Key: Char);
    procedure Timer1Timer(Sender: TObject);
    procedure mnuAutoRefreshClick(Sender: TObject);
    procedure lvColumnClick(Sender: TObject; Column: TListColumn);
    procedure lvCompare(Sender: TObject; Item1, Item2: TListItem;
      Data: Integer; var Compare: Integer);
  private
    procedure ShowTag(n: TTreeNode);overload;
    procedure ShowTag(cr : TRtkCursor);overload;
    function MatchItem(ct: TRtkCursor): boolean;
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  iSortOrder : Integer;
  bSortReverse : boolean;
  bInSearch : boolean;

  procedure RefreshPmcDB(time_out:__uint);

implementation
type
  TCriteria = record
    name : array[0..127] of char;
    desc : array[0..127] of char;
    dev  : array[0..127] of char;
    addr : array[0..127] of char;
    _type : integer;
  end;
var
  g_Criteria : TCriteria;
{$R *.dfm}

procedure RefreshPmcDB(time_out:__uint);
var
  hNode:integer;
  node:PRtkNode;
  nodeKey:Array[0..rtkm_node_key_length] of char;
  treeNode:TTreeNode;
  childNode:TTreeNode;
begin
  form1.tv.Images := form1.ImageList1;
  form1.tv.Items.Clear();
  treeNode := form1.tv.Items.AddChild(nil, 'NicBuilder工业监控系统');
  treeNode.ImageIndex := 25;
  treeNode.SelectedIndex := 25;

	// discover_nodes(time_out);

  if lock_rtdb(0, 1000)=0 then
    Exit;

	hNode := open_first_node();
  while cursor_is_end(hNode)=0 do
  begin
    node  := PRtkNode(cursor_get_item(hNode));
    CopyMemory(@nodeKey, @node.key, sizeof(node.key));
    nodeKey[rtkm_node_key_length] := chr(0);
    childNode := Form1.tv.Items.AddChild(treeNode, nodeKey);
    childNode.Data := nil;
    childNode.HasChildren := true;
    childNode.ImageIndex := 2;
    childNode.SelectedIndex := 2;
    cursor_move_next(hNode);
  end;
  close_handle(hNode);

  unlock_rtdb();
end;

procedure TForm1.btnRefreshClick(Sender: TObject);
begin
  RefreshPmcDB(node_time_out);
end;

procedure ExpandPmcNode(treeNode:TTreeNode);
var
  hNode:integer;
  hGroup:integer;
  temp:PChar;
  nodeKey:Array[0..rtkm_node_key_length] of char;
  groupKey:Array[0..rtkm_group_key_length] of char;
  group:PRtkGroup;
  childNode:TTreeNode;
begin
  if assigned(treeNode.Data) then
    Exit;
  treeNode.Data := Pointer(1);
  treeNode.HasChildren := false;

  ZeroChrArray(nodeKey, 0, rtkm_group_key_length);
  temp := PChar(treeNode.Text);
  PChar2Array(temp, nodeKey, 0, length(temp)-1);

  // discover_groups(@nodeKey, group_time_out);

  if lock_rtdb(0, group_time_out)=0 then
    Exit;

  hNode := open_node(@nodekey);

  if hNode <> 0 then
    begin
      hGroup := cursor_open_first_subitem(hNode);
      if hGroup <> 0 then
        begin
          while cursor_is_end(hGroup)=0 do
            begin
              group := PRtkGroup(cursor_get_item(hGroup));
              CopyMemory(@groupKey, @group.key, sizeof(group.key));
              groupKey[rtkm_group_key_length] := chr(0);
              childNode := form1.tv.Items.AddChild(treeNode, groupKey);
              childNode.Data := nil;
              childNode.HasChildren := True;
              childNode.ImageIndex := 1;
              childNode.SelectedIndex := 1;
              cursor_move_next(hGroup);
            end;
        end;
    end;
  unlock_rtdb();
end;

procedure ExpandPmcGroup(fatherNode:TTreeNode; treeNode:TTreeNode);
var
 hNode:integer;
 hGroup:integer;
 hTag:integer;
 temp:PChar;
 temp2:PChar;
 nodeKey:Array[0..rtkm_node_key_length] of char;
 groupKey:Array[0..rtkm_group_key_length] of char;
 tagKey:Array[0..rtkm_tag_key_length] of char;
 tag:PRtkTag;
 childNode:TTreeNode;
begin
  if Assigned(treeNode.data) then
    Exit;
  treeNode.Data := Pointer(1);

  treeNode.HasChildren := false;

  ZeroChrArray(nodeKey, 0, rtkm_node_key_length);
  ZeroChrArray(groupKey, 0, rtkm_group_key_length);
  temp := PChar(fatherNode.Text);
  temp2 := PChar(treeNode.Text);
  PChar2Array(temp, nodeKey, 0, length(temp)-1);
  PChar2Array(temp2, groupKey, 0, length(temp2)-1);

  // discover_tags(@nodeKey, @groupKey, tag_time_out);

  if lock_rtdb(0, group_time_out)=0 then
    Exit;

  hNode := open_node(@nodeKey);

  if hNode <> 0 then
    begin
      hGroup := open_group(hNode, @groupKey);
      if hGroup <> 0 then
      begin
        hTag := cursor_open_first_subitem(hGroup);
        while cursor_is_end(hTag)=0 do
        begin
          tag := PRtkTag(cursor_get_item(hTag));
          CopyMemory(@tagKey, @tag.key, sizeof(tag.key));
          tagKey[rtkm_tag_key_length] := chr(0);
          childNode := form1.tv.Items.AddChild(treeNode, tagKey);
          childNode.Data := nil;
          if get_tag_type(tag.s.flags) = dt_bool then
          begin
            childNode.ImageIndex := 3;
            childNode.SelectedIndex := 3;
          end
          else
          begin
            childNode.ImageIndex := 12;
            childNode.SelectedIndex := 12;
          end;
          cursor_move_next(hTag);
        end;
        close_handle(hTag);
      end;
    end;

  unlock_rtdb();
end;

procedure TForm1.tvExpanding(Sender: TObject; Node: TTreeNode;
  var AllowExpansion: Boolean);
begin
  case Node.Level of
    1:
      ExpandPmcNode(Node);
    2:
      ExpandPmcGroup(Node.Parent, Node);
    else
      ;
  end;
end;

procedure TForm1.btnOKClick(Sender: TObject);
begin
  ModalResult := mrOk;
end;

function TForm1.MatchItem(ct : TRtkCursor):boolean;
var
  pTag : PRtkTag;
begin
  Result := false;
  pTag := PRtkTag(cursor_get_item(ct));
  if not assigned(pTag) then
    Exit;
  with g_Criteria do
  begin

    if name[0] <> chr(0) then
    begin
      {match name}
      if not pattern_match(name, pchar(key_to_string(@pTag.key, sizeof(pTag.key)))) then
        Exit;
    end;

    if desc[0] <> chr(0) then
    begin
      {match name}
      if not pattern_match(desc, pTag.s.Description) then
        Exit;
    end;

    if dev[0] <> chr(0) then
    begin
      if not pattern_match(dev, pchar(key_to_string(@pTag.s.device, sizeof(pTag.s.Device)))) then
        Exit;
    end;

    if addr[0] <> chr(0) then
    begin
      if not pattern_match(addr, pTag.s.Address) then
        Exit;
    end;

    if (_type=1) and (get_tag_type(pTag.s.Flags) <> dt_bool) then
      Exit;

    if (_type=2) and (get_tag_type(pTag.s.Flags) <> DT_Real4) then
      Exit;

    if (_type=3) and (get_tag_type(pTag.s.Flags) <> DT_Real8) then
      Exit;

    if (_type=4) and (get_tag_type(pTag.s.Flags) <> DT_Int16) then
      Exit;

    if (_type=5) and (get_tag_type(pTag.s.Flags) <> DT_Int32) then
      Exit;

    if (_type=6) and (get_tag_type(pTag.s.Flags) <> DT_Date) then
      Exit;

    if (_type=7) and (get_tag_type(pTag.s.Flags) = DT_bool) then
      Exit;
  end; // with

  Result := true;
end;

procedure TForm1.btnSearchClick(Sender: TObject);
var
  cn, cg, ct : TRtkCursor;
begin
  bInSearch := true;
  {
  do search in current group first, if not found, search globally
  but for now, we just search globally for a quick implementation
  }

  lv.Items.BeginUpdate;
  lv.Items.Clear;
  lv.Items.EndUpdate;

  // fill search criteria
  zeromemory(@g_Criteria, sizeof(g_Criteria));
  if cmbName.Text <> '*' then
    strlcopy(@g_Criteria.name[0], PChar(cmbname.Text), sizeof(g_Criteria.name) - 1);
  if cmbDesc.Text <> '*' then
    strlcopy(@g_Criteria.desc[0], pchar(cmbDesc.Text), sizeof(g_Criteria.desc) - 1);
  if cmbDevice.Text <> '*' then
    strlcopy(@g_Criteria.dev[0], pchar(cmbDevice.Text), sizeof(g_Criteria.dev) - 1);
  if cmbAddress.Text <> '*' then
    strlcopy(@g_Criteria.addr[0], pchar(cmbAddress.Text), sizeof(g_Criteria.addr) - 1);

  g_Criteria._type := cmbSearchIn.ItemIndex;

  if lock_rtdb(0, 1000)=0 then
    Exit;

  cn := open_first_node;
  while cursor_is_end(cn)=0 do
  begin
    cg := cursor_open_first_subitem(cn);
    while cursor_is_end(cg)=0 do
    begin
      ct := cursor_open_first_subitem(cg);
      while cursor_is_end(ct)=0 do
      begin
        if MatchItem(ct) then
        begin
          ShowTag(ct);
        end;
        cursor_move_next(ct);
      end;
      close_handle(ct);
      cursor_move_next(cg);
    end;
    close_handle(cg);
    cursor_move_next(cn);
  end;
  close_handle(cn);
  
  unlock_rtdb();

  sb.Panels[0].Text := IntToStr(lv.Items.Count) + ' 个项目.';

  bInSearch := false;
end;

function TagValue(pTag : PRtkTag) : string;
var
  strVal : TPackedTag;
begin
  if (pTag.d.Value.Flags and TF_Valid)=0 then
  begin
    Result := 'N.A.';
    Exit;
  end;
  set_tag_type(strVal.flags, dt_string);
  pmc_type_cast(pTag.d.Value, strVal);
  result := strVal.value.str;
  pmc_clear(strVal);
end;

procedure TForm1.ShowTag(cr : TRtkCursor);
var
  li : TListItem;
  pTag : PRtkTag;
  pAttrs : TTagAttrs;
  i : Integer;
begin
  pTag := PRtkTag(cursor_get_item(cr));
  if not assigned(pTag) then
    Exit;
  li := lv.Items.Add;
  li.Caption := key_to_string(@pTag.key, sizeof(pTag.key));
  with li.SubItems do
  begin
    Add(
      key_to_string(
        @pTag.node,
        sizeof(pTag.node)) + '.' + key_to_string(@pTag.group, sizeof(pTag.group)
        )
      );
    Add(pTag.s.Description);
    case get_tag_type(pTag.s.Flags) of
    dt_bool:
      Add('Bool');
    DT_Real4:
      Add('Float');
    DT_Real8:
      Add('Double');
    DT_Int16:
      Add('Int16');
    dt_int32:
      add('int32');
    DT_Date:
      Add('Date');
    dt_int8:
      add('int8');
    dt_int64:
      add('int64');
    dt_uint8:
      add('uint8');
    dt_uint16:
      add('uint16');
    dt_uint32:
      add('uint32');
    dt_uint64:
      add('uint64');
    end;

    Add(key_to_string(@pTag.s.Device, sizeof(pTag.s.Device)));
    Add(pTag.s.Address);
    Add(TagValue(pTag));

    case get_tag_type(pTag.s.Flags) of
    dt_bool :
    begin
      Add('');
      Add('');
      Add('');
      Add(tag_mask(pTag.s.Flags));
      Add('');
      Add('');
      Add('');
      Add('');
      Add(pTag.s.Msg.OnMsg);
      Add(pTag.s.Msg.OffMsg);
    end;
    DT_Real4, DT_Real8, dt_int64, dt_uint64:
    begin
      Add(pTag.s.Msg.EU);
      Add(FloatToStr(pTag.s.u_max.flt));
      Add(FloatToStr(pTag.s.u_min.flt));
      Add(tag_mask(pTag.s.flags));
      Add(FloatToStr(pTag.s.u_hihi.flt));
      Add(FloatToStr(pTag.s.u_hi.flt));
      Add(FloatToStr(pTag.s.u_lo.flt));
      Add(FloatToStr(pTag.s.u_lolo.flt));
      Add('');
      Add('');
    end;
    dt_int8, DT_Int16, dt_int32, dt_uint8, dt_uint16, dt_uint32:
    begin
      Add(pTag.s.Msg.EU);
      Add(FloatToStr(pTag.s.u_max.i));
      Add(FloatToStr(pTag.s.u_min.i));
      Add(tag_mask(pTag.s.Flags));
      Add(FloatToStr(pTag.s.u_hihi.i));
      Add(FloatToStr(pTag.s.u_hi.i));
      Add(FloatToStr(pTag.s.u_lo.i));
      Add(FloatToStr(pTag.s.u_lolo.i));
      Add('');
      Add('');
    end;
    DT_Date:
    begin
      for i:=0 to 9 do
        Add('');
    end;
    else
      assert(false);
    end;
    pAttrs := TTagAttrs.Create;
    pAttrs.attrs := pTag^;
    li.Data := Pointer(pAttrs);
  end;
end;

procedure TForm1.ShowTag(n : TTreeNode);
var
  tn : TTagName;
  cr : TRtkCursor;
begin
  assert(n.Level = 3);
  tagname_by_text(tn, pchar(n.Parent.Parent.Text + '.' + n.Parent.Text + '.' + n.Text));
  cr := open_tag_f(tn);
  ShowTag(cr);
  close_handle(cr);
end;

procedure TForm1.tvChange(Sender: TObject; Node: TTreeNode);
var
  n  : TTreeNode;
  i  : integer;
begin
  lv.Items.BeginUpdate;
  try
    lv.Items.Clear;
    for i:=0 to tv.SelectionCount - 1 do
    begin
      n := tv.Selections[i];
      if n.Level<>3 then
        Continue;
      ShowTag(n);
    end;
  finally
    lv.Items.EndUpdate;
  end;
end;

procedure TForm1.SelectFields(Sender: TObject);
var
  mnu : TMenuItem;
  col : TListColumn;
  i : Integer;
begin
  {hide unselected fields in detail pane}
  mnu := Sender as TMenuItem;
  mnu.Checked := not mnu.Checked;
  for i:=0 to lv.Columns.Count - 1 do
  begin
    if lv.Column[i].Caption = mnu.Caption then
      Break;
  end;
  assert(i<lv.Columns.Count);
  if mnu.Checked then
    lv.Column[i].Width := 50
  else begin
    lv.Column[i].Width := 0;
  end;
end;

procedure TForm1.btnCancelClick(Sender: TObject);
begin
  btnClearAll.Click;
  ModalResult := mrCancel;
end;

procedure TForm1.btnClearAllClick(Sender: TObject);
begin
  lv.ClearSelection;
end;

procedure TForm1.btnSelectAllClick(Sender: TObject);
begin
  lv.SelectAll;
end;

procedure TForm1.lvDeletion(Sender: TObject; Item: TListItem);
begin
  if assigned(Item.Data) then
    TTagAttrs(Item.Data).Free;
  Item.Data := nil;
end;

procedure TForm1.cmbNameExit(Sender: TObject);
var
  cmb : TComboBox;
begin
  cmb := Sender as TComboBox;
  if not assigned(cmb) then
    Exit;
  if cmb.Items.IndexOf(cmb.Text)<0 then
    cmb.Items.Insert(0, cmb.Text);
end;

procedure LoadCombo(cmb : TComboBox; key : string);
var
  reg : TRegistry;
  mru : string;
  args : CArgs;
  i : integer;
begin
  reg := TRegistry.Create;
  cmb.Items.Clear;
  mru := '*';
  try
    reg.RootKey := HKEY_CURRENT_USER;
    reg.OpenKey('Software\\PMC 2\\PmcSeeker', true);
    mru := reg.ReadString(key + '-MRU');
  except
  end;
  reg.Free;
  if mru='' then
    mru := '*';
  parse_arg_ex(pchar(mru), args, ',;');
  for i:=0 to args.argc - 1 do
  begin
    cmb.Items.Add(args.argv[i]);
  end;
  cmb.ItemIndex := 0;
end;

procedure SaveCombo(cmb : TComboBox; key : string);
var
  reg : TRegistry;
  mru : string;
  i : integer;
  saveCount : integer;
begin
  reg := TRegistry.Create;
  try
    reg.RootKey := HKEY_CURRENT_USER;
    reg.OpenKey('Software\\PMC 2\\PmcSeeker', true);
    mru := '';
    saveCount := cmb.Items.Count;
    if saveCount > 6 then
      saveCount := 6;
    for i:=0 to saveCount - 1 do
    begin
      mru := mru + ',' + cmb.Items[i];
    end;
    reg.WriteString(key + '-MRU', mru);
  except
  end;
  reg.Free;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  LoadCombo(cmbName, 'Name');
  LoadCombo(cmbDesc, 'Description');
  LoadCombo(cmbDevice, 'Device');
  LoadCombo(cmbAddress, 'Address');
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  SaveCombo(cmbName, 'Name');
  SaveCombo(cmbDesc, 'Description');
  SaveCombo(cmbDevice, 'Device');
  SaveCombo(cmbAddress, 'Address');
end;

procedure TForm1.btnCopyClick(Sender: TObject);
var
  it : TListItem;
begin
  {clipboard}
  if not assigned(lv.Selected) then
    Exit;
  it := lv.Selected;
  Clipboard.SetTextBuf(Pchar(it.SubItems.Strings[0] + '.' + it.Caption));
end;

procedure TForm1.lvDblClick(Sender: TObject);
begin
  if assigned(lv.ItemFocused) then
    lv.ItemFocused.Selected := true;
  btnOk.Click;
end;

procedure TForm1.tvDblClick(Sender: TObject);
var
  nd : TTreeNode;
begin
  {}
  nd := tv.Selected;
  if nd.Level <> 3 then
    Exit;
  tv.ClearSelection;
  nd.Selected := true;
  btnOk.Click;
end;

procedure TForm1.cmbNameKeyPress(Sender: TObject; var Key: Char);
begin
  if Ord(Key)=VK_Return then
    btnSearch.Click;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
  i : Integer;
  pTag : PRtkTag;
  it : TListItem;
  tn : TTagName;
begin
  if not Visible then
    Exit;
  lv.Items.BeginUpdate;
  for i:=0 to lv.Items.Count - 1 do
  begin
    it := lv.Items.Item[i];
    with it do
    begin
      tagname_by_text(tn, PChar(SubItems[0] + '.' + Caption));
      pTag := query_tag_f(@tn);
      if pTag<>nil then
        SubItems[5] := TagValue(pTag);
    end;
  end;
  lv.Items.EndUpdate;
end;

procedure TForm1.mnuAutoRefreshClick(Sender: TObject);
begin
  mnuAutoRefresh.OnClick := nil;
  mnuAutoRefresh.Checked := not mnuAutoRefresh.Checked;
  Timer1.Enabled := (Sender as TMenuItem).Checked;
  mnuAutoRefresh.OnClick := mnuAutoRefreshClick;
end;

procedure TForm1.lvColumnClick(Sender: TObject; Column: TListColumn);
begin
  if iSortOrder = Column.Index then
    bSortReverse := not bSortReverse;
  iSortOrder := Column.Index;
  (Sender as TListView).AlphaSort;
end;

procedure TForm1.lvCompare(Sender: TObject; Item1, Item2: TListItem;
  Data: Integer; var Compare: Integer);
begin
  if bInSearch then
  begin
    Compare := Item1.Index - Item2.Index;
    Exit;
  end;
  if iSortOrder=0 then
    Compare := CompareText(Item1.Caption, Item2.Caption)
  else
    Compare := CompareText(Item1.SubItems[iSortOrder-1], Item2.SubItems[iSortOrder-1]);
  if bSortReverse then
    Compare := -Compare;
end;

end.


