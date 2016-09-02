{
  2005-8-29 增加了查找的历史记录功能，增加了跳转功能。
}
unit untFormSearch;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfrmSearch = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    GroupBox1: TGroupBox;
    RadioButton2: TRadioButton;
    RadioButton1: TRadioButton;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    lb: TListBox;
    Button4: TButton;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure lbDblClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSearch: TfrmSearch;

implementation

{$R *.dfm}
uses
  common, f8, libcobj, rtkdefs, f8ui, blkLayout;

procedure search_blk(s : string);
begin
end;

function search_var_in_blk(p : PTreeNode; bFirst : __bool; con : f8_uint):f8_bool;cdecl;
var
  b : PBlk;
  pin : Pblk_pin_t;
  v : Pblk_var_t;
  buf : array[0..f8_max_path-1] of char;
begin
  b := TreeNode2Blk(p);
  pin := b^.vtbl.first_pin(b);
  while assigned(pin) do
  begin
    case pin^._class of
    PIN_CLASS_DI,PIN_CLASS_DO:
      begin
        v := query_var_by_pin(pin);
        if assigned(v) and (stricomp(v^.name, pchar(con))=0) then
        begin
          //IBlk_path_by_blk(b, buf, sizeof(buf));
          IBLk_path_by_pin(pin,buf,sizeof(buf));
          frmSearch.lb.AddItem(buf, nil);
        end;
      end;
    end;
    pin := b^.vtbl.next_pin(pin);
  end;
  result := __true;
end;

procedure search_var(s : string);
begin
  ITreeNode_traverse(
    Blk2TreeNode(g_sections),
    @search_var_in_blk,
    __true,
    ITreeNode_pre_order,
    f8_uint(pchar(s))
    );
end;


procedure TfrmSearch.Button1Click(Sender: TObject);
begin
  lb.Clear;
end;

procedure TfrmSearch.Button2Click(Sender: TObject);
begin
  lb.AddItem('----------------------', nil);
  if frmSearch.radioButton2.Checked then
  begin
    lb.AddItem('搜索变量 ' + edit1.text, nil);
    search_var(edit1.Text)
  end else begin
    lb.addItem('搜索功能块 ' + edit1.text, nil);
    search_blk(edit1.text);
  end;
end;

procedure TfrmSearch.Button3Click(Sender: TObject);
begin
  close;
end;

procedure TfrmSearch.Button4Click(Sender: TObject);
begin
  lbDblClick(sender);
end;

procedure TfrmSearch.lbDblClick(Sender: TObject);
var
  name : string;
  blk : Pblk;
  it : TBlk;
  pit : TPin;
  pt : tPoint;
  i : integer;
  p : Pblk_pin_t;
begin
  if lb.itemIndex < 0 then
    exit;
  //
  name := lb.Items[lb.itemindex];
  // blk := IBlk_blk_by_path(g_sections, pchar(name));
  p := IBLK_pin_by_path(g_sections,pchar(name));
  if not assigned(p) then
    exit;
  blk := p^.blk;
  g_layout.bind(delphi_parent(blk));
  // activate the parent block view
  it := TBlk(g_layout.ItemByPointer(blk));
  pit := it.getPin(p);
  if not assigned(pit) then
    exit;
  close;
  g_layout.PrepareCanvas;
  pt.x := pit.Left;
  pt.Y := pit.Top;
  g_layout.makeVisible(pt);
  for i:=0 to 6 do
  begin
    pit.Selected := not pit.Selected;
    g_layout.Refresh;
    sleep(100);
  end;
  g_layout.ReleaseCanvas;
end;

procedure TfrmSearch.FormShow(Sender: TObject);
begin
  //
end;

end.
