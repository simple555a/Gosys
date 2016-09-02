unit untFormBlkBrowser;

interface

{$WARN UNIT_PLATFORM OFF}

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, ComCtrls,
  common,
  f8,
  ShellCtrls,
  BlkLayout;

type
  TfrmBlkBrowser = class(TForm)
    Panel1: TPanel;
    Panel2: TPanel;
    Label2: TLabel;                   
    Panel3: TPanel;
    Panel4: TPanel;
    Label1: TLabel;
    lv: TListView;
    Button1: TButton;
    procedure FormCreate(Sender: TObject);
    procedure lvStartDrag(Sender: TObject; var DragObject: TDragObject);
    procedure lvChange(Sender: TObject; Item: TListItem;
      Change: TItemChange);
  private
    { Private declarations }
    lo : TBlkLayout;
  public
    { Public declarations }
  end;

var
  frmBlkBrowser: TfrmBlkBrowser;

implementation
var
  blk : PBlk;
{$R *.dfm}

procedure TfrmBlkBrowser.FormCreate(Sender: TObject);
var
  fd : WIN32_FIND_DATA;
  fh : THandle;
  it : TListItem;
  ok : boolean;
begin
  // load blocks
  fh := FindFirstFile(pchar(g_apppath + '*.blk'), fd);
  if fh = invalid_handle_value then
    Exit;
  ok := true;
  while ok do
  begin
    it := lv.Items.Add;
    it.Caption := '';
    it.SubItems.Add(copy(fd.cFileName, 1, strlen(fd.cfilename)-4));
    ok := findnextfile(fh, fd);
  end;
  windows.findclose(fh);

  lo := tblklayout.Create(self);
  lo.FKernel := g_kernel;
  lo.Parent := panel1;
  lo.Scale := 1.0;
  lo.Align := alClient;
  lo.Visible := true;
  lo.SendToBack;
end;

procedure TfrmBlkBrowser.lvStartDrag(Sender: TObject;
  var DragObject: TDragObject);
begin
  //
end;

procedure TfrmBlkBrowser.lvChange(Sender: TObject; Item: TListItem;
  Change: TItemChange);
begin
  if not assigned(lv.Selected) then
    exit;
  if assigned(blk) then
  begin
    lo.Clean;
    delete_blk(blk);
  end;
  if blklib_load(pchar(lv.selected.SubItems[0]), blk, 0)<>F8_SUCCESS then
    exit;
  lo.Bind(blk);
  lo.Invalidate;
end;

end.
