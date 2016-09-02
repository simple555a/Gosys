unit untFormMain2;

interface

uses Windows, shellapi, Messages, Classes, Graphics, Forms, Controls, Menus,
  Dialogs, StdCtrls, Buttons, ExtCtrls, ComCtrls, ImgList, StdActns,
  ActnList, ToolWin, Grids, ValEdit, XPMan, ActnMan, ActnColorMaps,
  f8,
  libcobj,
  untFormBlkBrowser,
  common,
  rtknet,
  rtkdefs,
  f8ui,
  untFormSelTarget,
  untFormblkattrib,
  untformpinattrib,
  untFormTargetConfig,
  untFormOptions,
  BlkLayout;

type
  TSplitter = class(ExtCtrls.TSplitter)
  constructor Create(AOwner : TComponent);override;
  procedure SetOnClick(proc : TNotifyEvent); 
  end;

  TfrmMain = class(TForm)
  OpenDialog: TOpenDialog;
  SaveDialog: TSaveDialog;
  ActionList1: TActionList;
  FileNew1: TAction;
  FileOpen1: TAction;
  FileSave1: TAction;
  FileSaveAs1: TAction;
  FileExit1: TAction;
  EditCut1: TEditCut;
  EditCopy1: TEditCopy;
  EditPaste1: TEditPaste;
  HelpAbout1: TAction;
    il2: TImageList;
  MainMenu1: TMainMenu;
  FileNewItem: TMenuItem;
  FileOpenItem: TMenuItem;
  FileSaveItem: TMenuItem;
  FileSaveAsItem: TMenuItem;
  FileExitItem: TMenuItem;
  Edit1: TMenuItem;
  CutItem: TMenuItem;
  CopyItem: TMenuItem;
  PasteItem: TMenuItem;
  Help1: TMenuItem;
  HelpAboutItem: TMenuItem;
    actInsertBlock: TAction;
    actStartBlock: TAction;
    tmr: TTimer;
    actDelete: TAction;
    actZoomIn: TAction;
    actZoomOut: TAction;
    stb: TStatusBar;
    pm: TPopupMenu;
    Selectall1: TMenuItem;
    Font1: TMenuItem;
    N8: TMenuItem;
    P1: TMenuItem;
    D1: TMenuItem;
    U1: TMenuItem;
    N9: TMenuItem;
    O2: TMenuItem;
    F1: TMenuItem;
    actOnline: TAction;
    actOffline: TAction;
    actDownload: TAction;
    actUpload: TAction;
    actCheckVer: TAction;
    N10: TMenuItem;
    N11: TMenuItem;
    N12: TMenuItem;
    Saveascblk1: TMenuItem;
    actArchive: TAction;
    actProperty: TAction;
    actSearch: TAction;
    actStopBlock: TAction;
    actSyncDir: TAction;
    R1: TMenuItem;
    F2: TMenuItem;
    N2: TMenuItem;
    C1: TMenuItem;
    P2: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    N6: TMenuItem;
    actHelp: TAction;
    actInvertPin: TAction;
    N7: TMenuItem;
    I1: TMenuItem;
    actUpOneLevel: TAction;
    actOptions: TAction;
    N13: TMenuItem;
    ControlBar1: TControlBar;
    ToolBar2: TToolBar;
    ToolButton23: TToolButton;
    ToolButton24: TToolButton;
    ToolButton25: TToolButton;
    ToolButton26: TToolButton;
    ToolButton27: TToolButton;
    ToolButton28: TToolButton;
    actLeftJustify: TAction;
    actRightJustify: TAction;
    actTopJustify: TAction;
    actBottomJustify: TAction;
    ToolButton7: TToolButton;
    ToolBar3: TToolBar;
    ToolButton12: TToolButton;
    ToolButton20: TToolButton;
    ToolButton29: TToolButton;
    ToolButton30: TToolButton;
    ToolButton31: TToolButton;
    ToolButton32: TToolButton;
    ToolButton33: TToolButton;
    ToolBar4: TToolBar;
    ToolButton34: TToolButton;
    ToolButton35: TToolButton;
    ToolButton36: TToolButton;
    ToolButton37: TToolButton;
    ToolButton38: TToolButton;
    ToolButton39: TToolButton;
    ToolButton40: TToolButton;
    ToolButton41: TToolButton;
    ToolButton42: TToolButton;
    Panel1: TPanel;
    pnlDir: TPanel;
    tv: TTreeView;
    Splitter1: TSplitter;
    actTargetConfig: TAction;
    Edit2: TEdit;
    actExport: TAction;
    actExport1: TMenuItem;
    N1: TMenuItem;
    N14: TMenuItem;
    N15: TMenuItem;
    N16: TMenuItem;
    actViewBrowser: TAction;
    actViewDatabase: TAction;
    actViewBrowser1: TMenuItem;
    actViewDatabase1: TMenuItem;
    actViewMemory: TAction;
    N17: TMenuItem;
    actLink: TAction;
    N18: TMenuItem;
    N19: TMenuItem;
    N20: TMenuItem;
    N21: TMenuItem;
    F3: TMenuItem;
    ToolBar1: TToolBar;
    ToolButton1: TToolButton;
    edtSearch: TEdit;
  procedure FileNew1Execute(Sender: TObject);
  procedure FileSave1Execute(Sender: TObject);
  procedure FileExit1Execute(Sender: TObject);
  procedure HelpAbout1Execute(Sender: TObject);
  procedure tvChange(Sender: TObject; Node: TTreeNode);
    procedure FormCreate(Sender: TObject);
    procedure actInsertBlockExecute(Sender: TObject);
    procedure tvExpanding(Sender: TObject; Node: TTreeNode;
      var AllowExpansion: Boolean);
    procedure tmrTimer(Sender: TObject);
    procedure actDeleteExecute(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FileOpen1Execute(Sender: TObject);
    procedure Selectall1Click(Sender: TObject);
    procedure actOnlineExecute(Sender: TObject);
    procedure actDownloadExecute(Sender: TObject);
    procedure actUploadExecute(Sender: TObject);
    procedure actCheckVerExecute(Sender: TObject);
    procedure actOfflineExecute(Sender: TObject);
    procedure actArchiveExecute(Sender: TObject);
    procedure actZoomInExecute(Sender: TObject);
    procedure actZoomOutExecute(Sender: TObject);
    procedure actPropertyExecute(Sender: TObject);
    procedure actSyncDirExecute(Sender: TObject);
    procedure FileSaveAsItemClick(Sender: TObject);
    procedure EditCopy1Execute(Sender: TObject);
    procedure EditPaste1Execute(Sender: TObject);
    procedure EditCut1Execute(Sender: TObject);
    procedure actStartBlockExecute(Sender: TObject);
    procedure actStopBlockExecute(Sender: TObject);
    procedure actHelpExecute(Sender: TObject);
    procedure actInvertPinExecute(Sender: TObject);
    procedure actUpOneLevelExecute(Sender: TObject);
    procedure actOptionsExecute(Sender: TObject);
    procedure actLeftJustifyExecute(Sender: TObject);
    procedure actRightJustifyExecute(Sender: TObject);
    procedure actTopJustifyExecute(Sender: TObject);
    procedure actBottomJustifyExecute(Sender: TObject);
    procedure Panel2MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure ControlBar1DockOver(Sender: TObject; Source: TDragDockObject;
      X, Y: Integer; State: TDragState; var Accept: Boolean);
    procedure Panel1DockOver(Sender: TObject; Source: TDragDockObject; X,
      Y: Integer; State: TDragState; var Accept: Boolean);
    procedure tvDblClick(Sender: TObject);
    procedure tvStartDrag(Sender: TObject; var DragObject: TDragObject);
    procedure tvEndDrag(Sender, Target: TObject; X, Y: Integer);
    procedure FormDockOver(Sender: TObject; Source: TDragDockObject; X,
      Y: Integer; State: TDragState; var Accept: Boolean);
    procedure pnlDirEndDock(Sender, Target: TObject; X, Y: Integer);
    procedure pnlDirDblClick(Sender: TObject);
    procedure actTargetConfigExecute(Sender: TObject);
    procedure Edit2KeyPress(Sender: TObject; var Key: Char);
    procedure actExportExecute(Sender: TObject);
    procedure tvDeletion(Sender: TObject; Node: TTreeNode);
    procedure actStartBlockUpdate(Sender: TObject);
    procedure actViewBrowserExecute(Sender: TObject);
    procedure actExportUpdate(Sender: TObject);
    procedure actInvertPinUpdate(Sender: TObject);
    procedure actLinkUpdate(Sender: TObject);
    procedure actLinkExecute(Sender: TObject);
    procedure actInsertBlockUpdate(Sender: TObject);
    procedure actLeftJustifyUpdate(Sender: TObject);
    procedure EditPaste1Update(Sender: TObject);
    procedure EditCopy1Update(Sender: TObject);
    procedure actOnlineUpdate(Sender: TObject);
    procedure actOfflineUpdate(Sender: TObject);
    procedure actSearchExecute(Sender: TObject);
  private
    (* Private declarations en*)
    (* these two coordinates are of the last mouse event *)
    m_CurBlk : PBlk;

    procedure oneditblock(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
    procedure oneditpin(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
    procedure onAddLink(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
    procedure onaddblock(sender : tblklayout; action:tblknotification; obj, obj1 :pointer);
    procedure onBind(sender : tblklayout; action:tblknotification; obj, obj1 :pointer);

    procedure LoadBlkLibrary(nd : TTreeNode);
    procedure ReloadUserBlocks;
    procedure ShowProperty(ob : TUiItem);
    procedure ShowBlkProperty(blk : PBlk);
    procedure ShowPinProperty(p : Pblk_pin_t);
    function  CurNode:TTreeNode;
    procedure Splitter1OnClick(Sender : TObject);
  public
    procedure ReloadKernel;
    procedure RefreshBlock(blk : PBlk);
    function  CurBlk:PBlk;
    procedure ShowBlockLayout(blk : PBlk);
    procedure UpdateStatus;
  (* Public declarations *)
  end;

var
  frmMain: TfrmMain;

implementation

uses about, sysutils, libutils,
   activex,
   untCopyPaste,
   comobj;

const
  LibMagic = 3355;
  ProgramMagic = 3354;
  TargetConfigMagic = 3356;
  InactiveMagic = 3357;
  LibDirMagic = 3358;

type
  PNodeAttribute = ^TNodeAttribute;
  TNodeAttribute = record
    t : f8_u16;
    p : Pointer;
    pv: array [0..3] of Pointer;
  end;

var
  oldPuts : TUtilsPutsProc;
  g_UserBlocks : TTreeNode;
  frmSplash : TAboutBox;
  g_MainThread : dword;
{$R *.dfm}

procedure handleAdapterEvent(a : PKAdapter; event, p0, p1 : f8_int);cdecl;
begin
  case event of
  AE_STATUS_CHANGED:
  begin
    g_layout.FbOnline := isOnline;
    g_layout.UiInvalidate;
  end;
  AE_IN_PROGRESS:
    ;
  end;
end;

function _my_puts(level : integer; s : pchar):integer;stdcall;
begin
  result := oldPuts(level, s);
end;

function _my_puts_1(level : integer; s : pchar):integer;stdcall;
begin
  if assigned(frmSplash) and (GetCurrentThreadId=g_MainThread) then
  begin
    frmSplash.Label1.Caption := s;
    frmSplash.Update;
    Sleep(1);
  end;
  result := oldPuts(level, s);
end;

function _find_subitem(nd : TTreeNode; name : pchar):TTreeNode;
begin
  result := nd.getFirstChild;
  while assigned(result) do
  begin
    if StrIComp(Pchar(result.Text), name)=0 then
      exit;
    result := result.getNextSibling;
  end;
  result := nil;
end;

function _node_by_path(nd : TTreeNode; path : pchar):TTreeNode;
var
  args : TArgs;
  i    : integer;
begin
  Result := nd;
  parse_arg_ex(path, args, '/');
  for i:=0 to args.argc - 1 do
  begin
    Result := _find_subitem(Result, args.argv[i]);
    if not assigned(nd) then
      Exit;
  end;
end;

procedure AssociateNodeWith4(n : TTreeNode; t : f8_u16; p,p1,p2,p3,p4 : Pointer);
var
  a : PNodeAttribute;
begin
  new (a);
  a.t := t;
  a.p := p;
  a.pv[0] := p1;
  a.pv[1] := p2;
  a.pv[2] := p3;
  a.pv[3] := p4;
  n.Data := a;
end;

procedure AssociateNodeWith(n : TTreeNode; t : f8_u16; p : Pointer);
begin
  AssociateNodeWith4(n,t,p,nil,nil,nil,nil);
end;

{
  this proc adds to the block tree view
}
function _mount_blk_at(blk : pblk; nd : TTreeNode):TTreeNode;
var
  b : pblk;
  mend : TTreeNode;
begin
  mend := nd.owner.addchild(nd, IBlk_name(blk));
  AssociateNodeWith(mend, ProgramMagic, blk);
  if blk.h.magic=CBLK_MAGIC then
  begin
    mend.ImageIndex := 34;
    mend.HasChildren := true;
    b := delphi_firstsub(blk);
    while assigned(b) do
    begin
      _mount_blk_at(b, mend);
      b := delphi_next(b);
    end;
  end else begin
    mend.ImageIndex := 36;
    mend.SelectedIndex := 36;
    mend.HasChildren := false;
  end;
  mend.SelectedIndex := mend.ImageIndex;
  result := mend;
end;

function _mount_blk(blk : pblk):TTreeNode;
var
  nd : TTreeNode;
  path : string;
begin
  path := GetBlkPath(delphi_parent(blk));
  nd := frmMain.tv.Items.GetFirstNode;
  nd := _node_by_path(nd, pchar(path));
  result := _mount_blk_at(blk, nd);
end;

procedure TfrmMain.FileNew1Execute(Sender: TObject);
begin
  KProxyShellWithError(Handle,'create');
  KProxyShellWithError(Handle,'mount path=/ name=section_1 type=base.blank rw');
  g_bUntitled := true;
  g_projname := 'Untitled';
  caption := g_projname + ' - ' + g_layout.path;
  ReloadKernel;
  ReloaduserBlocks;
end;

procedure TfrmMain.FileSave1Execute(Sender: TObject);
begin
  if g_bUntitled then
    FileSaveAsItemClick(sender)
  else
    KProxyShellWithError(Handle,'save');
end;

procedure TfrmMain.FileExit1Execute(Sender: TObject);
begin
  Close;
end;

procedure TfrmMain.HelpAbout1Execute(Sender: TObject);
begin
  AboutBox.ShowModal;
end;

(*
  show current selected block
*)
procedure TfrmMain.tvChange(Sender: TObject; Node: TTreeNode);
var
  blk : pblk;
  ublk : TBlk;
  nblk : pblk;
  a : PNodeAttribute;
begin
  // show current block
  a := PNodeAttribute(node.Data);
  if not assigned(a) then
    Exit;
  if (a.t <> ProgramMagic) then
    Exit;
  blk := Pblk(a.p);
  if not assigned(blk) then
    exit;
  g_layout.PrepareCanvas;
  if blk.h.magic = BBLK_MAGIC then
  begin
    nblk := delphi_parent(blk);
    if CurBlk<>nblk then
      ShowBlockLayout(nblk);
    m_CurBlk := nblk;
    ublk := g_Layout.GetBlk(blk);
    g_Layout.DeselectAll;
    if assigned(ublk) then
    begin
      g_Layout.Select(ublk);
      g_layout.Hilight(ublk);
    end
  end else begin
    ShowBlockLayout(blk);
    m_CurBlk := blk;
  end;
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.FormCreate(Sender: TObject);
var
  nd : TTreeNode;
begin
  if not GetDebug then
  begin
    frmSplash := TAboutBox.Create(nil);
    frmSplash.Visible := true;
    frmSplash.Update;
  end;

  g_MainThread := GetCurrentThreadId();
  oldPuts := TUtilsPutsProc(pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^);
  // pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^ := dword(@_my_puts);
  if not GetDebug then
    pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^ := dword(@_my_puts_1);

  init_network(RTK_SKIP_STANDARD_BUSES, nil);
  kproxy_init;
  g_kernel := kproxy_get_kernel;
  g_adapter := kproxy_get_adapter;
  g_adapter^.eventSink := handleAdapterEvent;
  g_sections := ke_get_blk(g_kernel, nil);

  edit2.Visible := GetDebug;

  g_projname := 'Untitled';
  g_bUntitled := true;

  g_Layout := tblklayout.Create(self);
  g_Layout.FKernel := g_kernel; 
  g_Layout.Parent := self;
  g_Layout.Scale := 1.0;
  g_layout.Align := alClient;
  g_layout.OnEditBlock := oneditblock;
  g_layout.oneditpin := oneditpin;
  g_layout.OnAddLink := onAddLink;
  g_layout.OnAddBlock := onaddblock;
  g_layout.OnBind := onbind;
  g_layout.SendToBack;
  splitter1.SetOnClick(Splitter1OnClick);
  // g_layout.m_bshowgrid := false;
  g_layout.PopupMenu := pm;

  nd := tv.Items.AddChild(nil, '控制策略');
  AssociateNodeWith(nd, ProgramMagic, ke_get_blk(g_kernel, nil));
  nd.Selected := true;

  nd := tv.Items.AddChild(nil, '策略库');
  AssociateNodeWith(nd, InactiveMagic, nil);
  LoadBlkLibrary(nd);

  nd := tv.Items.AddChild(nil, '目标系统设置');
  nd.ImageIndex := 2;
  nd.SelectedIndex := 2;
  AssociateNodeWith(nd, TargetConfigMagic, pointer(TargetConfigMagic));

  if not GetDebug then
  begin
    Sleep(1500);
    frmSplash.Visible := false;
    frmSplash.Free;
    Sleep(800);
    frmSplash := nil;
  end;
end;

procedure TfrmMain.actInsertBlockExecute(Sender: TObject);
var
  nb : tnewblock;
begin
  if g_layout.Readonly then
    exit;
  g_layout.PrepareCanvas();
  nb := tnewblock.Create;
  nb.Blktype := 'base.blank';
  nb.Pt := g_layout.InsertionPoint;
  OnAddBlock(g_layout,evtAddBlock,nb,nil);
  nb.Free;
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.tvExpanding(Sender: TObject; Node: TTreeNode;
  var AllowExpansion: Boolean);
var
  blk : PBlk;
  a : PNodeAttribute;
begin
  //
  a := PNodeAttribute(Node.Data);
  if a.t <> ProgramMagic then
    Exit;
  blk := PBlk(Node.Data);
  if not assigned(blk) then
    Exit;
  if blk.h.magic <> CBLK_MAGIC then
    Exit;
  if blk.h.funcs_or_subs=0 then
    Node.HasChildren := false;
end;

procedure TfrmMain.tmrTimer(Sender: TObject);
begin
  try
    UpdateStatus;
    if isOnline then
    begin
      g_Layout.UiInvalidateChanged;
    end;
  except
  end;
end;

function FileName(const dataObj : IDataObject):string;
var
  fetc : FormatEtc;
  med  : StgMedium;
  p    : pchar;
begin
  ZeroMemory(@fetc, sizeof(fetc));
  fetc.cfFormat := 49158; // filename
  fetc.tymed := TYMED_HGLOBAL;
  Result := '';
  if (dataObj.GetData(fetc, med) = S_OK) then
  begin
    p := GlobalLock(med.hGlobal);
    Result := p;
    GlobalFree(med.hGlobal);
  end;
end;


procedure tfrmmain.oneditblock(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
begin
  showproperty(TUiItem(obj));
end;

procedure tfrmmain.oneditpin(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
begin
  showproperty(TUiItem(obj));
end;

procedure TfrmMain.actDeleteExecute(Sender: TObject);
var
  it : TUiItem;
  ublk : TBlk;
  ulk : TLink;
  uep : TExtPin;
  path : array [0..F8_MAX_PATH-1] of char;
  cmd : string;
  nd : TTreeNode;
begin
  if g_Layout.SelCount=0 then
    Exit;
  if MessageBox(Handle, '确定要删除选中的这些对象吗?', Pchar(Application.Title),
    MB_YESNO)<>IDYES then
    Exit;

  g_layout.PrepareCanvas();

  g_layout.Invalidate;

  while g_Layout.SelCount<>0 do
  begin
    it := g_Layout.GetFirstSelection;
    if it is TBlk then
    begin
      ublk := it as TBlk;
      IBlk_path_by_blk(ublk.blk, path, sizeof(path));
      cmd := 'umount ' + path;
    end else if it is TLink then
    begin
      ulk := it as TLink;
      IBlk_path_by_pin(ulk.Flink.t, path, sizeof(path));
      cmd := 'ulink ' + path;
    end else if it is TExtPin then
    begin
      uep := it as TExtPin;
      IBlk_path_by_pin(uep.op.p.ulink, path, sizeof(path));
      cmd := 'uexp ' + path;
    end else begin
      g_Layout.Deselect(it);
      cmd := '';
    end;

    if cmd<>'' then
    begin
      if KProxyShellWithError(Handle,pchar(cmd))<>0 then
      // if kproxy_shell(pchar(cmd))<>0 then
      begin
        Exit;
      end;
      if it is TBlk then
      begin
        nd := _node_by_path(tv.Items.GetFirstNode, path);
        nd.Delete;
      end;
      g_Layout.DropItem(it);
    end;
  end;

  g_layout.ReleaseCanvas;
end;

function TfrmMain.CurBlk:PBlk;
begin
  Result := m_CurBlk;
end;

(*
  if mouse was down on a single UI item, drag it
*)
procedure TfrmMain.FormDestroy(Sender: TObject);
begin
  tmr.Enabled := false;
  g_layout.Free;
  KProxyShellWithError(Handle,'close');
  kproxy_uninit;
  uninit_network;
end;

procedure TfrmMain.FileOpen1Execute(Sender: TObject);
var
  cmd : string;
begin
  if OpenDialog.Execute then
  begin
    cmd := 'open ' + OpenDialog.FileName;
    if KProxyShellWithError(Handle,pchar(cmd))=F8_SUCCESS then
    begin
      g_projname := opendialog.filename;
      g_bUntitled := false;
    end;
    ReloadKernel;
    ReloaduserBlocks;
  end;
end;

{
  rebuild block tree view, this occurs when a new project is opened.
}
procedure TfrmMain.ReloadKernel;
var
  rt : TTreeNode;
  blk : PBlk;
begin
  tv.Items.BeginUpdate;
  g_Layout.Clean;

  // this will delete all nodes except the root
  rt := tv.Items.GetFirstNode;
  rt.DeleteChildren;

  blk := delphi_firstsub(g_sections);
  while assigned(blk) do
  begin
    _mount_blk_at(blk, rt);
    blk := delphi_next(blk);
  end;

  ShowBlockLayout(g_sections);

  rt.Expand(false);

  tv.Items.EndUpdate;
end;

procedure TfrmMain.RefreshBlock(blk: PBlk);
begin
  // show current block
  if not assigned(blk) then
    exit;
  Caption := GetBlkPath(blk);
end;

procedure TfrmMain.Selectall1Click(Sender: TObject);
var
  r : TRect;
begin
  r.Left := -500;
  r.Top := 500;
  r.Right := default_canvas_width;
  r.bottom := -DEFAULT_CANVAS_HEIGHT;
  g_layout.PrepareCanvas;
  g_Layout.Select(r);
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.actOnlineExecute(Sender: TObject);
var
  frm : TfrmSelTarget;
  {
  pi  : PROCESS_INFORMATION;
  si  : STARTUPINFO;
  }
begin
  frm := TfrmSelTarget.Create(Self);
  frm.ShowModal;
  if frm.ModalResult=mrOk then
  begin
  {
    if not check_running('f8 simulator') then
    begin
      ZeroMemory(@si, sizeof(si));
      si.cb := sizeof(si);
      if CreateProcess(
        nil,
        pchar(g_AppPath + 'fsimnt.exe'),
        nil,nil,false,0,nil,nil,
        si,
        pi
        ) then
      begin
        WaitForInputIdle(pi.hProcess, 1000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      end;
      Sleep(2000);
    end;
  }
    if KProxyShellWithError(Handle,pchar('online ' + frm.cmbTarget.Text),2000) = F8_SUCCESS then
    begin
      tmr.Enabled := true;
    end;
  end;
  frm.Free;
end;

procedure TfrmMain.actDownloadExecute(Sender: TObject);
begin
  KProxyShellWithError(Handle,'dload');
end;

procedure TfrmMain.actUploadExecute(Sender: TObject);
begin
  KProxyShellWithError(Handle,'uload');
end;

procedure TfrmMain.actCheckVerExecute(Sender: TObject);
begin
  KProxyShellWithError(Handle,'ver');
end;

procedure TfrmMain.actOfflineExecute(Sender: TObject);
begin
  KProxyShellWithError(Handle,'offline');
end;

function _e_ent(ent : Pblk_ent_t; context : __uint):__bool;cdecl;
var
  n,rt:TTreeNode;
begin
  rt := TTreeNode(context);
  n := rt.Owner.AddChild(rt, ent.name);
  if ent.magic = bblk_magic then
    n.ImageIndex := 57
  else
    n.ImageIndex := 56;
  n.SelectedIndex := n.ImageIndex;
  AssociateNodeWith4(n, LibMagic, ent.name, ent.fileName, nil,nil,nil);
  Result := true;
end;

function _e_dir(ent : Pblk_dir_t;context : __uint):__bool;cdecl;
var
  n,rt : TTreeNode;
begin
  rt := TTreeNode(context);
  if string(ent.name)='user' then
    n := g_UserBlocks
  else
    n := rt.Owner.AddChild(rt, ent.name);
  AssociateNodeWith(n, LibDirMagic, ent.name);
  n.ImageIndex := 58;
  n.SelectedIndex := 59;
  enumerate_blk_ent(_e_ent,ent.name, __uint(n));
  Result := true;
end;

procedure TfrmMain.LoadBlkLibrary(nd : TTreeNode);
begin
  tv.Items.BeginUpdate;

  g_UserBlocks := nd.Owner.AddChild(nd, 'user');
  g_UserBlocks.ImageIndex := 58;
  g_UserBlocks.SelectedIndex := 59;
  AssociateNodeWith(g_userBlocks, InactiveMagic, nil);

  enumerate_blk_dir(_e_dir, __uint(nd));
  tv.Items.EndUpdate;
end;

procedure TfrmMain.actArchiveExecute(Sender: TObject);
var
  name : string;
begin
  //
  if not Inputquery(Application.Title, '输入库功能块名', name) then
    exit;
  if not is_valid_blk_name(pchar(name)) then
  begin
    messagebox(handle, pchar(name + ' 不是一个合法的功能块名称'), '错误', mb_ok);
    exit;
  end;
  KProxyShellWithError(
    Handle,
    pchar('lib ' + GetBlkPath(g_layout.Block) + ' ' + name)
    );
  //
  ReloadUserBlocks;
end;

procedure TfrmMain.onAddLink(Sender: TBlkLayout;
  action: TBlkNotification; obj, obj1: pointer);
var
  p1, p2 : tpin;
  cmd : string;
  p  : pblk_pin_t;
  lk : pcblk_link_t;
  ulk : tlink;
begin
  p1 := tpin(obj);
  p2 := tpin(obj1);
  p1.UiInvalidate;
  p2.UiInvalidate;
  cmd := 'link ' + p1.FPinPath + ' ' + p2.Fpinpath;
  if KProxyShellWithError(Handle,pchar(cmd))<>F8_SUCCESS then
    exit;
  if p1.FbIsOutput then
    p := p2.p
  else
    p := p1.p;
  lk := Icblk_first_connection(p);
  assert(assigned(lk));
  ulk := g_layout.AddLink(lk);
  ulk.UiInvalidate;
end;

procedure TfrmMain.onaddblock(sender: tblklayout; action: tblknotification;
  obj, obj1: pointer);
var
  o : tnewblock;
  cmd : string;
  blk : pblk;
  ublk : Tblk;
  name : string;
begin
  o := tnewblock(obj);
  name := UniqueName(g_layout.block);
  cmd := 'mount type="' + o.blktype + '" path=' + g_layout.path + ' name=' +
    name + ' rw';
  if KProxyShellWithError(Handle,pchar(cmd))<>F8_SUCCESS then
    exit;
  blk := iblk_blk_by_path(g_layout.block, pchar(name));
  assert(assigned(blk));
  setblkpos(blk, o.pt);
  ublk := g_layout.addblock(blk);
  ublk.uiinvalidate;
  _mount_blk_at(blk, curNode);
end;

procedure TfrmMain.actZoomInExecute(Sender: TObject);
begin
  g_layout.Scale := g_layout.Scale * 1.1;
end;

procedure TfrmMain.actZoomOutExecute(Sender: TObject);
begin
  g_layout.Scale := g_layout.Scale / 1.1;
end;

procedure TfrmMain.actPropertyExecute(Sender: TObject);
begin
  showproperty(g_layout.hilighted);
end;

procedure TfrmMain.ShowProperty(ob: TUiItem);
var
  blk : Pblk;
  p : Pblk_pin_t;
begin
  p := nil;
  blk := nil;
  if not assigned(ob) then
    blk := g_layout.Block
  else if ob is TPin then
    p := tpin(ob).p
  else if ob is TBlk then
    blk := TBlk(ob).blk;

  if assigned(blk) then
    ShowBlkProperty(blk)
  else if assigned(p) then
    ShowPinProperty(p);
end;

procedure TfrmMain.actSyncDirExecute(Sender: TObject);
var
  nd : TTreeNode;
begin
  nd := _node_by_path(tv.Items.GetFirstNode, pchar(g_layout.path));
  if assigned(nd) then
  begin
    nd.MakeVisible;
    tv.OnChange := nil;
    nd.Selected := true;
    tv.OnChange := tvChange;
  end;
end;

function TfrmMain.CurNode: TTreeNode;
begin
  result := _node_by_path(tv.Items.GetFirstNode, pchar(g_layout.path));
end;

procedure TfrmMain.ShowBlockLayout(blk: PBlk);
begin
  g_layout.Bind(blk);
end;

procedure TfrmMain.onBind(sender: tblklayout; action: tblknotification;
  obj, obj1: pointer);
begin
  Caption := g_projname + ' - ' + g_layout.Path;
end;

procedure TfrmMain.FileSaveAsItemClick(Sender: TObject);
var
  cmd : string;
begin
  if savedialog.Execute then
  begin
    cmd := 'save ' + savedialog.FileName;
    if KProxyShellWithError(Handle,pchar(cmd))=F8_SUCCESS then
    begin
      g_projname := savedialog.filename;
      caption := g_projname + ' - ' + g_layout.path;
      g_bUntitled := false;
    end;
  end;
end;

procedure TfrmMain.EditCopy1Execute(Sender: TObject);
begin
  CopyItemsToClipboard(Handle, g_Layout);
end;

procedure TfrmMain.EditPaste1Execute(Sender: TObject);
var
  h : pointer;
  i : integer;
  o : Ppasted_obj_t;
  blk : TBlk;
  pt : TPoint;
  ilk : TLink;
  lk : Pcblk_link_t;
begin
  h := pasteItemsFromClipboard(Handle, g_Layout.Block);
  i := 0;
  o := kproxy_get_pasted_object(h, i);
  g_layout.PrepareCanvas();
  g_layout.DeselectAll;
  while assigned(o) do
  begin
    case o^.magic of
    blk_magic:
    begin
      pt := getBlkPos(PBlk(o^.obj));
      inc(pt.X, g_layout.FiGridUnit);
      dec(pt.y, g_layout.FiGridUnit);
      // setBlkPos(Pblk(o^.obj), pt);
      blk := g_layout.AddBlock(Pblk(o^.obj));
      blk.Selected := true;
      blk.UiInvalidate;
      _mount_blk(PBlk(o^.obj));
    end;
    lnk_magic:
    begin
      lk := Pcblk_link_t(o^.obj);
      ilk := g_layout.AddLink(lk);
      ilk.UiInvalidate;
    end;
    end;
    inc(i);
    o := kproxy_get_pasted_object(h, i);
  end;
  kproxy_delete_paste(h);
  g_layout.releaseCanvas;
end;

procedure TfrmMain.EditCut1Execute(Sender: TObject);
begin
  BlkUnimplemented(handle);
end;

procedure TfrmMain.actStartBlockExecute(Sender: TObject);
begin
  KproxyShellWithError(Handle, 'start');
end;

procedure TfrmMain.actStopBlockExecute(Sender: TObject);
begin
  KproxyShellWithError(Handle, 'stop');
end;

procedure TfrmMain.actHelpExecute(Sender: TObject);
var
  s : array [0..1] of string;
  i : Integer;
begin
  s[0] := g_appPath + 'pmcf8.chm';
  s[1] := g_appPath + '../../../ctrller/docs/pmcf8.chm';
  for i:=0 to 1 do
    if FileExists(s[i]) then
    begin
      ShellExecute(
        Handle,
        'open',
        pchar(s[i]),
        nil,
        nil,
        SW_SHOWNORMAL
        );
      exit;
    end;
end;

procedure TfrmMain.actInvertPinExecute(Sender: TObject);
var
  p : TPin;
begin
  //
  if not assigned(g_layout.Hilighted()) then
    exit;
  if not (g_layout.Hilighted is tpin) then
    exit;
  p := g_layout.hilighted as tpin;
  if p.p._type<>pin_t_bool then
    exit;
  if KProxyShellWithError(Handle,pchar('invert ' + p.Fpinpath))<>F8_SUCCESS then
    exit;
  g_layout.PrepareCanvas;
  p.UiInvalidate;
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.actUpOneLevelExecute(Sender: TObject);
var
  blk:pblk;
begin
  blk := delphi_parent(g_layout.Block);
  if assigned(blk) then
    g_layout.Bind(blk);
end;

procedure TfrmMain.actOptionsExecute(Sender: TObject);
var
  frm : TfrmOptions;
begin
  frm := TfrmOptions.Create(self);
  frm.ShowModal;
  frm.Free;
end;

procedure TfrmMain.actLeftJustifyExecute(Sender: TObject);
var
  i : Integer;
  l : TList;
  tb : tblk;
  bb : tblk;
begin
  (sender as taction).Enabled := false;
  if g_layout.Readonly then
    exit;
  if g_layout.SelCount<=2 then
    exit;
  l := g_layout.GetAllBlocks;
  g_layout.PrepareCanvas();
  bb := tblk(l.Items[0]);
  for i:=1 to l.Count-1 do
  begin
    tb := tblk(l.Items[i]);
    tb.UiInvalidate;
    tb.Left := bb.Left;
    tb.UiOnResize;
    tb.UiInvalidate;
  end;
  l.Free;
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.actRightJustifyExecute(Sender: TObject);
var
  i : Integer;
  l : TList;
  tb : tblk;
  bb : tblk;
begin
  (sender as taction).Enabled := false;
  if g_layout.Readonly then
    exit;
  if g_layout.SelCount<=2 then
    exit;
  l := g_layout.GetAllBlocks;
  g_layout.PrepareCanvas();
  bb := tblk(l.Items[0]);
  for i:=1 to l.Count-1 do
  begin
    tb := tblk(l.Items[i]);
    tb.UiInvalidate;
    tb.Left := bb.Left + bb.Width - tb.Width;
    tb.UiOnResize;
    tb.UiInvalidate;
  end;
  l.Free;
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.actTopJustifyExecute(Sender: TObject);
var
  i : Integer;
  l : TList;
  tb : tblk;
  bb : tblk;
begin
  (sender as taction).Enabled := false;
  if g_layout.Readonly then
    exit;
  if g_layout.SelCount<=2 then
    exit;
  l := g_layout.GetAllBlocks;
  g_layout.PrepareCanvas();
  bb := tblk(l.Items[0]);
  for i:=1 to l.Count-1 do
  begin
    tb := tblk(l.Items[i]);
    tb.UiInvalidate;
    tb.top := bb.Top;
    tb.UiOnResize;
    tb.UiInvalidate;
  end;
  l.Free;
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.actBottomJustifyExecute(Sender: TObject);
var
  i : Integer;
  l : TList;
  tb : tblk;
  bb : tblk;
begin
  (sender as taction).Enabled := false;
  if g_layout.Readonly then
    exit;
  if g_layout.SelCount<=2 then
    exit;
  l := g_layout.GetAllBlocks;
  g_layout.PrepareCanvas();
  bb := tblk(l.Items[0]);
  for i:=1 to l.Count-1 do
  begin
    tb := tblk(l.Items[i]);
    tb.UiInvalidate;
    tb.top := bb.Top - tb.Height + tb.Height;
    tb.UiOnResize;
    tb.UiInvalidate;
  end;
  l.Free;
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.Panel2MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  pnlDir.BeginDrag(false);  
end;

procedure TfrmMain.ControlBar1DockOver(Sender: TObject;
  Source: TDragDockObject; X, Y: Integer; State: TDragState;
  var Accept: Boolean);
begin
  accept := false;
  if not (source.control is TToolBar) then
    exit;
  accept := true;
end;

procedure TfrmMain.Panel1DockOver(Sender: TObject; Source: TDragDockObject;
  X, Y: Integer; State: TDragState; var Accept: Boolean);
begin
  accept := false;
  if not (source.control = pnldir) then
    exit;
  accept := true;
end;

procedure TfrmMain.tvDblClick(Sender: TObject);
var
  cmd : string;
  blk : pblk;
  name : string;
  ublk : TBlk;
  nd : TTreeNode;
  a : PNodeAttribute;
begin
  // add the specified block
  if not assigned(tv.Selected) then
    exit;
  nd := tv.Selected;
  a := PNodeAttribute(nd.Data);
  if a.t=LibMagic then
  begin
    name := UniqueName(g_layout.block);
    cmd := 'mount path=' + g_layout.Path + ' name=' + name +
      ' file="' + pchar(a.pv[0]) + '"';
    if KProxyShellWithError(Handle,pchar(cmd))<>F8_SUCCESS then
      exit;
    blk := IBlk_blk_by_path(g_layout.block, pchar(name));
    assert(assigned(blk));
    SetBlkPos(blk,g_layout.InsertionPoint);
    g_layout.preparecanvas;
    ublk := g_layout.AddBlock(blk);
    ublk.UiInvalidate;
    g_layout.ReleaseCanvas;

    _mount_blk_at(blk, CurNode);
    // nd.MakeVisible;
    tv.OnChange := nil;
    // nd.Selected := true;
    tv.OnChange := tvChange;
  end else if a.t=TargetConfigMagic then
  begin
    actTargetConfig.Execute;
  end;
end;

procedure TfrmMain.tvStartDrag(Sender: TObject;
  var DragObject: TDragObject);
var
  o : tnewblock;
  a : PNodeAttribute;
begin
  if not assigned(tv.selected) then
    exit;
  a := tv.Selected.Data;
  if a.t <> LibMagic then
    exit;
  o := tnewblock.create;
  dragobject := o;
  o.Blktype := tv.Selected.Parent.Text + '.' +  tv.selected.Text;
  g_layout.PrepareCanvas;
end;

procedure TfrmMain.tvEndDrag(Sender, Target: TObject; X, Y: Integer);
begin
  if Sender is TNewBlock then
    g_layout.ReleaseCanvas;
end;

procedure TfrmMain.FormDockOver(Sender: TObject; Source: TDragDockObject;
  X, Y: Integer; State: TDragState; var Accept: Boolean);
begin
  accept := false;
  if not (source.Control=pnldir) then
    exit;
  accept := true;
end;

procedure TfrmMain.pnlDirEndDock(Sender, Target: TObject; X, Y: Integer);
begin
  if panel1.DockClientCount<>0 then
  begin
    pnldir.Constraints.MinWidth := 150;
    panel1.AutoSize := false;
  end else begin
    pnldir.Constraints.MinWidth := 3;
    panel1.AutoSize := true;
  end;
end;

procedure TfrmMain.pnlDirDblClick(Sender: TObject);
begin
  pnldir.ManualDock(panel1, panel1, alClient);
end;

procedure TfrmMain.actTargetConfigExecute(Sender: TObject);
var
  frm : TfrmTargetConfig;
begin
  frm := TFrmTargetconfig.Create(self);
  frm.ShowModal;
  frm.Free;
end;

procedure TfrmMain.Edit2KeyPress(Sender: TObject; var Key: Char);
begin
  if Ord(Key)<>VK_RETURN THEN
    EXIT;
  KProxyShellWithError(Handle,pchar(edit2.text));
end;

procedure TfrmMain.actExportExecute(Sender: TObject);
var
  name : string;
  tp : TPin;
  ep : TExtPin;
  ret : f8_status;
begin
  if not assigned(g_layout.Hilighted()) then
    Exit;
  if not (g_layout.Hilighted is TPin) then
    exit;
  //
  tp := g_layout.hilighted as TPin;
  if assigned(tp.p.ulink) then
    Exit;
  if not Inputquery(Application.Title, '输入导出引脚名', name) then
    exit;
  if not is_valid_pin_name(pchar(name)) then
  begin
    messagebox(handle, pchar(name + ' 不是一个合法的引脚名称'), '错误', mb_ok);
    exit;
  end;
  ret := KProxyShellWithError(
    Handle,
    pchar('exp ' + name + ' ' + GetPinPath(tp.p))
    );
  if ( ret = f8_success) then
  begin
    ep := Textpin.Create(g_layout);
    ep.Bind(tp);
    g_layout.AddItem(ep, ep.p);
    tp.ep := ep;
    tp.UiOnResize;
    ep.UiInvalidate;
  end;
end;

procedure TfrmMain.tvDeletion(Sender: TObject; Node: TTreeNode);
begin
  if assigned(node.Data) then
    Dispose(PNodeAttribute(Node.Data));
end;

procedure TfrmMain.ReloadUserBlocks;
begin
  g_UserBlocks.DeleteChildren;
  enumerate_blk_ent(_e_ent, 'user', __uint(g_UserBlocks));
end;


procedure TfrmMain.actStartBlockUpdate(Sender: TObject);
begin
  // actStartBlock.Enabled := g_layout.Online and g_layout
end;

procedure TfrmMain.UpdateStatus;
begin
  if isOnline then
    self.stb.Panels[2].Text := '联机'
  else
    self.stb.Panels[2].Text := '脱机';
  if g_layout.Readonly then
    self.stb.Panels[3].Text := '只读'
  else
    self.stb.Panels[3].Text := '读写';
end;

procedure TfrmMain.actViewBrowserExecute(Sender: TObject);
begin
  panel1.Visible := not panel1.Visible;
  actViewBrowser.Checked := panel1.Visible;
end;

procedure TfrmMain.actExportUpdate(Sender: TObject);
var
  tp : TUiItem;
begin
  (Sender as TAction).Enabled := false;
  tp := g_layout.Hilighted;
  if not assigned(tp) then
    Exit;
  if not (tp is TPin) then
    Exit;
  if not IBLk_exportable((tp as TPin).p) then
    exit;
  (Sender as TAction).Enabled := true;
end;

procedure TfrmMain.actInvertPinUpdate(Sender: TObject);
var
  tp : TUiItem;
  blk : Pblk;
begin
  (Sender as TAction).Enabled := false;
  tp := g_layout.Hilighted;
  if not assigned(tp) then
    Exit;
  if not (tp is TPin) then
    Exit;
  blk := (tp as TPin).p.blk;
  blk := delphi_parent(blk);
  if assigned(blk) then
    if (blk.h.flags and BLK_F_READONLY)<>0 then
    begin
      (Sender as TAction).Enabled := false;
      exit;
    end;
  if (tp as TPin).rp._type<>pin_t_bool then
    exit;
  (Sender as TAction).Enabled := true;
end;

procedure TfrmMain.actLinkUpdate(Sender: TObject);
var
  it1,it2:Tuiitem;
  p1,p2 : TPin;
begin
  (sender as Taction).enabled := false;
  if g_layout.SelPinCount<>2 then
    exit;
  it1 := g_layout.getfirstselection;
  it2 := g_layout.GetNextSelection(it1);
  p1 := it1 as tpin;
  p2 := it2 as tpin;
  if not IBlk_linkable(p1.p, p2.p) then
    exit;
  (sender as taction).Enabled := true;
end;

procedure TfrmMain.actLinkExecute(Sender: TObject);
var
  it1,it2:Tuiitem;
  p1,p2 : TPin;
begin
  if g_layout.SelCount<>2 then
    exit;
  it1 := g_layout.getfirstselection;
  it2 := g_layout.GetNextSelection(it1);
  if not ((it1 is tpin) and (it2 is tpin)) then
    exit;
  p1 := it1 as tpin;
  p2 := it2 as tpin;
  if not IBlk_linkable(p1.p, p2.p) then
    exit;
  g_layout.PrepareCanvas();
  OnAddLink(g_layout, evtAddLink, p1, p2);
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.actInsertBlockUpdate(Sender: TObject);
begin
  (sender as taction).Enabled := false;
  if g_layout.Readonly then
    exit;
  (sender as taction).Enabled := true;
end;

procedure TfrmMain.actLeftJustifyUpdate(Sender: TObject);
var
  ee : boolean;
begin
  ee := false;
  if not g_layout.Readonly and  (g_layout.SelBlkCount>=2) then
    ee := true;
  (sender as taction).enabled := ee;
end;

procedure TfrmMain.Splitter1OnClick(Sender: TObject);
begin
  actViewBrowser.Execute;
end;

{ TSplitter }

constructor TSplitter.Create(AOwner: TComponent);
begin
  inherited;
end;

procedure TSplitter.SetOnClick(proc: TNotifyEvent);
begin
  OnClick := proc;
end;

procedure TfrmMain.ShowBlkProperty(blk: PBlk);
var
  nd : TTreeNode;
  frm2 : Tfrmblkattrib;
begin
  frm2 := tfrmblkattrib.create(self);
  frm2.blk := blk;
  nd := _node_by_path(tv.Items.GetFirstNode, pchar(GetBlkPath(blk)));
  if frm2.showmodal=mrOk then
  begin
    if assigned(g_layout.Hilighted()) then
    begin
      g_layout.PrepareCanvas;
      g_layout.Hilighted.UiOnScale;
      g_layout.Hilighted.UiInvalidate;
      g_layout.ReleaseCanvas;
    end;
    if assigned(nd) then
      nd.Text := Iblk_name(blk);
  end;
  frm2.free;
end;

procedure TfrmMain.ShowPinProperty(p: Pblk_pin_t);
var
  frm : tfrmpinattrib;
begin
  frm := tfrmpinattrib.create(self);
  frm.p := g_layout.ItemByPointer(p) as TPin;
  frm.showmodal;
  frm.free;
end;

procedure TfrmMain.EditPaste1Update(Sender: TObject);
var
  ee : boolean;
begin
  ee := false;
  if CanPaste(g_layout) then
    ee := true;
  (sender as taction).enabled := ee;
end;

procedure TfrmMain.EditCopy1Update(Sender: TObject);
var
  ee : boolean;
begin
  ee := false;
  if g_layout.SelCount<>0 then
    ee := true;
  (sender as taction).enabled := ee;
end;

procedure TfrmMain.actOnlineUpdate(Sender: TObject);
var
  ee : boolean;
begin
  ee := false;
  if not isOnline then
    ee := true;
  (sender as taction).enabled := ee;
end;

procedure TfrmMain.actOfflineUpdate(Sender: TObject);
var
  ee : boolean;
begin
  ee := false;
  if isOnline then
    ee := true;
  (sender as taction).enabled := ee;
end;

procedure TfrmMain.actSearchExecute(Sender: TObject);
var
  name : string;
  blk : Pblk;
  it : TBlk;
  pt : tPoint;
  i : integer;
begin
  //
  name := edtSearch.Text;
  if name='' then
  begin
  if not inputquery(Application.Title, '输入要查找的功能块名', name) then
    exit;
  end;
  blk := IBlk_subblk_by_name(g_layout.Block, pchar(name));
  if not assigned(blk) then
  begin
    MessageBox(handle, '对象没有找到', pchar(application.title), mb_ok);
    exit;
  end;
  it := TBlk(g_layout.ItemByPointer(blk));
  assert(assigned(it));
  g_layout.PrepareCanvas;
  pt.x := it.Left;
  pt.Y := it.Top;
  g_layout.makeVisible(pt);
  for i:=0 to 5 do
  begin
    it.Selected := not it.Selected;
    g_layout.Refresh;
    sleep(150);
  end;
  g_layout.ReleaseCanvas;
end;

end.
