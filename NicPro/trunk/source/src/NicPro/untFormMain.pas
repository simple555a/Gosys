unit untFormMain;

interface

uses Windows, shellapi, Messages, Classes, Graphics, Forms, Controls, Menus,
  Dialogs, StdCtrls, Buttons, ExtCtrls, ComCtrls, ImgList,
  StdActns,
  sysutils, 
  ActnList,
  ToolWin,
  Grids,
  ValEdit,
  ActnMan,
  f8,
  libcobj,
  untFormBlkBrowser,
  common,
  librtk,
  //rtknet,
  rtkdefs,
  f8ui,
  untFormSelTarget,
  untFormblkattrib,
  untformpinattrib,
  untFormTargetConfig,
  untFormOptions,
  untFormVariables,
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
    actZoomIn: TAction;
    actZoomOut: TAction;
    stb: TStatusBar;
    pm: TPopupMenu;
    Selectall1: TMenuItem;
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
    actHelp: TAction;
    actInvertPin: TAction;
    N7: TMenuItem;
    I1: TMenuItem;
    actUpOneLevel: TAction;
    actOptions: TAction;
    N13: TMenuItem;
    actLeftJustify: TAction;
    actRightJustify: TAction;
    actTopJustify: TAction;
    actBottomJustify: TAction;
    Panel1: TPanel;
    pnlDir: TPanel;
    tv: TTreeView;
    Splitter1: TSplitter;
    actTargetConfig: TAction;
    actExport: TAction;
    actExport1: TMenuItem;
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
    ToolBar5: TToolBar;
    ToolBar3: TToolBar;
    ToolButton12: TToolButton;
    ToolButton20: TToolButton;
    ToolButton29: TToolButton;
    ToolButton30: TToolButton;
    ToolButton31: TToolButton;
    ToolButton32: TToolButton;
    ToolButton42: TToolButton;
    ToolBar2: TToolBar;
    ToolButton23: TToolButton;
    ToolButton24: TToolButton;
    ToolButton25: TToolButton;
    ToolButton26: TToolButton;
    ToolButton7: TToolButton;
    ToolButton27: TToolButton;
    ToolButton28: TToolButton;
    ToolButton38: TToolButton;
    ToolButton41: TToolButton;
    ToolButton40: TToolButton;
    ToolButton39: TToolButton;
    ToolBar1: TToolBar;
    ToolButton1: TToolButton;
    ToolBar4: TToolBar;
    ToolButton34: TToolButton;
    ToolButton35: TToolButton;
    actStartSimulator: TAction;
    N22: TMenuItem;
    N1: TMenuItem;
    N23: TMenuItem;
    actInsertExternalBlock: TAction;
    actInsertExternalBlock1: TMenuItem;
    Panel2: TPanel;
    actViewOutput: TAction;
    N6: TMenuItem;
    Panel3: TPanel;
    Memo1: TMemo;
    ComboBox1: TComboBox;
    Splitter2: TSplitter;
    actDelete: TAction;
    actPrint: TAction;
    actFilePrintSetup: TFilePrintSetup;
    actFilePageSetup: TFilePageSetup;
    N11: TMenuItem;
    PageSetup1: TMenuItem;
    actEnableConfig: TAction;
    ToolButton2: TToolButton;
    N12: TMenuItem;
    N14: TMenuItem;
    actViewCpu: TAction;
    actViewStation: TAction;
    N24: TMenuItem;
    CPU1: TMenuItem;
    IO1: TMenuItem;
    actDisassemblyBlock: TAction;
    N25: TMenuItem;
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
    procedure edtSearchKeyPress(Sender: TObject; var Key: Char);
    procedure actViewDatabaseExecute(Sender: TObject);
    procedure actDownloadUpdate(Sender: TObject);
    procedure actStartSimulatorExecute(Sender: TObject);
    procedure actInsertExternalBlockExecute(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure ComboBox1KeyPress(Sender: TObject; var Key: Char);
    procedure actViewOutputExecute(Sender: TObject);
    procedure PrintSetup1Click(Sender: TObject);
    procedure actEnableConfigUpdate(Sender: TObject);
    procedure actEnableConfigExecute(Sender: TObject);
    procedure actPrintExecute(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure stbDrawPanel(StatusBar: TStatusBar; Panel: TStatusPanel;
      const Rect: TRect);
    procedure actViewCpuExecute(Sender: TObject);
    procedure actViewStationExecute(Sender: TObject);
    procedure actDisassemblyBlockUpdate(Sender: TObject);
    procedure actDisassemblyBlockExecute(Sender: TObject);
  private
    (* Private declarations en*)
    (* these two coordinates are of the last mouse event *)
    m_CurBlk : PBlk;

    procedure oneditblock(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
    procedure oneditpin(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
    procedure onAddLink(Sender : TBlkLayout; action:TBlkNotification;obj,obj1 : pointer);
    procedure onaddblock(sender : tblklayout; action:tblknotification; obj, obj1 :pointer);
    procedure onBind(sender : tblklayout; action:tblknotification; obj, obj1 :pointer);
    procedure onLayoutHint(s : string);

    procedure LoadBlkLibrary(nd : TTreeNode);
    procedure ReloadUserBlocks(path : string='');
    procedure ShowProperty(ob : TUiItem);
    procedure ShowBlkProperty(blk : PBlk);
    procedure ShowPinProperty(p : Pblk_pin_t);
    function  CurNode:TTreeNode;
    procedure Splitter1OnClick(Sender : TObject);

    function promptSave:boolean;

    procedure OnOutputString(var m: TMessage); message WM_USER;
    procedure ShowException(Sender: TObject; E: Exception);
  public
    procedure ReloadKernel;
    procedure RefreshBlock(blk : PBlk);
    function  CurBlk:PBlk;
    procedure ShowBlockLayout(blk : PBlk);
    procedure UpdateStatus;
    procedure OutputString(s : string);
  (* Public declarations *)
  end;

var
  frmMain: TfrmMain;

implementation

uses about,
  libutils,
  activex,
  untCopyPaste,
  winsock,
  untFormSearch,
  printers,
  untProgress,
  comobj;
type
  PString = ^string;

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
  g_bCmdLineParsed : boolean;
{$R *.dfm}

{$define ENABLE_USER_BLOCKS}

procedure handleAdapterEvent(a : PKAdapter; event, p0, p1 : f8_int);cdecl;
begin
  case event of
  AE_STATUS_CHANGED:
  begin
    g_layout.FbOnline := isOnline;
    g_layout.uiInvalidate;
  end;
  AE_IN_PROGRESS:
  begin
    if g_bCmdInProgress then begin
      g_progress := p0;
      frmMain.stb.Repaint;
    end;
  end;
  AE_CONNECTION_LOST:
  BEGIN
    reConnect(1, '');
  END;
  end;
end;

function _my_puts(level : integer; s : pchar):integer;stdcall;
begin
  result := oldPuts(level, s);
end;

function _my_puts_1(level : integer; s : pchar):integer;stdcall;
var
  ss : Pstring;
begin
  if level>0 then
    if assigned(frmSplash) then begin
      if (GetCurrentThreadId=g_MainThread) then begin
        frmSplash.Label1.Caption := s;
        frmSplash.Update;
        //Sleep(1);
      end;
    end else begin
      if (GetCurrentThreadId=g_MainThread) then
        frmMain.outputString(s)
      else begin
        new(ss);
        ss^ := s;
        postMessage(frmMain.Handle,wm_user,Integer(ss),0);
      end;
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
  if blk.h.magic<>CBLK_MAGIC then
    exit;
  mend := nd.owner.addchild(nd, IBlk_name(blk));
  AssociateNodeWith(mend, ProgramMagic, blk);
  if blk.h.magic=CBLK_MAGIC then
  begin
    mend.ImageIndex := 34;
    mend.HasChildren := false;
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
  if not promptSave then
    exit;
  KProxyShellWithError(Handle,'create');
  // KProxyShellWithError(Handle,'mount path=/ name=section_1 type=base.blank rw');
  g_bUntitled := true;
  g_bModified := false;
  g_projname := 'Untitled';
  caption := g_projname + ' - ' + g_layout.path;
  ReloadKernel;
  ReloaduserBlocks;
end;

procedure TfrmMain.FileSave1Execute(Sender: TObject);
begin
  if g_bUntitled then
    FileSaveAsItemClick(sender)
  else begin
    KProxyShellWithError(Handle,'save');
    g_bModified := false;
  end
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
  pt : tPoint;
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
      pt.x := ublk.left;
      pt.Y := ublk.top;
      g_layout.makeVisible(pt);
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
  wsa : WSAData;
begin
  if not GetDebug then
  begin
    frmSplash := TAboutBox.Create(nil);
    frmSplash.Visible := true;
    frmSplash.Update;
  end;

  Application.OnException := ShowException;
  g_MainThread := GetCurrentThreadId();
  oldPuts := TUtilsPutsProc(pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^);
  // pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^ := dword(@_my_puts);
  // if not GetDebug then
    pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^ := dword(@_my_puts_1);

  WSAStartup($202, wsa);
  initKproxy;
  g_adapter^.eventSink := handleAdapterEvent;

  g_projname := 'Untitled';
  g_bUntitled := true;

  panel2.Align := alClient;
  panel3.BevelOuter := bvNone;

  g_Layout := tblklayout.Create(self);
  g_Layout.FKernel := g_kernel;
  g_Layout.Parent := panel2;
  g_Layout.Scale := 1.0;
  g_layout.Align := alClient;
  g_layout.OnEditBlock := oneditblock;
  g_layout.oneditpin := oneditpin;
  g_layout.OnAddLink := onAddLink;
  g_layout.OnAddBlock := onaddblock;
  g_layout.OnBind := onbind;
  g_layout.onHint := onLayoutHint;
  g_layout.SendToBack;
  // g_layout.m_bshowgrid := false;
  g_layout.PopupMenu := pm;

  splitter1.SetOnClick(Splitter1OnClick);
  splitter2.SetOnClick(actViewOutputExecute);

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
    //Sleep(1500);
    frmSplash.Visible := false;
    frmSplash.Free;
    //Sleep(800);
    frmSplash := nil;
  end;

  stb.Panels.Items[0].Style := psOwnerDraw;
end;

procedure TfrmMain.actInsertBlockExecute(Sender: TObject);
var
  nb : tnewblock;
begin
  if g_layout.Readonly then
    exit;
  g_layout.PrepareCanvas();
  nb := tnewblock.Create;
  nb.BlkFile := string(get_exe_dir) + 'blank.blk';
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
        if assigned(nd) then nd.Delete;
      end;
      g_Layout.DropItem(it);
    end;
  end;
  g_layout.UiInvalidate;
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
  // uninit_network;
  WSACleanup;
  pdword(pdword(pdword(dword(@utils_puts) + 2)^)^)^ := dword(@oldPuts);
end;

procedure TfrmMain.FileOpen1Execute(Sender: TObject);
var
  cmd : string;
  path : array[0..MAX_PATH] of char;
begin
  //
  if not promptSave then
    exit;
  GetCurrentDirectory(sizeof(path), @path[0]);
  openDialog.InitialDir := path;
  openDialog.Filter := 'Executable(*.ce)|*.ce|Text files(*.txt)|*.txt|All files(*.*)|*.*';
  if OpenDialog.Execute then
  begin
    if openDialog.FilterIndex=2 then begin
      KProxyShellWithError(Handle,'close');
      // for play commands, the user blocks must be opened before hand
      ReloaduserBlocks(extractFilePath(OpenDialog.FileName));
      cmd := 'play '+OpenDialog.FileName
    end else
      cmd := 'open ' + OpenDialog.FileName;
    if KProxyShellWithError(Handle,pchar(cmd))=F8_SUCCESS then
    begin
      g_projname := opendialog.filename;
      g_bUntitled := false;
      g_bModified := false;
    end;
    ReloadKernel;
    if openDialog.FilterIndex=2 then
      probe_blk_resources(g_sections)
    else
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
    if reConnect(2, frm.cmbTarget.Text) = F8_SUCCESS then
    begin
      tmr.Enabled := true;
    end;
  end;
  frm.Free;
end;

procedure TfrmMain.actDownloadExecute(Sender: TObject);
var
  s : string;
begin
  s := '下载程序将导致控制程序停止执行，下载完毕后将继续执行。是否继续?';
  if MessageBox(handle,pchar(s),'警告',MB_YESNOCANCEL)<>IDYES then
    exit;
  if KProxyShellWithError(Handle,'dload')=0 then
    messagebox(handle,'下载成功.','提示',MB_OK);
end;

procedure TfrmMain.actUploadExecute(Sender: TObject);
begin
  KProxyShellWithError(Handle,'uload');
end;

procedure TfrmMain.actCheckVerExecute(Sender: TObject);
var
  sRemote : kstatus_t;
  s : string;
begin
  kproxy_shell('ver');
  {
  ZeroMemory(@sRemote, sizeof(sRemote));
  s := '本地版本: ' + f8UuidToString(g_sections.h.uuid);
  if isOnline and (getRemoteStatus(sRemote)=f8_success) then
  begin
    s := s + #13 + '远程版本: ' + f8UuidToString(sRemote.prog_id);
  end else begin
    s := s + #13 + '远程版本: N.A.';
  end;

  MessageBox(handle, pchar(s), '版本信息', mb_ok);
  }
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
  Result := __true;
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
  Result := __true;
end;

procedure TfrmMain.LoadBlkLibrary(nd : TTreeNode);
begin
  tv.Items.BeginUpdate;
{$IFDEF ENABLE_USER_BLOCKS}
  g_UserBlocks := nd.Owner.AddChild(nd, 'user');
  g_UserBlocks.ImageIndex := 58;
  g_UserBlocks.SelectedIndex := 59;
  AssociateNodeWith(g_userBlocks, InactiveMagic, nil);
{$ENDIF}
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
  if not is_valid_blk_name(pchar(name)) then begin
    messagebox(handle, pchar(name + ' 不是一个合法的功能块名称'), '错误', mb_ok);
    exit;
  end;
  KProxyShellWithError(
    Handle,
    pchar('lib / ' + name)
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
  pp : Integer;
begin
  o := tnewblock(obj);
  if o.Blktype<>'' then begin
    pp := pos('.',o.blkType);
    name := UniqueName(g_layout.block,copy(o.blkType,pp+1,length(o.blkType)))
  end else
    name := UniqueName(g_layout.block,'');
  if o.Blktype<>'' then
    cmd := 'mount type="' + o.blktype + '" path=' + g_layout.path + ' name=' +
      name + ' rw'
  else
    cmd := 'mount file="' + o.blkFile + '" path=' + g_layout.path + ' name=' +
      name + ' rw';
  if KProxyShellWithError(Handle,pchar(cmd))<>F8_SUCCESS then
    exit;
  blk := iblk_blk_by_path(g_layout.block, pchar(name));
  assert(assigned(blk));
  setblkpos(blk, o.pt);
  ublk := g_layout.addblock(blk);
  ublk.uiinvalidate;
  _mount_blk_at(blk, _node_by_path(tv.Items.GetFirstNode, pchar(g_layout.path)));
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
    case saveDialog.FilterIndex of
    1,3:
    begin
      cmd := 'save ' + savedialog.FileName;
      if KProxyShellWithError(Handle,pchar(cmd))=F8_SUCCESS then
      begin
        g_projname := savedialog.filename;
        caption := g_projname + ' - ' + g_layout.path;
        g_bUntitled := false;
        g_bModified := false;
      end;
    end;
    2:
    begin
      cmd := 'export -of:' + savedialog.FileName + ' -source -config';
      KProxyShellWithError(Handle,pchar(cmd))
    end;
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
  s[1] := g_appPath + 'help/pmcf8.chm';
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
  if a.t=LibMagic then begin
    name := UniqueName(g_layout.block,nd.Text);
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
  end else if a.t=TargetConfigMagic then begin
    actTargetConfig.Execute;
  end else if a.t=ProgramMagic then
    ShowBlkProperty(Pblk(a.p));
end;

{
  refer to Win32's drag-n-drop documentation
  and delphi's wrapping.

  StartDrag will generate a dragObject to
  hold information needed for this drag session. 
}
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
  if not is_valid_pin_name(pchar(name)) then begin
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

procedure TfrmMain.ReloadUserBlocks(path : string);
begin
{$IFDEF ENABLE_USER_BLOCKS}
  g_UserBlocks.DeleteChildren;
  if path<>'' then
    set_blklib_project_path(pchar(path));
  enumerate_blk_ent(_e_ent, 'user', __uint(g_UserBlocks));
{$ENDIF}
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
  if not actEnableConfig.Checked then
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
  if not (IBLk_exportable((tp as TPin).p)<>0) then
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
  if not (IBlk_linkable(p1.p, p2.p)<>0) then
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
  g_layout.PrepareCanvas();
  OnAddLink(g_layout, evtAddLink, p1, p2);
  g_layout.ReleaseCanvas;
end;

procedure TfrmMain.actInsertBlockUpdate(Sender: TObject);
begin
{$IFDEF NO_COMPOSITE_BLOCK}
  (sender as taction).Enabled := false;
  if g_layout.Readonly then
    exit;
  {blank block can only be inserted at top level}
  if assigned(delphi_parent(g_layout.Block)) then
    exit;
{$ENDIF}
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
  it : TUIItem;
  pit : TPin;
  v : Pblk_var_t;
begin
  // frmSearch.Edit1.Text := comboBox2.Text;
  it := g_Layout.GetFirstSelection;
  if it is TPin then begin
    pit := TPin(it);
    v := query_var_by_pin(pit.rp);
    if assigned(v) then
      frmSearch.Edit1.Text := v.name;
  end;
  frmSearch.show;
end;

{
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
    sleep(100);
  end;
  g_layout.ReleaseCanvas;
end;
}
procedure TfrmMain.edtSearchKeyPress(Sender: TObject; var Key: Char);
begin
  if ord(key)=vk_return then
    actSearch.Execute;
end;

procedure TfrmMain.onLayoutHint(s: string);
begin
  self.stb.Panels[4].Text := s;
end;

procedure TfrmMain.actViewDatabaseExecute(Sender: TObject);
begin
  if not assigned(frmVariables) then
    Application.CreateForm(TfrmVariables, frmVariables);
  frmVariables.ShowModal;
end;

procedure TfrmMain.actDownloadUpdate(Sender: TObject);
begin
  (sender as TAction).Enabled := isOnline;
end;

procedure TfrmMain.actStartSimulatorExecute(Sender: TObject);
var
  pi : PROCESS_INFORMATION;
  si : STARTUPINFO;
  s : string;
begin
  s := g_AppPath + 'fsimnt.exe';
  zeroMemory(@si, sizeof(si));
  si.cb := sizeof(si);
  ZeroMemory(@pi, sizeof(pi));
  if createProcess(nil, pchar(s), nil, nil, FALSE, 0, NIL, NIL, si, pi) then
  begin
    closeHandle(pi.hProcess);
    closeHandle(pi.hThread);
  end;
end;

procedure TfrmMain.actInsertExternalBlockExecute(Sender: TObject);
var
  name : string;
  cmd : string;
  path : array[0..MAX_PATH] of char;
  blk : PBlk;
begin
  //
  GetCurrentDirectory(sizeof(path), @path[0]);
  openDialog.InitialDir := path;
  openDialog.Filter := 'Block files(*.blk)|*.blk|All files(*.*)|*.*';
  if OpenDialog.Execute then
  begin
    name := uniqueName(g_layout.Block,'');
    cmd := 'mount file="' + OpenDialog.FileName + '" path="' + g_layout.Path + '" name=' + name;
    if KProxyShellWithError(Handle,pchar(cmd))=F8_SUCCESS then
    begin
      g_layout.Bind(g_layout.block);
      blk := IBlk_blk_by_path(g_layout.block, pchar(name));
      _mount_blk_at(blk, CurNode);
    end;
  end;
end;

function TfrmMain.promptSave:boolean;
var
  ret : integer;
begin
  //
  result := true;
  if not g_bModified then
    exit;

  result := false;
  ret := MessageBox(handle, '文件没有保存，是否现在保存？', pchar(application.title), mb_yesnocancel);
  if ret=idyes then
  begin
    FileSave1Execute(SELF);
    result := not g_bModified;
  end else if ret=idno then begin
    result := true;
  end;
end;

procedure TfrmMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  if not promptSave then
    action := caNone;
end;

procedure TfrmMain.ComboBox1KeyPress(Sender: TObject; var Key: Char);
var
  i : integer;
  s : string;
  r : f8_status;
begin
  if Ord(Key)<>VK_RETURN THEN
    EXIT;
  memo1.Enabled := false;
  memo1.Text:='';
  r:=KProxyShellWithError(Handle,combobox1.text,0);
  memo1.Enabled := true;
  if r<>0 then
    exit;
  s := combobox1.Text;
  for i:=0 to combobox1.Items.Count-1 do
  begin
    if s=combobox1.items.Strings[i] then
      exit
  end;
  combobox1.Items.add(s);
end;

procedure TfrmMain.actViewOutputExecute(Sender: TObject);
begin
  // panel3.Visible := not panel3.Visible;
  // actViewoutput.Checked := panel3.Visible;
  if panel3.Height<3 then
    panel3.Height := panel3.Tag
  else begin
    panel3.Tag := panel3.Height;
    panel3.Height := 2;
  end;
  actViewoutput.Checked := panel3.Height>3;
end;

procedure TfrmMain.OnOutputString(var m: TMessage);
VAR
  s : Pstring;
begin
  s := Pstring(m.WParam);
  outputString(s^);
  dispose(s);
end;

procedure TfrmMain.OutputString(s: string);
var
  p,p2,ll : integer;
begin
  p := pos(chr($a),s);
  p2 := 1;
  ll := length(s);
  if memo1.lines.Count>128 then
    memo1.Text := '';
  while p>0 do begin
    memo1.Text := memo1.text + copy(s,p2,p-1);
    memo1.Lines.Add('');
    p2 := p+1;
    s := copy(s,p+1,ll);
    p := pos(chr($a),s);
  end;
  memo1.text := memo1.text + copy(s,p2,length(s));
end;

procedure TfrmMain.PrintSetup1Click(Sender: TObject);
begin
  //
end;

procedure TfrmMain.actEnableConfigUpdate(Sender: TObject);
begin
  //
  actEnableConfig.Enabled := isOnline;
  if not isOnline then
    actEnableConfig.Checked := false;
end;

procedure TfrmMain.actEnableConfigExecute(Sender: TObject);
var
  cmd : string;
begin
  //
  cmd := 'lock';
  assert(isOnline);
  actEnableConfig.Checked := not actEnableConfig.Checked;
  if actEnableConfig.Checked then
    cmd := cmd + ' true'
  else
    cmd := cmd + ' false';
  KProxyShellWithError(Handle,cmd);
end;

procedure TfrmMain.actPrintExecute(Sender: TObject);
begin
  g_layout.print;
end;

procedure TfrmMain.FormShow(Sender: TObject);
var
	i : integer;
  cmd : string;
begin
  if g_bCmdLineParsed then
    exit;
  g_bCmdLineParsed := true;
	for i:=1 to paramcount do
  	if file_exists(pchar(paramstr(i))) or file_exists(pchar(paramstr(i)+'.ce')) then begin
      cmd := 'open ' + paramstr(i);
      if KProxyShellWithError(Handle,pchar(cmd))=F8_SUCCESS then begin
        g_projname := paramstr(i);
        g_bUntitled := false;
        g_bModified := false;
      end;
      ReloadKernel;
      ReloaduserBlocks;
      exit;
    end;
end;

procedure TfrmMain.stbDrawPanel(StatusBar: TStatusBar; Panel: TStatusPanel;
  const Rect: TRect);
var
  r : TRect;
begin
  //
  if not g_bCmdInProgress then
    exit;
  r := rect;
  r.Right := r.left + (r.Right-r.left)*g_progress div 100;
  with statusBar.Canvas do begin
    brush.Color := clBlue;
    FillRect(r);
  end;
end;

procedure TfrmMain.ShowException(Sender: TObject; E: Exception);
begin
  delphi_puts(msg_error,'Exception:');
  delphi_puts(msg_error,pchar(e.Message));
  delphi_puts(msg_error,chr(10)+chr(13));
end;

procedure TfrmMain.actViewCpuExecute(Sender: TObject);
begin
  kproxy_shell('cpu');
end;

procedure TfrmMain.actViewStationExecute(Sender: TObject);
begin
  kproxy_shell('station');
end;

procedure TfrmMain.actDisassemblyBlockUpdate(Sender: TObject);
var
  it:Tuiitem;
  blk : Pblk;
begin
  (sender as TAction).Enabled := false;
  it := g_layout.getfirstselection;
  if not (it is TBlk) then
    exit;
  (sender as taction).Enabled := true;
end;

procedure TfrmMain.actDisassemblyBlockExecute(Sender: TObject);
var
  it:Tuiitem;
  blk : Pblk;
begin
  it := g_layout.getfirstselection;
  if not (it is TBlk) then
    exit;
  kproxy_shell(pchar('u ' + (it as Tblk).path));
end;

end.
