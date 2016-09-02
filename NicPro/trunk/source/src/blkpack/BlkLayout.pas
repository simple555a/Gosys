(*
2003/4/2 Jackie

TBlkLayout
there should be an object to give user visual feedback of
the layout of a composite function block, which is composed
by subblocks(which in turn can be either basic block or
composite block), links, pin-definitions.

TSubblk
there should be an object to give user control over subblocks
of composite block, e.g., user can move/rename/delete a subblock.

TPin
a sub object of TSubblk, user can select two TPins to make a
connection

TLinkDef
a UI object for struct cblk_link_t.

TSubblk,TPin,TLinkDef are descendants of one or more
of the following classes:
1) TVisible
is an object that can be seen, hittest, and deleted
2) TDragable
is an object that can be draged, either alone or along with other
TDragable
3) TSelectable
is an object that can give a hilighted visual feedback when
clicked to indicate a selected state.

Tasks
1) insert block
2) select a block by click
3) select multiple blocks by click while holding CTRL
4) select blocks/links/external pins by click while holding CTRL
5) select multiple blocks/links/external pins by rubber rect
6) make connection by selecting two pins in a row, or alternatively
by make a series of clicking from one pin until the clicking on
another pin
7) drag selected object(s)
8) delete selected object(s)
9) dragging on blocks seems straight forward, because they are
like regid body, while links are not so, they can change their shapes
while dragged, if a link alone is dragged, we must distinguish
two actions : drag-and-move, drag-and-reshape

layout.dragger handles drag-dropping initiated from inside the layout.
while vw's DragDrop/DragOver routine handles drag-drop initiated from
outside the layout. 
*)
unit BlkLayout;

interface

uses
  f8,
  SysUtils,
  Classes,
  Controls,
  ExtCtrls,
  types,
  rtkdefs,
  stdctrls,
  libcobj,
  windows,
  graphics,
  math,
  libutils,
  common,
  Forms;

const
  DEFAULT_CANVAS_WIDTH  =	16837*4;	// twips
  DEFAULT_CANVAS_HEIGHT =	11905*4;	// twips

type
  PUiItem = ^TUiItem;
  TUiItem = class;
  TBlkLayout = class;
  TBlk = class;
  TPin = class;
  TLink = class;
  TExtPin = class;

  // TDrag
  TDragTask = (dgMove, dgRect, dgLink, dgReshapeLink, dgReshapeLogic);

  TCanvasState = record
    PenColor : TColor;
    PenWidth : Integer;
    PenStyle : TPenStyle;
    PenMode  : TPenMode;
    BrushColor : TColor;
  end;

  TUiItem = class
  public
    Top : Integer;
    Left : Integer;
    Width : Integer;
    Height : Integer;
    Color : TColor;
    Layout : TBlkLayout;
    
  protected
    FDragLastPos : TPoint;
    FbStartDrag : boolean;
    FSelected : Boolean;
    FAssociated : tlist;
    FAssociatedMaster : tlist;
    Fpathname : string;

    function GetCanvas:TCanvas;
    procedure SetSelected(s : boolean);
    function GetCanvasHandle : hwnd;

  public
    Selectable : boolean;
    Copiable : boolean;
    Moveable : boolean;
    Sizable : boolean;

    DragCursor : TCursor;
    link : rtkdefs.LIST_ENTRY;
    selLink : rtkdefs.LIST_ENTRY;

    constructor Create(lo : TBlkLayout);virtual;
    destructor Destroy;override;
    procedure render;virtual;abstract;
    procedure renderText;virtual;abstract;
    procedure renderRect(x, y:Integer);virtual;
    function SaveToStream(ms : PStream):boolean;virtual;
    function getPointer:Pointer;virtual;abstract;

    procedure uiOnStartDrag(x, y:Integer);virtual;
    procedure uiOnDragging(x, y:Integer);virtual;
    procedure uiOnEndDrag(Cancelled: Boolean);virtual;
    procedure uiOnResize;virtual;
    procedure UiInvalidate;virtual;
    procedure UiInvalidateRect(r : TRect);
    procedure UiInvalidateText(p: tpoint; s: string;
      rightAligned: boolean = false);
    function UiTextRect(p : tpoint; s : string;
      rightAligned : boolean = false):TRect;

    function HitTest(x, y:Integer):TUiItem;virtual;
    function BoundingRect:TRect;virtual;
    procedure uiOnDrop;virtual;

    function IntersectRect(r : TRect):boolean;overload;virtual;

    procedure DebugDump;virtual;

    property Canvas : TCanvas read GetCanvas;
    property CanvasHandle : HWND read GetCanvasHandle;
    property Selected : boolean read FSelected write SetSelected;

    procedure UiInvalidateChanged;virtual;
    procedure ShowPropertyPage(p : TForm);virtual;
    procedure Associate(ui : TUiItem);
    procedure Deassociate(ui : TUiItem);
    function  HintMessage : string; virtual;
  end;

  TBlkNotification = (
    evtEditBlock,
    evtEditPin,
    evtEditLink,
    evtAddLink,
    evtAddBlock,
    evtOnBind
    );

  TBlkNotifyEvent = procedure (
    Sender : TBlkLayout;
    action : TBlkNotification;
    obj, obj1 : pointer
    )
    of object;

  TOnHintEvent = procedure(h : string) of object;
  
  TNewBlock = class(TDragObjectEx)
  public
    Blktype : string;
    BlkFile : string;
    Pt : TPoint;
  end;

  TBlkLayout = class(TScrollBox)
  private
    { Private declarations }
    vw : TPaintBox;
    pnlBiu : TPanel;
    dragger : TButton;

    Fuuid : f8_uuid;
    FRootUuid : f8_uuid;
    FScale : real;
    FSelCount : Integer;
    FDragStart, FDragLastPos : TPoint;
    Fpathname : string;
    blk : PBlk;
    FHilighted : TUiItem;
    FItems : rtkdefs.LIST_ENTRY;
    FItemCount : Integer;
    FInsertionPoint : TPoint;

    FSelectedBlocks : Integer;
    FSelectedPins : Integer;
    FSelectedLinks : Integer;

    FbMouseDownCaptured : boolean;
    FbMouseDblClickCaptured : boolean;

    procedure setScale(scale : real);
    procedure beginInternalDrag(X, Y: Integer);
    procedure internalDragging(x, y: Integer);
    procedure draggerEndDrag(Sender, Target: TObject; X, Y: Integer);
    procedure draggerStartDrag(Sender: TObject;
      var DragObject: TDragObject);
      
    procedure vwMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);

    procedure vwDragDrop(Sender, Source: TObject; X, Y: Integer);
    procedure vwDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);

    procedure vwMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure vwMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure vwDblClick(Sender : tobject);
  protected
    { Protected declarations }
    FOneditBlock : TBlkNotifyEvent;
    FOneditPin : TBlkNotifyEvent;
    FOnAddLink : TBlkNotifyEvent;
    FOnAddBlock : TBlkNotifyEvent;
    FOnBind : TBlkNotifyEvent;
    FOnHint : TOnHintEvent;

    FblkHash : pointer;
    FIsPrinting : boolean;
  public
    FbReadOnly : boolean;
    FbShowGrid : boolean;
    FKernel : Pkernel_t;
    H_MARGIN : integer;
    V_MARGIN : integer;
    TEXT_SHIFT : integer;
    FiGridUnit  : integer;
    FbOnline : boolean;
    FDragTask : TDragTask;
    FDotSize : Integer;
    FNotSize : Integer;
    FPinHeight : Integer;
    SelLink : rtkdefs.LIST_ENTRY;
    hingMessage : string;

  published
    property OnEditBlock : tblknotifyevent read foneditblock write foneditblock stored true;
    property oneditpin : tblknotifyevent read foneditpin write foneditpin stored true;
    property OnAddLink : Tblknotifyevent read fonaddlink write fonaddlink stored true;
    property OnAddBlock : Tblknotifyevent read fonaddblock write fonaddblock stored true;
    property OnBind : Tblknotifyevent read fonbind write fonbind stored true;
    property ShowGrid : boolean read Fbshowgrid write FbShowGrid stored true;
    property OnHint : tOnhintevent read FOnHint write FOnHint stored true;

  public    
    { Public declarations }
    constructor Create(AOwner : TComponent);override;
    destructor Destroy;override;

    procedure Clean;
    procedure Bind(b : PBlk);
    procedure render;
    procedure print;
    procedure renderGrid;
    procedure AddItem(ui : TUiItem; k : Pointer);
    function AddBlock(b: PBlk): TBlk;
    function AddLink(Flink: Pcblk_link_t): TLink;
    function GetCanvas:TCanvas;
    function GetOnline:boolean;
    function GetStarted : boolean;
    function GetItem(idx : Integer):TUiitem;
    function GetItemCount:Integer;
    function GetInsertionPoint:TPoint;
    function GetReadonly : boolean;
    function GetAllBlocks:TList;
    function ItemByPointer(p : Pointer) : TUiItem;

    property Canvas:TCanvas read GetCanvas;
    property Online:Boolean read GetOnline;
    property Started:boolean read GetStarted;
    property ItemCount : integer read GetItemCount;
    property Scale : real read FScale write SetScale;
    property Path : string read Fpathname;
    property Block:PBlk read blk;
    property InsertionPoint : TPoint read GetInsertionPoint;
    property Readonly : boolean read GEtReadOnly;
    property SelCount : Integer read FSelCount;
    property SelBlkCount : Integer read FSelectedBlocks;
    property SelPinCount : Integer read FSelectedPins;
    property SelLinkCount : Integer read FSelectedLinks;

    function ItemFromPoint(x, y: Integer): TUiItem;
    procedure Deselect(it: TUiItem);
    procedure DeselectAll;
    procedure Select(it: TUiItem);overload;
    procedure Select(r : TRect);overload;

    procedure AddToSelection(it : TUiItem);
    procedure RemoveFromSelection(it : TUiItem);

    function  GetBlk(p: PBlk): TBlk;
    function  GetLink(Flink: Pcblk_link_t): TLink;
    function  GetPin(p: Pblk_pin_t): TPin;
    procedure Hilight(ob: TUiItem);
    function  Hilighted: TUiItem;
    function  GetFirstSelection : TUiItem;
    function  GetNextSelection(it : TUiItem) : TUiItem;
    procedure UiInvalidateChanged;
    procedure PrepareCanvas(bZoom : boolean = true);
    procedure ReleaseCanvas;
    procedure DropItem(it : TUiItem);
    function FirstItem:TUiItem;
    function LastItem:TUiItem;
    function PreviousItem(it : TUiItem):TUiItem;
    function NextItem(it : TUiItem):TUiItem;
    procedure PutOnTop(it : TUiItem);

    procedure L2D(Canvas : TCanvas; var x, y:integer);overload;
    function D2L(Canvas : TCanvas; x : Integer):Integer;overload;
    function L2D(Canvas : TCanvas; x : Integer):Integer;overload;
    procedure L2D(canvas : TCanvas; var p : TPoint);overload;
    procedure D2L(canvas : TCanvas; var p : TPoint);overload;
    procedure L2D(Canvas : TCanvas; var s : TSize);overload;
    procedure D2L(Canvas : TCanvas; var s : TSize);overload;
    procedure D2L(Canvas : TCanvas; var x,y: integer);overload;
    procedure D2L(Canvas : TCanvas; var r:TRect);overload;
    procedure L2D(Canvas : TCanvas; var r : TRect);overload;
    function Normalize(x : integer):integer;overload;
    procedure Normalize(var p : TPoint);overload;
    procedure Normalize(var r : TRect);overload;
    procedure Normalize(var a, b: integer);overload;

    procedure vwOnPaint(Sender : TObject);
    procedure UiInvalidate;
    procedure makeVisible(pt : TPoint);
  published
    { Published declarations }
  end;

  (*
      function block's size is determined by the following constraints
      1) must be large enouth to hold all the texts, including
        type name/instance name/input and output pin names/
      2) must be large enouth than some predefined minimums

      metrics :
      h1,h2,h3,w1,w2 are all in normalized coordinates.

    drawing origin
    x-----------------------
    | <instance_name>   h0
    | |-------------|  ---
    --|<ei>    <eo> |-- h1
      |__         __|  ___
        |         |     h2
      --          --|  ---
      | <typename>  |
      |             |  h3
    --|<di>    <do> |--
      --------------- ----

   |w0|  w1   |  w2 |w0|

    h0 = w0 = h2 == FiGridUnit
    width = w0 + w1 + w2 + w0;
  *)
  TBlkMetrics = record
    h, w : integer;
    h0, h1, h2, h3, w1, w2 : integer;
    pt : array[0..11] of tpoint;
  end;

  TBlk = class(TUiItem)
  private
    function isExtensibleBlock: boolean;
    function getPath:string;
  public
    blk : PBlk;
    FMetrics : TBlkMetrics;
    eis, eos, dis, dos : TList;
    FillColor : TColor;
    m_Pins : rtkdefs.LIST_ENTRY;
  public
    constructor create(lk : TBlkLayout);override;
    destructor destroy;override;
    procedure bind(b : PBlk);
    procedure loadMetrics;
    procedure render;override;
    procedure renderText;override;
    procedure renderRect(x, y: integer);override;
    procedure uiOnResize;override;
    function hitTest(x, y:Integer):TUiItem;override;
    function saveToStream(ms : PStream):boolean;override;
    procedure uiInvalidate;override;
    procedure uiInvalidateChanged;override;
    function getPointer:Pointer;override;
    procedure debugDump;override;
    function hintMessage : string; override;
    property Path:string read getPath;
    function  getPin(p : Pblk_pin_t):TPin;
  end;

  TPinMetrics = packed record
    // all in relative coordination
    tip, tep : TPoint;
    tep2 : TPoint;
  end;

  TPin = class(TUiItem)
  public
    FMetrics : TPinMetrics;
    rp, p : Pblk_pin_t;
    last_value : dyn_pin_t;
    last_value_str : string;
    ep : TExtPin;
    FbIsOutput : boolean;
    FPinName : string;
    FPinPath : string;
    OwnerBlk : TBlk;
    m_BlkPinLink : LIST_ENTRY;

    {textTip is the position to output the pin name}
    function TextTip:TPoint;
    {pinTip is the outmost position of the pin bar}
    function pinTip:TPoint;
    {text2Tip is the position to putput value string }
    function Text2Tip : TPoint;
    procedure uiOnStartDrag(x, y:Integer);override;
    procedure uiOnDragging(x, y:Integer);override;
    procedure uiOnEndDrag(Cancelled: Boolean);override;
    function getPointer:Pointer;override;
    procedure loadMetrics;
  public
    constructor Create(lk : TBlkLayout);override;
    function barRect : TRect;
    function BoundingRect:TRect;override;
    procedure Bind(AownerBlk : TBlk; p : Pblk_pin_t);
    procedure render;override;
    procedure renderText;override;
    procedure uiOnResize;override;
    function  HitTest(x, y:Integer):TUiItem;override;
    procedure DebugDump;override;
    procedure UiInvalidate;override;
    procedure UiInvalidateChanged;override;
    function hintMessage : string; override;
    function ValueStr : string;
  end;

  TExtPin = class(TUiItem)
  public
    FMetrics : TPinMetrics;
    rp, p : Pblk_pin_t;
    last_value_str : string;
    op : TPin;
    tep : tpoint; // reference point of text
    tip : tpoint; // reference point of tip
    s  : string;
  public
    constructor Create(lk : TBlkLayout);override;
    procedure Bind(o : TPin);
    procedure render;override;
    procedure renderText;override;
    function  HitTest(x, y:Integer):TUiItem;override;
    procedure DebugDump;override;
    procedure UiInvalidate;override;
    function  ValueStr:string;
    procedure uiOnDrop;override;
    function getPointer:Pointer;override;
  end;

  PLinkMetrics = ^TLinkMetrics;
  TLinkMetrics = packed record
    Count : Integer;
    Points : array[0..31] of TPoint;
  end;

  TReshapeKind = (rkHorizontal,rkVertical,rkDiagonal);

  TLink = class(TUiItem)
  public
    FMetrics : TLinkMetrics;
    Flink : Pcblk_link_t;
    Fsb, Ftb : TBlk;

    constructor Create(lk : TBlkLayout);override;
    destructor Destroy;override;
    procedure Bind(Flink : Pcblk_link_t);
    procedure Calibrate;
    procedure FollowPin(p : TPin);

    procedure render;override;
    procedure renderText;override;
    procedure DrawKnots;
    procedure UiInvalidate;override;
    function  HitTest(x,y:Integer):TUiItem;override;
    procedure uiOnResize;override;

    procedure uiOnStartDrag(x, y:Integer);override;
    procedure uiOnDragging(x, y:Integer);override;
    procedure uiOnEndDrag(Cancelled: Boolean);override;
    procedure UiInvalidateChanged;override;
    function getPointer:Pointer;override;

    procedure InsertPoint(idx : Integer; pt : TPoint);
    procedure RemovePoint(idx : Integer);
    procedure SetPoint(idx : Integer; pt : TPoint);
    procedure CompactPoints;
    procedure DebugDump;override;
    procedure AutoWire;
    function  Source:TPin;
    function  Target:TPin;
    procedure Align;
  protected
    FS1, FS2 : TPoint;
    FReshapeSection : Integer;
    FReshapeKind : TReshapeKind;
    FResPrefix : string;
    FResKey : f8_uuid;
    FLastValue : dyn_pin_t;
  end;

procedure Register;

function Between(min,Max,x:Integer):boolean;overload;
function Between(min,Max,x:real):boolean;overload;
function PointInRect(p : TPoint; r : TRect):boolean;
function UiContainRect(r1, r2 : TRect):boolean;
function UiIntersectRect(r1, r2: TRect): boolean;
procedure PushCanvasState(Canvas : TCanvas);
procedure PopCanvasState(Canvas : TCanvas);
function DefaultCanvas:TRect;
function ContainingUiItem(lk : PLIST_ENTRY):TUiItem;
function ContainingSelectedUiItem(lk : PLIST_ENTRY):TUiItem;
function ContainingBlkPin(lk : PLIST_ENTRY):TPin;
procedure BlkUnimplemented(h : hwnd);
procedure CanvasRTextout(Canvas : TCanvas; p : tpoint; s : string);
procedure UiUnionRect(var r : TRect; r1, r2 : TRect);
procedure UnifyRectD(var r : TRect);

implementation

uses
  printers;
const
  CANVAS_STATE_STACK_DEPTH = 16;
var
  g_CanvasStateStack : array[0..CANVAS_STATE_STACK_DEPTH-1] of TCanvasState;
  g_CanvasStateStackPointer : Integer;
  g_UiItemLinkOffset : integer;
  g_UiSelectedItemLinkOffset : Integer;
  g_Pin_BlkPinLinkOffset : Integer;

function SectionCross(a1,a2,b1,b2:TPoint;var c : TPoint):boolean;forward;

procedure Register;
begin
  RegisterComponents('Standard', [TBlkLayout]);
end;

{ TBlkLayout }

procedure TBlkLayout.Bind(b: PBlk);
var
  sb : Pblk;
  Flink : Pcblk_link_t;
  cb : PCblk;
  oldCursor : TCursor;
begin
  Clean;
  oldCursor := screen.Cursor;
  try
    Fuuid := b.uuid;
    FRootUuid := ke_get_blk(Fkernel, nil)^.h.uuid;
    
    PrepareCanvas;
    screen.cursor := crHourGlass;

    vw.Invalidate;

    blk := b;
    // create child block UI objects
    sb := delphi_firstsub(b);
    while assigned(sb) do
    begin
      AddBlock(sb);
      sb := delphi_next(sb);
    end;

    if b.h.magic = BBLK_MAGIC THEN
    begin
      AddBlock(b);
    end else begin
      cb := Blk2Cblk(b);
      Flink := ICblk_first_link(cb);
      while assigned(Flink) do
      begin
        AddLink(Flink);
        Flink := ICblk_next_link(Flink);
      end;

      FbReadOnly := false;
      if (b.h.flags and BLK_F_READONLY)<>0 then
        FbReadOnly := true;
    end;
  finally
    ReleaseCanvas;
    screen.cursor := oldCursor;
  end;

  self.FHilighted := nil;
  Fpathname := GetBlkPath(blk);

  if assigned(fonbind) then
    fonbind(self, evtOnBind, nil, nil);
end;

procedure TBlkLayout.Clean;
var
  lk : plist_entry;
  ui : TUiItem;
begin
  if GetDebug and false then
    delphi_puts(msg_debug, 'Cleaning layout.');
  while FItemCount>0 do
  begin
    lk := fitems.flink;
    ui := containinguiitem(lk);
    DropItem(ui);
  end;
  initializelisthead(@fitems);
  fitemcount := 0;
  initializelisthead(@selLink);
  FSelCount := 0;
  FSelectedBlocks := 0;
  FSelectedPins := 0;
  FSelectedLinks := 0;
  fInsertionPoint.X := 0;
  finsertionPoint.y := 0;
  ke_clear_hash(FblkHash);
end;

constructor TBlkLayout.Create(AOwner: TComponent);
begin
  inherited;
  
  InitializeListHead(@FItems);
  InitializeListHead(@selLink);
  FItemCount := 0;
  FSelCount := 0;
  FSelectedBlocks:=0;
  FSelectedPins:=0;
  FSelectedLinks:=0;

  FblkHash := ke_init_hash;
  
  pnlBiu := TPanel.Create(Self);
  pnlBiu.Parent := Self;
  pnlBiu.Name := 'pnlBiu';
  self.Ctl3D := false;

  vw := TPaintBox.Create(Self);
  vw.Parent := pnlBiu;
  vw.Align := alClient;
  vw.Name := 'vw';
  pnlBiu.Left := 0;
  pnlBiu.Top := 0;
  // pnlBiu.DoubleBuffered := true;

  dragger := TButton.Create(Self);
  dragger.parent := pnlbiu;
  dragger.Visible := false;
  dragger.Name := 'dragger';

  FInsertionPoint.x := 0;
  FInsertionPoint.y := 0;

  FHilighted := nil;
  FbReadOnly := false;
  FbShowGrid := true;
  FbMouseDownCaptured := false;
  FbMouseDblClickCaptured := false;

  pnlBiu.Visible := true;
  vw.Visible := true;

  vw.OnPaint := vwOnPaint;
  vw.OnMouseDown := vwMouseDown;
  vw.OnMouseUp := vwMouseUp;
  vw.OnDragOver := vwDragOver;
  vw.OnDragDrop := vwDragDrop;
  vw.OnMouseMove := vwMouseMove;
  vw.OnDblClick := vwDblClick;

  dragger.OnStartDrag := draggerStartDrag;
  dragger.OnEndDrag := draggerEndDrag;

  OnMouseWheel := MouseWheel;
end;

procedure TBlkLayout.render;
var
  r : TRect;
  ui : TUiItem;
begin
  PrepareCanvas;

  if not assigned(ke_get_blk(Fkernel, @Fuuid)) then
    bind(ke_get_blk(Fkernel, nil));

  if FbReadOnly or Online then
    Color := pnlBiu.color
  else
    Color := clWhite;

  // reset canvas elements
  Canvas.Brush.Color := Color;
  Canvas.Pen.mode := pmCopy;

  if not Online then
  begin
    // draw background
    r.Left := 0;
    r.Top := 0;
    r.Right := DEFAULT_CANVAS_WIDTH;
    r.Bottom := - DEFAULT_CANVAS_HEIGHT;
    Canvas.FillRect(r);
  end;

  if not (FbReadOnly or Online) then
    renderGrid;

{$DEFINE USE_LAYERED_render}
{
  in layered mode, the objects are rendered in this order

  *) unselected links
  *) unselected blocks and ext-pins
  *) selected links
  *) selected blocks and ext-pins
}

{$IFDEF USE_LAYERED_render}
  ui := firstItem;
  while assigned(ui) do
  begin
    if (ui is TLink) and not (ui.Selected) then
    begin
      ui.render;
    end;
    ui := nextItem(ui);
  end;

  ui := firstItem;
  while assigned(ui) do
  begin
    if (((ui is TBlk) or (ui is TExtPin))) and not ui.Selected then
    begin
      ui.render;
      ui.renderText;
    end;
    ui := nextItem(ui);
  end;

  ui := firstItem;
  while assigned(ui) do
  begin
    if (ui is TLink) and ui.Selected then
    begin
      ui.render;
    end;
    ui := nextItem(ui);
  end;

  ui := firstItem;
  while assigned(ui) do
  begin
    if (((ui is TBlk) or (ui is TExtPin))) and ui.Selected then
    begin
      ui.render;
      ui.renderText;
    end;
    ui := nextItem(ui);
  end;

  ReleaseCanvas;
{$ELSE}
  ui := firstItem;
  while assigned(ui) do
  begin
    ui.render;
    ui.renderText;
    ui := nextItem(ui);
  end;
{$ENDIF}      
end;

function TBlkLayout.AddBlock(b: PBlk):TBlk;
var
  ui : TBlk;
begin
  ui := TBlk.Create(self);
  AddItem(ui, b);
  ui.Bind(b);
  ui.uiOnResize;
  Result := ui;
end;

function TBlkLayout.AddLink(Flink: Pcblk_link_t):TLink;
var
  ui : TLink;
begin
  ui := TLink.Create(self);
  AddItem(ui, flink);
  ui.Bind(Flink);
  Result := ui;
end;

procedure TBlkLayout.L2D(Canvas : TCanvas; var x, y:integer);
var
  p : TPoint;
begin
  p.x := x;
  p.y := y;
  LPtoDP(Canvas.Handle, p, 1);
  x := p.x;
  y := p.y;
end;

function TBlkLayout.D2L(Canvas : TCanvas; x : Integer):Integer;
var
  y : Integer;
begin
  y := 0;
  D2L(Canvas,x,y);
  Result := x;
  x := 0;
  y := 0;
  D2L(Canvas, x, y);
  Result := abs(x - Result);
end;

function TBlkLayout.L2D(Canvas : TCanvas; x : Integer):Integer;
var
  y : Integer;
begin
  y := 0;
  L2D(Canvas,x,y);
  result := x;
  x := 0;
  y := 0;
  L2D(Canvas, x, y);
  Result := abs(x-result);
end;

procedure TBlkLayout.L2D(Canvas : TCanvas; var s : TSize);
begin
  LPtoDP(Canvas.Handle, s, 1);
end;

procedure TBlkLayout.D2L(Canvas : TCanvas; var s : TSize);
begin
  DPtoLP(Canvas.Handle, s, 1);
end;

procedure TBlkLayout.D2L(Canvas : TCanvas; var x,y: integer);
var
  p : TPoint;
begin
  p.x := x;
  p.y := y;
  DPtoLP(Canvas.Handle, p, 1);
  x := p.x;
  y := p.y;
end;

procedure TBlkLayout.D2L(Canvas : TCanvas; var r:TRect);
begin
  DPtoLP(Canvas.Handle, r.left, 1);
  DPtoLP(Canvas.Handle, r.right, 1);
end;

procedure TBlkLayout.L2D(Canvas : TCanvas; var r : TRect);
begin
  LPtoDP(Canvas.Handle, r.left, 1);
  LPtoDP(Canvas.Handle, r.right, 1);
end;

// translate in grid units
procedure TBlkLayout.Normalize(var a, b: integer);
begin
  a := Normalize(a);
  b := Normalize(b);
end;

// translate in grid units
function TBlkLayout.Normalize(x : integer):integer;
begin
  if(x>0) then
  begin
    x := (x + FiGridUnit div 2) div FiGridUnit;
  end else begin
    x := (x - FiGridUnit div 2) div FiGridUnit;
  end;
  result := x * FiGridUnit;
end;

// translate in grid units
procedure TBlkLayout.Normalize(var p : TPoint);
begin
  p.x := Normalize(p.x);
  p.y := Normalize(p.y);
end;

// translate in grid units
procedure TBlkLayout.Normalize(var r : TRect);
begin
  r.left := Normalize(r.left);
  r.top := Normalize(r.top);
  r.right := Normalize(r.right);
  r.bottom := Normalize(r.bottom);
  // UnifyRect(r);
end;

procedure Swap(var a, b: Integer);
var
  tmp : Integer;
begin
  tmp := a;
  a := b;
  b := tmp;
end;

procedure UiUnionRect(var r : TRect; r1, r2 : TRect);
begin
  unifyrectd(r1);
  unifyrectd(r2);
  r.Left := min(r1.left, r2.left);
  r.Right := max(r1.Right, r2.right);
  r.Top := max(r1.Top, r2.top);
  r.Bottom := min(r1.Bottom, r2.bottom);
end;

(*

*)
procedure UnifyRectD(var r : TRect);
begin
  if r.Left > r.Right then
    Swap(r.Left, r.Right);
  if r.Top < r.Bottom then
    Swap(r.Top, r.Bottom);
end;

function TBlkLayout.ItemFromPoint(x, y:Integer):TUiItem;
var
  it : TUiItem;
begin
  it := lastitem;
  while assigned(it) do
  begin
    result := it.HitTest(x, y);
    if assigned(result) then
      Exit;
    it := previousitem(it);
  end;
  Result := nil;
end;

procedure TBlkLayout.Deselect(it: TUiItem);
begin
  it.Selected := false;
end;

procedure TBlkLayout.Select(it: TUiItem);
begin
  it.Selected := true;
end;

procedure TBlkLayout.DeselectAll;
var
  it : TUiItem;
begin
  it := gEtfirstsELECTION;
  while assigned(it) do
  begin
    it.selected := false;
    it := gEtfirstsELECTION;
  end;
end;

procedure TBlkLayout.renderGrid;
var
  x,y : integer;
  tp : TPoint;
begin
  PushCanvasState(Canvas);
  if (FbShowGrid) then
  begin
    Canvas.Pen.Color := TColor(RGB(230,230,230));
    Canvas.Pen.Style := psSolid;

    // draw horizontal lines
    y:=V_MARGIN;
    while y <= DEFAULT_CANVAS_HEIGHT - V_MARGIN do
    begin
      tp.X := Normalize(V_MARGIN);
      tp.Y := Normalize(-y);
      Canvas.MoveTo(tp.x, tp.y);
      tp.x := default_canvas_width - v_margin;
      tp.y := Normalize(-y);
      Canvas.LineTo(tp.x, tp.y);
      Inc(y, FiGridUnit);
    end;

    // vertical lines
    x:=H_MARGIN;
    while x <= DEFAULT_CANVAS_WIDTH - H_MARGIN do
    begin
      tp.x := Normalize(x);
      tp.y := Normalize(-v_margin);
      Canvas.MoveTo(tp.x, tp.y);
      tp.x := Normalize(x);
      tp.Y := Normalize(V_MARGIN-DEFAULT_CANVAS_HEIGHT);
      Canvas.LineTo(tp.x, tp.y);
      Inc(x, FiGridUnit);
    end;
    Canvas.Pen.Color := clBtnText;
  end;
(*
  Canvas.MoveTo(FiGridUnit, -FiGridUnit);
  Canvas.LineTo(DEFAULT_CANVAS_WIDTH - FiGridUnit, -DEFAULT_CANVAS_HEIGHT + FiGridUnit);
  Canvas.MoveTo(DEFAULT_CANVAS_WIDTH - FiGridUnit, -FiGridUnit);
  Canvas.LineTo(FiGridUnit, -DEFAULT_CANVAS_HEIGHT + FiGridUnit);
*)
  PopCanvasState(Canvas);
end;

procedure TBlkLayout.internalDragging(x, y: Integer);
var
  it : TUiItem;
  r : TRect;
begin
  if FDragTask = dgRect then
  begin
    Canvas.Pen.Mode := pmNotXor;
    Canvas.Pen.Style := psDash;
    Canvas.Brush.Color := Color;
    Canvas.Brush.Style := bsClear;

    r.left := dragger.left;
    r.top := dragger.Top;
    r.right := FDragLastPos.x;
    r.bottom := FDragLastPos.y;
    // Canvas.FrameRect(r);
    Canvas.Rectangle(r);

    FDragLastPos.x := X;
    FDragLastPos.y := Y;

    r.left := dragger.left;
    r.top := dragger.Top;
    r.right := FDragLastPos.x;
    r.bottom := FDragLastPos.y;
    // Canvas.FrameRect(r);
    Canvas.Rectangle(r);
    Exit;
  end;
  
  if FHilighted is TLink then
    FHilighted.uiOnDragging(x-FDragStart.X, y-FDragStart.Y)
  else if FHilighted is TPin then
    FHilighted.uiOnDragging(x, y)
  else begin
    it := FirstItem;
    while assigned(it) do begin
      if it.Selected then begin
        if (it is TBlk) then
          it.uiOnDragging(x-FDragStart.X, y-FDragStart.Y);
      end;
      it := NextItem(it);
    end;
  end;
  FDragLastPos.X := x;
  FDragLastPos.Y := y;
  dragger.dragcursor := hilighted.dragcursor;
end;

procedure TBlkLayout.Select(r: TRect);
var
  it : TUiItem;
begin
{$IFDEF MOUSE_DEBUG}
  if getdebug then
    writeln('Selecting [', r.left, ',', r.top, ']-[', r.right, ',', r.bottom, ']');
{$ENDIF}
  it := firstitem;
  if r.Bottom>r.Top then
    while assigned(it) do begin
      if it.IntersectRect(r) then
        Select(it);
      it := nextitem(it);
    end
  else
    while assigned(it) do begin
      if UiContainRect(r, it.BoundingRect) then
        Select(it);
      it := nextitem(it);
    end
end;

function TBlkLayout.GetBlk(p: PBlk): TBlk;
begin
  Result := itemByPointer(p) as TBlk;
end;

function TBlkLayout.GetLink(Flink: Pcblk_link_t): TLink;
var
  it : TUiItem;
begin
  it := self.FirstItem;
  while assigned(it) do
  begin
    if ((it is TLink) and ((it as TLink).Flink = Flink)) then
    begin
      Result := it as TLink;
      Exit;
    end;
    it := self.NextItem(it);
  end;
  Result := nil;
end;

function TBlkLayout.GetPin(p: Pblk_pin_t): TPin;
var
  blk : TBlk;
begin
  blk := GetBlk(p.blk);
  assert(assigned(blk));
  Result := blk.GetPin(p);
end;

procedure TBlkLayout.Hilight(ob: TUiItem);
begin
  FHilighted := ob;
end;

function TBlkLayout.Hilighted: TUiItem;
begin
  Result := FHilighted;
end;

procedure TBlkLayout.UiInvalidateChanged;
var
  uit : TUiItem;
begin
  PrepareCanvas;
  uit := self.FirstItem;
  while assigned(uit) do begin
    uit.UiInvalidateChanged;
    uit := self.NextItem(uit);
  end;
  ReleaseCanvas;
end;

constructor TBlk.Create(lk : TBlkLayout);
begin
  inherited Create(lk);
  InitializeListhead(@m_Pins);
  Moveable := true;
  eis := TList.Create;
  eos := TList.Create;
  dis := TList.Create;
  dos := TList.Create;
  FMetrics.h := -1;
end;

procedure TBlk.bind(b : PBlk);
var
  p,rp : Pblk_pin_t;
  Pos : TPoint;
  pin : TPin;
  bAutoScan : boolean;
begin
  Self.blk := b;
  eis.Clear;
  eos.Clear;
  dis.Clear;
  dos.Clear;

  Pos := GetBlkPos(b);
  Width := FMetrics.w;
  Height := FMetrics.h;
  Left := Pos.X;
  Top := Pos.Y;

  bAutoScan := (blk.h.flags and BLK_F_AUTOSCAN)<>0;

  p := b.vtbl.first_pin(b);
  while assigned(p) do begin
    rp := IBlk_real_pin(p);
    if (rp.flags and pin_f_hidden)=0 then begin
      case rp._class of
      PIN_CLASS_EI:
        if not bAutoScan then
          eis.Add(p);
      PIN_CLASS_EO:
        eos.Add(p);
      PIN_CLASS_DI:
        dis.Add(p);
      PIN_CLASS_DO:
        dos.Add(p);
      end;
    end;
    p := b.vtbl.next_pin(p);
  end;

  loadMetrics;
  
  p := b.vtbl.first_pin(b);
  while assigned(p) do begin
    rp := IBlk_real_pin(p);
    if (rp.flags and pin_f_hidden)=0 then begin
      if not (bAutoScan and (rp._class=PIN_CLASS_EI)) then begin
        pin := TPin.Create(Layout);
        pin.Bind(self, p);
        InsertTailList(@m_Pins, @pin.m_BlkPinLink);
        associate(pin);
        Layout.AddItem(pin, p);
      end;
    end;
    p := b.vtbl.next_pin(p);
  end;
  
  Fpathname := getblkpath(blk);

  if blk.h.magic = cblk_magic then
    FillColor := rgb(13,224,212)
  else if (blk^.h.flags and BLK_F_DISABLED)<>0 then
    FillColor := rgb(64,64,64)
  else
    FillColor := rgb(253,216,198);
end;

procedure TBlk.render;
var
  tp : TPin;
  lk : Plist_entry;
begin
  PushCanvasState(Canvas);

  if(Selected) then begin
    Canvas.Pen.Color := clBlue;
    Canvas.Pen.Width := Layout.D2L(Canvas, 2);
  end else begin
    Canvas.Pen.Color := clBlack;
    Canvas.Pen.Width := Layout.D2L(Canvas, 1);
  end;

  canvas.brush.Style := bssolid;
  canvas.Brush.Color := FillColor;
  renderRect(Left, Top);

  Canvas.Pen.Width := Layout.D2L(Canvas, 1);

  lk := m_Pins.flink;
  while lk<>@m_Pins do
  begin
    tp := containingBlkPin(lk);
    tp.render;
    lk := lk.flink;
  end;

  PopCanvasState(Canvas);
end;

procedure TPin.render;
var
  r : TRect;
  pv:boolean;
begin
  PushCanvasState(Canvas);

  r := barRect;
  Canvas.Brush.Color := clBlack;
  if (rp._type = pin_t_bool) and (layout.Online) then begin
    pv := rp.dp.d.i8 <>0;
    if (rp.flags and PIN_F_INVERTED)<>0 then
      pv := not pv;
    if  pv then
      canvas.Brush.Color := clgreen
    else
      canvas.brush.color := clred;
  end;
  Canvas.FillRect(r);

  last_value := rp^.dp^;

  if (rp._type = pIN_T_BOOL) and ((rp.flags and pin_f_inverted)<>0) then begin
    r.Top := top-Height div 2 + Layout.FNotSize;
    if FbIsOutput then begin
      r.Left := Left;
    end else begin
      r.Left := pinTip.X + Width - 2 * Layout.FNotSize;
    end;
    r.Right := r.Left + Layout.FNotSize * 2;
    r.Bottom := r.Top - Layout.FNotSize * 2;
    Canvas.Brush.Color := Layout.Color;
    Canvas.Ellipse(r);
  end;

  PopCanvasState(Canvas);
end;

constructor TLink.Create(lk : TBlkLayout);
begin
  inherited Create(lk);
  // Selectable := false;
  Fsb := nil;
  Ftb := nil;
end;

procedure TLink.Bind(Flink : Pcblk_link_t);
var
  blk : PBlk;
  id : integer;
  ublk : tblk;
  uip : tpin;
begin
  Self.Flink := Flink;

  blk := Cblk2Blk(Flink.blk);
  id  := Icblk_id_by_link(Flink);
  if (blk.h.flags and BLK_F_READONLY)<>0 then begin
    FResKey := blk.h.uuid;
    FResPrefix := Format('!l%d!', [id]);
  end else begin
    FResKey := Flink.uuid;
    FResPrefix := '';
  end;

  ublk := layout.GetBlk(Flink.s.blk);
  assert(assigned(ublk));
  uip  := ublk.GetPin(Flink.s);
  assert(assigned(uip));
  uip.Associate(self);
  ublk := layout.GetBlk(Flink.t.blk);
  assert(assigned(ublk));
  uip  := ublk.GetPin(Flink.t);
  assert(assigned(uip));
  uip.Associate(self);

  Calibrate;

  Fpathname := '(' + getpinpath(Flink.s) + '->' + getpinpath(Flink.t) + ')';
end;

procedure TLink.render;
var
  i : Integer;
  rp : Pblk_pin_t;
  vl : boolean;
begin
  PushCanvasState(Canvas);

  Canvas.Pen.Style := psSolid;
  if Selected then begin
    Canvas.Pen.Width := Layout.D2L(Canvas, 2);
    Canvas.Pen.Color := clBlue;
  end else begin
    Canvas.Pen.Width := 1;
    Canvas.Pen.Color := clBlack;
  end;

  rp := IBlk_real_pin(Flink.s);
  if Layout.Online then
    if rp._type=PIN_T_BOOL then begin
      vl := rp.dp.d.u8<>0;
      if (rp.flags and pin_f_inverted)<>0 then
        vl := not vl;
      if vl then
        Canvas.Pen.Color := clGreen
      else
        Canvas.Pen.Color := clRed;
    end;

  with FMetrics do begin
    Canvas.MoveTo(layout.Normalize(Points[0].X), layout.Normalize(Points[0].Y));
    for i:=1 to FMetrics.Count - 1 do begin
      Canvas.LineTo(layout.Normalize(Points[i].X), layout.Normalize(Points[i].Y));
    end;
  end;

  // draw knots
  Canvas.Brush.Color := Canvas.Pen.Color;
  DrawKnots;

  PopCanvasState(Canvas);
end;

function TUiItem.BoundingRect: TRect;
begin
  Result.Left := Left;
  Result.Right := Left + Width;
  Result.Top := Top;
  Result.Bottom := Top - Height;
end;

constructor TUiItem.Create(lo : TBlkLayout);
begin
  Layout := lo;
  Selectable := true;
  Copiable := false;
  Moveable := false;
  Sizable := false;

  DragCursor := crSizeAll;
  InitializeListHead(@link);
  InitializeListHead(@selLink);
  FAssociated := tlist.Create;
  Fassociatedmaster := tlist.Create;
end;

constructor TPin.Create(lk : TBlkLayout);
begin
  inherited Create(lk);
  Selectable := true;
  ep := nil;
  InitializeListHead(@m_BlkPinLink);
end;

procedure TPin.bind(AownerBlk : TBlk; p : Pblk_pin_t);
begin
  Self.p := p;
  rp := IBlk_real_pin(p);
  FbIsOutput := IBlk_is_output(p)<>0;
  last_value_str := '';
  Fpathname := GetPinpath(p);
  Fpinname := IBlk_pin_name(p);
  Fpinpath := GetPinPath(p);
  ownerBlk := AownerBlk;

  loadMetrics;
  
  if assigned(p.ulink) then begin
    ep := Textpin.Create(layout);
    ep.Bind(self);
    layout.AddItem(ep, ep.p);
  end;
end;

procedure TBlk.renderRect( x, y: integer);
var
  pp : array[0..11] of tpoint;
  i : integer;
begin
  for i:=0 to 11 do begin
    pp[i].x := Fmetrics.pt[i].X + x;
    pp[i].y := Fmetrics.pt[i].y + y;
    layout.Normalize(pp[i]);
  end;

  if (pp[0].x <> pp[11].x) and getdebug then
    writeln('Skew detected.');

  canvas.Polygon(
    [
      pp[0],
      pp[1],
      pp[2],
      pp[3],
      pp[4],
      pp[5],
      pp[6],
      pp[7],
      pp[8],
      pp[9],
      pp[10],
      pp[11],
      pp[0]
    ]
  );
end;

function Between(min,Max,x:Integer):boolean;
begin
  Result := ((x>min) and (x<Max)) or ((x<min) and (x>Max));
end;

function Between(min,Max,x:real):boolean;
begin
  Result := ((x>min) and (x<Max)) or ((x<min) and (x>Max));
end;

function PointInRect(p : TPoint; r : TRect):boolean;
begin
  Result := False;
  if not Between(r.Left, r.Right, p.x) then
    Exit;
  if not Between(r.Top, r.Bottom, p.y) then
    Exit;
  Result := True;
end;

procedure TUiItem.DebugDump;
begin
  if GetDebug then
    Writeln(
      'Item ', ClassName,
      ',Pos=(', Left, ',', Top,
      '),Width=', Width, ',Height=', Height
      );
end;

function TUiItem.GetCanvas: TCanvas;
begin
  Result := Layout.Canvas;
end;

function TUiItem.GetCanvasHandle: hwnd;
begin
  result := layout.pnlbiu.handle;
end;

function TUiItem.HitTest(x, y: Integer): TUiItem;
var
  p : TPoint;
begin
  p.x := x;
  p.y := y;
  Result := nil;
  if PointInRect(p, BoundingRect) then begin
    DragCursor := crHandPoint;
    Result := Self;
  end;
end;

function TUiItem.IntersectRect(r: TRect): boolean;
var
  rb : TRect;
begin
  rb := BoundingRect;
  Result := UiIntersectRect(rb, r);
end;

function SingleIntersectRect(r1, r2: TRect): boolean;
var
  p : TPoint;
begin
  Result := true;
  if PointInRect(r1.TopLeft, r2) then
    Exit;
  p.X := r1.Right;
  p.Y := r1.Top;
  if PointInRect(p, r2) then
    Exit;
  if PointInRect(p, r2) then
    Exit;
  p.X := r1.Left;
  p.Y := r1.Bottom;
  if PointInRect(p, r2) then
    Exit;
  Result := False;
end;

function UiContainRect(r1, r2 : TRect):boolean;
begin
  result := pointinrect(r2.TopLeft, r1) and pointinrect(r2.BottomRight, r1);
end;

function UiIntersectRect(r1, r2: TRect): boolean;
begin
  Result := SingleIntersectRect(r1, r2) or SingleIntersectRect(r2, r1);
end;

procedure TUiItem.renderRect(x, y: Integer);
begin
  //
end;

function TUiItem.SaveToStream(ms : PStream):boolean;
begin
  //
  Result := false;
end;

procedure TUiItem.SetSelected(s: boolean);
begin
  if FSelected = s then
    Exit;
  UiInvalidate;
  FSelected := s;
  if FSelected then
    Layout.AddToSelection(Self)
  else
    Layout.RemoveFromSelection(Self);
end;

procedure TUiItem.ShowPropertyPage(p : TForm);
begin
  //
end;

procedure TUiItem.UiInvalidate;
var
  rr : TRect;
begin
  Color := Layout.Color;
  rr := BoundingRect;
  Layout.L2D(Canvas, rr);
  UnifyRectD(rr);
  InflateRect(rr, Layout.TEXT_SHIFT, -Layout.TEXT_SHIFT);
  InvalidateRect(canvashandle, @rr, false);
end;

procedure TUiItem.UiInvalidateChanged;
begin
  // UiInvalidate;
end;

procedure TUiItem.uiOnDragging(x, y: Integer);
begin
  PushCanvasState(Canvas);

  Canvas.Pen.Mode := pmNotXor;
  Canvas.Pen.Style := psDash;
  canvas.Brush.Style := bsClear;
  // erase old
  if not FbStartDrag then
    renderRect(FDragLastPos.X, FDragLastPos.Y);

  FbStartDrag := false;
  
  // draw new
  FDragLastPos.X := Layout.Normalize(Left + x);
  FDragLastPos.Y := Layout.Normalize(Top + y);
  renderRect(FDragLastPos.X, FDragLastPos.Y);

  PopCanvasState(Canvas);
end;

procedure TUiItem.uiOnEndDrag(Cancelled: Boolean);
begin
  PushCanvasState(Canvas);

  if not FbStartDrag then begin
    // erase last drawn object
    Canvas.Pen.Mode := pmNotXor;
    Canvas.Pen.Style := psDash;
    renderRect(FDragLastPos.X, FDragLastPos.Y);
  end;

  if not Cancelled then begin
    // Erase old position
    UiInvalidate;
    // move to new position
    Left := Layout.Normalize(FDragLastPos.X);
    Top := Layout.Normalize(FDragLastPos.Y);
    uiOnResize;
    UiInvalidate;
  end;

  PopCanvasState(Canvas);
end;

procedure TUiItem.uiOnStartDrag(x, y:Integer);
begin
  FbStartDrag := true;
end;

procedure TBlk.uiOnResize;
var
  k : string;
  pos : TPoint;
  uip : TPin;
  lk : Plist_entry;
begin
  inherited;
  pos.x := Left;
  pos.y := Top;
  k := Fpathname;
  SetBlkPos(blk, pos);

  lk := m_Pins.fLink;
  while lk<>@m_Pins do begin
    uip := ContainingBlkPin(lk);
    uip.uiOnResize;
    lk := lk.Flink;
  end;
end;

procedure TPin.loadMetrics;
var
  blk : TBlk;
begin
  blk := OwnerBlk;

  Width := blk.FMetrics.h0;
  Height := Layout.FPinHeight;

  case rp._class of
  PIN_CLASS_EI,PIN_CLASS_DI:
  begin
    FMetrics.Tip.x := 0;
    FMetrics.Tep.x := blk.FMetrics.h0 + Layout.TEXT_SHIFT * 2;
  end;
  PIN_CLASS_EO,PIN_CLASS_DO:
  begin
    FMetrics.tip.x := blk.Width;
    FMetrics.tep.x := FMetrics.tip.x - blk.FMetrics.h0 - 2*Layout.TEXT_SHIFT;
  end;
  end;

  case rp._class of
  PIN_CLASS_EI:
    FMetrics.tip.y := -(blk.eis.IndexOf(p)+1) * blk.FMetrics.h0;
  PIN_CLASS_EO:
    FMetrics.tip.y := -(blk.eos.IndexOf(p)+1) * blk.FMetrics.h0;
  PIN_CLASS_DI:
    FMetrics.tip.y := -(blk.dis.IndexOf(p)+2) * blk.FMetrics.h0
      - blk.FMetrics.h1 - blk.FMetrics.h2;
  PIN_CLASS_DO:
    FMetrics.tip.y := -(blk.dos.IndexOf(p)+2) * blk.FMetrics.h0
      - blk.FMetrics.h1 - blk.FMetrics.h2;
  end;

  FMetrics.tip.Y := FMetrics.tip.y - blk.FMetrics.h0 div 2;
  FMetrics.tep.y := FMetrics.tip.y + blk.Fmetrics.h0 div 2 - Layout.TEXT_SHIFT;

  FMetrics.tep2.X := FMetrics.tip.X;
  FMetrics.tep2.Y := FMetrics.tip.Y+ layout.FiGridUnit - layout.TEXT_SHIFT;
  if FbIsOutput then begin
    Inc(FMetrics.tep2.x, layout.TEXT_SHIFT);
  end else begin
    Dec(FMetrics.tep2.x, layout.TEXT_SHIFT);
  end;
end;

function TPin.TextTip: TPoint;
var
  blk : TBLk;
begin
  blk := OwnerBlk;
  Result.X := blk.Left + self.FMetrics.tep.x;
  Result.y := blk.Top + self.FMetrics.tep.y;
end;

function TPin.pinTip: TPoint;
begin
  Result.X := ownerBlk.Left + Self.FMetrics.tip.X;
  Result.Y := self.Top - Self.Height div 2;
  // layout.L2D(canvas, result);
  // layout.d2l(canvas, result);
end;

function TBlk.GetPin(p : PBlk_pin_t): TPin;
var
  lk : PList_entry;
  it : TPin;
begin
  lk := m_Pins.Flink;
  while lk<>@m_Pins do begin
    it := ContainingBlkPin(lk);
    if (it.p = p) then begin
      Result := it;
      Exit;
    end;
    lk := lk.Flink;
  end;
  Result := nil;
end;

procedure TLink.Calibrate;
var
  res : PLinkMetrics;
begin
  res := PLinkMetrics(get_res_buf(@FResKey,nil,pchar(FResPrefix + 'metrics')));
  if not assigned(res) then
    AutoWire
  else
    FMetrics := res^;
  CompactPoints;
end;

procedure TPin.uiOnResize;
var
  uilk : TLink;
  blk : TBlk;
  it : TUiItem;
  i : Integer;
begin
  blk := OwnerBlk;
  case rp._class of
  PIN_CLASS_EI,PIN_CLASS_DI:
    Left := blk.Left;
  PIN_CLASS_EO,PIN_CLASS_DO:
    Left := blk.Left + FMetrics.tip.x - blk.FMetrics.h0;
  end;

  Top := FMetrics.tip.y + blk.Top + Height div 2;
  Top := Layout.Normalize(Top) + Layout.D2L(Canvas,1);

  // do calibration on behalf of associated external pins
  if assigned(ep) then begin
    ep.top := pinTip.Y + layout.Figridunit div 2;
    ep.Width := 2 * layout.FiGridUnit;
    ep.Height := layout.FiGridUnit;
    ep.tip.Y := pinTip.y;
    ep.tep.y := pinTip.Y + layout.FiGridUnit - layout.TEXT_SHIFT;
    if FbIsOutput then begin
      ep.Left := pinTip.X + 3 * layout.FiGridUnit;
      ep.tip.X := ep.Left;
      ep.tep.X := ep.left + 2 * layout.Figridunit + layout.TEXT_SHIFT;
    end else begin
      ep.tip.x := pinTip.X - 3 * layout.FiGridUnit;
      ep.Left := ep.tip.x - 2 * layout.Figridunit;
      ep.tep.x := ep.Left - layout.TEXT_SHIFT;
    end;
  end;

  for i:=0 to FAssociated.Count-1 do begin
    it := TUiItem(FAssociated.Items[i]);
    if it is TLink then begin
      uilk := it as TLink;
      uilk.UiInvalidate;
      uilk.FollowPin(self);
      uilk.UiInvalidate;
    end;
  end;
end;

procedure TLink.UiInvalidate;
var
  i : Integer;
  r : TRect;
begin
  Color := Layout.Color;
  for i:=0 to FMetrics.Count-2 do begin
    r.Left := FMetrics.Points[i].X;
    r.Top := FMetrics.Points[i].Y;
    r.Right := FMetrics.Points[i+1].X;
    r.Bottom := FMetrics.Points[i+1].Y;
    UnifyRectD(r);
    Layout.L2D(Canvas, r);
    InflateRect(r, 3, -3);
    InvalidateRect(canvasHandle, @r, false);
  end;
end;

function TBlk.isExtensibleBlock:boolean;
var
  s : string;
begin
  s := IBLk_class_name(blk);
  result:=(s='AND_BOOLX') or (s='OR_BOOLX');
end;

function TBlk.HitTest(x, y: Integer): TUiItem;
var
  r : TRect;
  p : TPoint;
  r2 : TRect;
begin
  r := BoundingRect;
  p.x := x;
  p.y := y;
  if isExtensibleBlock then begin
    r2 := r;
    r2.Top := r2.Bottom + layout.Figridunit;
    UnifyRectD(r2);
    if PointInRect(p,r2) then begin
      Result := self;
      DragCursor := crSizeNs;
      exit;
    end;
  end;
  UnifyRectD(r);
  InflateRect(r, -layout.Figridunit*2, 0);
  Result := nil;
  if PointInRect(p,r) then begin
    Result := Self;
    DragCursor := crSizeAll;
    Exit;
  end;
end;

function TLink.HitTest(x, y: Integer): TUiItem;
var
  i : Integer;
  r : TRect;
  p : TPoint;
begin
  p.x := x;
  p.y := y;
  Result := nil;
  for i:=0 to FMetrics.Count-2 do begin
    r.Left := FMetrics.Points[i].X;
    r.Top := FMetrics.Points[i].Y;
    r.Right := FMetrics.Points[i+1].X;
    r.Bottom := FMetrics.Points[i+1].Y;
    if not ((r.left=r.right) or (r.top=r.bottom)) then begin
      assert(false);
      Exit;
    end;
    UnifyRectD(r);
    InflateRect(r, Layout.FiGridUnit div 2, -Layout.FiGridUnit div 2);
    if not PointInRect(p, r) then
      Continue;
    Result := Self;
    {test if the section under cursor is horizontal or vertical}
    if FMetrics.points[i].x=FMetrics.points[i+1].x then begin
      {vertical, horizontal reshaping is possible}
      DragCursor := crSizeWE;
      FReshapeKind := rkHorizontal;
    end else begin
      assert(FMetrics.points[i].y=FMetrics.points[i+1].y);
      DragCursor := crSizeNS;
      FReshapeKind := rkVertical;
    end;
    FReshapeSection := i;
{
    if getDebug then
      writeln('Reshape link at section ',integer(FReshapeSection),' mode: ',FReshapeKind=rkHorizontal);
}
    Exit;
  end;
end;

procedure TLink.uiOnDragging(x, y: Integer);
var
  p1, p2 : TPoint;
begin
  // Writeln('Dragging ', x, ',', y);
  if Layout.FDragTask<>dgReshapeLink then
    Exit;

  PushCanvasState(Canvas);

  Canvas.Pen.Mode := pmNotXor;
  Canvas.Pen.Style := psDash;

  p1 := FS1;
  p2 := FS2;

  if not FbStartDrag then begin
    // erase old
    case FReshapeKind of
    rkHorizontal:
    begin
      if p1.x<>p2.x then
        assert(false);
      Inc(p1.X, FDragLastPos.x);
      Inc(p2.x, FDragLastPos.x);
    end;
    rkVertical:
    begin
      if p1.y<>p2.y then
        assert(false);
      Inc(p1.y,FDragLastPos.y);
      Inc(p2.y,FDragLastPos.y);
    end;
    end;
    Canvas.MoveTo(p1.X, p1.y);
    Canvas.LineTo(p2.x, p2.y);
  end;

  FbStartDrag := false;

  x := Layout.Normalize(x);
  y := Layout.Normalize(y);

  // draw new
  p1 := FS1;
  p2 := FS2;
  case FReshapeKind of
  rkHorizontal:
  begin
    Inc(p1.X, x);
    Inc(p2.x, x);
  end;
  rkVertical:
  begin
    Inc(p1.y,y);
    Inc(p2.y,y);
  end;
  end;
  Canvas.MoveTo(p1.X, p1.y);
  Canvas.LineTo(p2.x, p2.y);

  FDragLastPos.X := x;
  FDragLastPos.Y := y;

  PopCanvasState(Canvas);
end;

procedure TLink.uiOnEndDrag(Cancelled: Boolean);
begin
  if Cancelled then
    Exit;

  if Layout.FDragTask<>dgReshapeLink then
    Exit;

  UiInvalidate;

  if FReshapeSection=0 then begin
    // insert a point
    InsertPoint(1,Source.pinTip);
    Inc(FReshapeSection);
  end;
  if FReshapeSection=FMetrics.Count-2 then
    InsertPoint(FMetrics.Count-2, Target.pinTip);

  with FMetrics do begin
    case FReshapeKind of
    rkHorizontal:
    begin
      Inc(FS1.x, FDragLastPos.x);
      Inc(FS2.X, FDragLastPos.x);
    end;
    rkVertical:
    begin
      Inc(FS1.y, FDragLastPos.y);
      Inc(FS2.y, FDragLastPos.y);
    end;
    end;
  end;

  SetPoint(FReshapeSection, FS1);
  SetPoint(FReshapeSection+1, FS2);

  uiOnResize;
  UiInvalidate;
end;

procedure PushCanvasState(Canvas : TCanvas);
begin
  if g_CanvasStateStackPointer = 0 then
  //  raise Exception.Create('CanvasStackOverflow');
    exit;
  Dec(g_CanvasStateStackPointer);
  with g_CanvasStateStack[g_CanvasStateStackPointer] do begin
    PenColor := Canvas.Pen.Color;
    PenWidth := Canvas.Pen.Width;
    PenStyle := Canvas.Pen.Style;
    PenMode  := Canvas.Pen.Mode;
    BrushColor := Canvas.Brush.Color;
  end;
end;

procedure PopCanvasState(Canvas : TCanvas);
begin
  if g_CanvasStateStackPointer = CANVAS_STATE_STACK_DEPTH - 1 then
  //  raise Exception.Create('CanvasStackUnderflow');
    exit;
  with g_CanvasStateStack[g_CanvasStateStackPointer] do begin
    Canvas.Pen.Color := PenColor;
    Canvas.Pen.Width := PenWidth;
    Canvas.Pen.Style := PenStyle;
    Canvas.Pen.Mode  := PenMode;
    Canvas.Brush.Color := BrushColor;
  end;
  Inc(g_CanvasStateStackPointer);
end;

procedure TLink.uiOnResize;
begin
  CompactPoints;
  set_res_buf(
    @FResKey,
    @FMetrics,
    sizeof(FMetrics),
    pchar(FResPrefix + 'metrics')
    );
end;

// called when the pertaining block is moving
procedure TLink.FollowPin(p: TPin);
var
  idx : Integer;
  tp : TPoint;
  b1,b2:TBlk;
begin
  assert((p.p=Flink.s) or (p.p=Flink.t));
  UiInvalidate;
  tp := p.pinTip;
  // if both end moved?
  b1 := (Layout).GetBlk(Flink.s.blk);
  b2 := (Layout).GetBlk(Flink.t.blk);
  if b1.Selected and b2.Selected then begin
    if p.FbIsOutput then
    with Fmetrics do begin
      tp.X := p.pinTip.X - Points[0].X;
      tp.y := p.pinTip.y - Points[0].y;
      for idx := 0 to FMetrics.Count-1 do begin
        Inc(Points[idx].X, tp.x);
        Inc(Points[idx].y, tp.y);
      end;
      uiOnResize;
      Align;
      UiInvalidate;
    end;
    Exit;
  end;
  with FMetrics do begin
    if p.p=Flink.s then begin
      RemovePoint(0);
      InsertPoint(0, tp);
      tp.X := Points[1].X;
      InsertPoint(1, tp);
    end else begin
      RemovePoint(Count-1);
      // note count's changed by above statement
      InsertPoint(Count, tp);
      // now, count was restalled by last statement
      tp.X := Points[Count-2].X;
      InsertPoint(Count-1, tp);
    end;
    CompactPoints;
  end;
  UiInvalidate;
  uiOnResize;
end;

procedure TLink.uiOnStartDrag(x, y: Integer);
begin
  inherited;
  if Layout.FDragTask<>dgReshapeLink then
    Exit;
  hitTest(x,y);

  FS1 := FMetrics.Points[FReshapeSection];
  FS2 := FMetrics.Points[FReshapeSection+1];
end;

{
  after insertion, pt will become the idx-th point
  idx is from 0 through FMetrics.Count
}
procedure TLink.InsertPoint(idx: Integer; pt: TPoint);
var
  i : Integer;
begin
  if idx>FMetrics.Count then
    raise Exception.Create('Too many points.');
  for i:= FMetrics.Count-1 downto idx do
    FMetrics.Points[i+1] := FMetrics.Points[i];
  FMetrics.Points[idx] := pt;
  Inc(FMetrics.Count);
end;

procedure TLink.RemovePoint(idx: Integer);
begin
  {
  if FMetrics.Count <= 2 then
    raise Exception.Create('Too little points.');
  if (idx=0) or (idx=FMetrics.Count-1) then
    raise Exception.Create('Cannot delete end point');
  }
  // unimplemented
  for idx:=idx to FMetrics.Count-2 do
    FMetrics.Points[idx] := FMetrics.Points[idx+1];
  Dec(FMetrics.Count);
end;

procedure TLink.SetPoint(idx: Integer; pt: TPoint);
begin
  if not ((idx>=0) and (idx<=FMetrics.Count-1)) then begin
    Writeln('idx=',idx,',Count=',FMetrics.Count,',Reshape=', FReshapeSection);
    assert(false);
  end;
  FMetrics.Points[idx] := pt;
end;

function TBlk.SaveToStream(ms: PStream): boolean;
begin
  result := true;
end;

procedure TLink.DrawKnots;
var
  lk : Pcblk_link_t;
  i, j: Integer;
  ulk : TLink;
  tp : TPoint;
begin
  lk := Icblk_next_connection(Flink.s, Flink);
  while assigned(lk) do
  begin
    // draw knots of lk and Flink
    ulk := (Layout).ItemByPointer(lk) as TLink;
    for i:=0 to FMetrics.Count-2 do
      for j:=0 to ulk.FMetrics.Count-2 do begin
        // find cross points of Flink's section[i] and ulk's section[j]
        if SectionCross(
            FMetrics.Points[i],
            FMetrics.Points[i+1],
            ulk.FMetrics.Points[j],
            ulk.FMetrics.Points[j+1],
            tp
            ) then begin
          tp.x := layout.Normalize(tp.x);
          tp.y := layout.Normalize(tp.y);
          Canvas.Ellipse(
            tp.x-Layout.FDotSize,
            tp.y-Layout.FDotSize,
            tp.x+Layout.FDotSize,
            tp.y+Layout.FDotSize
            );
        end;
      end;
    lk := ICblk_next_connection(Flink.s, lk);
  end;
end;

function SectionCross(a1,a2,b1,b2:TPoint;var c : TPoint):boolean;
var
  anga,angb : integer;
procedure SwapPoint(var a,b:TPoint);
var
  tp : TPoint;
begin
  tp:=a;
  a:=b;
  b:=tp;
end;
begin
  Result := false;

  if a1.X = a2.X then
    anga := 90
  else
    anga := 0;
  if b1.X = b2.X then
    angb := 90
  else
    angb := 0;
  if anga = angb then
    Exit;

  // ok, one vertical and one horizontal
  if anga=90 then begin
    SwapPoint(a1,b1);
    SwapPoint(a2,b2);
  end;

  if a1.X >= a2.X then
    SwapPoint(a1,a2);
  if b1.Y < b2.y then
    SwapPoint(b1,b2);

  // ok, now a is horizontal, and a1 is on the
  // left of a2, b1 is above b2

  // 1) b1 should be between a1 and a2
  if (b1.X<a1.X) or (b1.x > a2.x) then
    Exit;
  if (a1.y>b1.y) or (a1.y<b2.y) then
    Exit;
  if (b1.x=a1.x) and (b1.y=a1.y) then
    Exit;
  if (b1.x=a2.x) and (b1.y=a2.y) then
    Exit;
  if (b2.x=a1.x) and (b2.y=a1.y) then
    Exit;
  if (b2.x=a2.x) and (b2.y=a2.y) then
    Exit;

  // ok, crossing point got
  c.X := b1.X;
  c.y := a1.Y;

  Result := true;
end;

procedure TLink.CompactPoints;
var
  i : Integer;
  bShouldDelete : boolean;
begin
  Align;
  if Fmetrics.Count<3 then
    Exit;
  i := 0;
  with FMetrics do
    while i<Count-2 do begin
      bShouldDelete := false;
      if (Points[i].X = Points[i+1].X) and (Points[i+2].X = Points[i].X) then
        bShouldDelete := true
      else if (Points[i].y = Points[i+1].y) and (Points[i+2].y = Points[i].y) then
        bShouldDelete := true;
      if bShouldDelete then
        RemovePoint(i+1)
      else
        Inc(i);
    end;
end;

procedure TLink.DebugDump;
var
  i : Integer;
  s : string;
begin
  if not GetDebug then
    Exit;
  s := 'Link object ' + F8UuidToString(Flink.uuid);
  with FMetrics do
    for i:=0 to Count-1 do
      s := s + '(' + IntToStr(Points[i].X) + ',' + IntToStr(Points[i].Y) + ')';
  Writeln(s);
end;

procedure TLink.AutoWire;
var
  ps, pt : TPin;
begin
  // auto wiring
  FMetrics.Count := 4;

  ps := Layout.GetPin(Flink.s);
  pt := Layout.GetPin(Flink.t);
  assert(assigned(ps) and assigned(pt));
  Left := 0;
  Width := 0;
  Top := 0;
  Height := 0;
  FMetrics.Points[0] := ps.pinTip;
  FMetrics.Points[3] := pt.pinTip;
  FMetrics.Points[1].X := (ps.pinTip.X + pt.pinTip.X) div 2;
  FMetrics.Points[1].Y := ps.pinTip.Y;
  FMetrics.Points[2].X := (ps.pinTip.X + pt.pinTip.X) div 2;
  FMetrics.Points[2].Y := pt.pinTip.Y;
  CompactPoints;
end;

function TLink.Source: TPin;
var
  blk : TBlk;
begin
  blk := (Layout).ItemByPointer(Flink.s.blk) as TBlk;
  assert(assigned(blk));
  Result := blk.GetPin(Flink.s);
end;

function TLink.Target: TPin;
var
  blk : TBlk;
begin
  blk := (Layout).ItemByPointer(Flink.t.blk) as TBlk;
  assert(assigned(blk));
  Result := blk.GetPin(Flink.t);
end;

procedure TLink.Align;
var
  i : Integer;
begin
  with Fmetrics do
    for i:=0 to Count-1 do begin
      Layout.Normalize(Points[i]);
    end;
end;

procedure TLink.UiInvalidateChanged;
var
  rp : Pblk_pin_t;
begin
  if not Layout.Online then
    Exit;
  rp := IBlk_real_pin(Flink.s);
  if rp._type<>PIN_T_BOOL then
    Exit;
  if rp.dp.d.i8 <> FLastValue.d.i8 then
    UiInvalidate;
  FLastValue := rp.dp^;
end;

function TPin.HitTest(x, y: Integer): TUiItem;
var
  p : TPoint;
  r : TRect;
begin
  p.x := x;
  p.y := y;
  Result := nil;
  r := BoundingRect;
  // InflateRect(r, Layout.TEXT_SHIFT, -Height*2);
  if PointInRect(p, r) then begin
    DragCursor := crCross;
    Result := Self;
  end;
end;

procedure TPin.DebugDump;
var
  s : string;
begin
  s := Fpathname;
  if GetDebug then
    Writeln('PinObject ', s, ',@(',Left,',',Top,'),Width=',Width,',Height=',Height);
end;

destructor TBlk.Destroy;
begin
  eis.Free;
  eos.free;
  dis.free;
  dos.free;
  inherited;
end;

destructor TLink.Destroy;
begin
  inherited;
end;

function DefaultCanvas:TRect;
var
  r : TRect;
begin
  r.left := 0;
  r.top := 0;
  r.Right := default_canvas_width;
  r.bottom := -default_canvas_height;
  result := r;
end;

function TBlkLayout.GetCanvas:TCanvas;
begin
  if FIsPrinting then
    result := printer.Canvas
  else
    Result := vw.Canvas;
end;

function TBlkLayout.GetOnline:boolean;
begin
  Result := FbOnline;
end;

{
  called each time scale factor changes
}
procedure TBlkLayout.PrepareCanvas(bZoom : boolean);
var
  s : TSize;
  Canvas : TCanvas;
begin
  // prepare canvas
  Canvas := GetCanvas;
  Canvas.Handle;
  Canvas.Lock;

  SetMapMode(Canvas.Handle, MM_ANISOTROPIC);

  s.cx := GetDeviceCaps(Canvas.Handle, LOGPIXELSX);
  s.cy := GetDeviceCaps(Canvas.Handle, LOGPIXELSY);
  SetViewportExtEx(Canvas.Handle, s.cx, -s.cy, nil);
  SetWindowExtEx(Canvas.Handle, 1440, 1440, @s);
  // make modifications on MFTWIPS to get zoom effects
  s.cx := 1440;
  s.cy := 1440;

  if(bZoom) then begin
    s.cx := Round(s.cx / FScale);
    s.cy := Round(s.cy / FScale);
    SetWindowExtEx(Canvas.Handle, s.cx, s.cy, @s);
  end;

  Canvas.Font.PixelsPerInch := s.cx;
  Canvas.Font.Size := -9;
end;

destructor TBlkLayout.Destroy;
begin
  getCanvas.Unlock;
  ke_uninit_hash(FblkHash);
  inherited;
end;

procedure TBlkLayout.vwOnPaint(Sender : TObject);
begin
  render;
end;

procedure InitOffsets;
var
  b : TUiItem;
  c : TPin;
begin
  b := nil;
  c := nil;
  g_UiItemLinkOffset := Integer(@b.link);
  g_UiSelectedItemLinkOffset := Integer(@b.selLink);
  g_Pin_BlkPinLinkOffset := Integer(@c.m_BlkPinLink);
end;

function ContainingUiItem(lk : PLIST_ENTRY):TUiItem;
begin
  Result := TUiItem(Integer(lk) - g_UiItemLinkOffset);
end;

function ContainingSelectedUiItem(lk : PLIST_ENTRY):TUiItem;
begin
  Result := TUiItem(Integer(lk) - g_UiSelectedItemLinkOffset);
end;

procedure TBlkLayout.DropItem(it: TUiItem);
begin
  if FHilighted=it then
    FHilighted := nil;
  it.uiOnDrop;
  RemoveEntryList(@it.link);
  it.Selected := false;
  dec(FItemCount);
  assert(fitemcount>=0);
end;

function TBlkLayout.GetItem(idx: Integer): TUiitem;
var
  lk : plist_entry;
begin
  Result := nil;
  if idx>=ItemCount then
    Exit;
  lk := FItems.flink;
  while idx>0 do begin
    lk := lk.Flink;
    assert(lk <> @fitems);
    Dec(idx);
  end;
  Result := containinguiitem(lk);
end;

function TBlkLayout.GetItemCount: Integer;
begin
  result := FItemCount;
end;

procedure TBlkLayout.AddItem(ui: TUiItem; k : Pointer);
var
  hk : hash_key_t;
begin
  ui.Layout := Self;
  InsertTailList(@FItems, @ui.link);
  ui.UiInvalidate;
  hk := integer(k);
  ke_set_hash(Fblkhash, @hk, hash_val_t(ui));
  Inc(FItemCount);
end;

procedure TBlkLayout.ReleaseCanvas;
begin
  Canvas.Unlock;
end;

procedure TBlkLayout.vwMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  ob : TUiItem;
  blk: TBlk;
begin
  //
  SetFocus;
{$IFDEF MOUSE_DEBUG}
  if getdebug then
    writeln('MouseDown');
{$ENDIF}
  PrepareCanvas;

  if not FbMouseDblClickCaptured then
    FbMouseDownCaptured := true
  else
    Fbmousedowncaptured := false;

  D2L(canvas, x, y);

  ob := ItemFromPoint(X, Y);
  if not assigned(ob) then begin
    Fhilighted := nil;
    FDragTask := dgRect;
    DeselectAll;
  end else begin
    FDragTask := dgMove;
    if (ssCtrl in Shift) then begin
      // toggle selection
      ob.Selected := not ob.Selected;
    end else if (ssShift in Shift) then begin
      ob.Selected := true;
    end else begin
      if not ob.Selected then
        DeselectAll;
      ob.Selected := true;
    end;

    // only blocks can be multiply selected
    if ob is TLink then
      FDragTask := dgReshapeLink
    else if ob is TPin then begin
      ob.Selected := true;
      FDragtask := dgLink;
    end else if (ob is TBlk) and (TBlk(ob).isExtensibleBlock) then begin
      blk := Tblk(ob);
      blk.hitTest(x,y);
      if blk.dragCursor=crsizeNS then begin
        // FDragTask := dgReshapeLogic;
      end;
    end;

    PutOnTop(ob);
  end;

  FHilighted := ob;

  if Assigned(ob) then
    ob.DebugDump;

  ReleaseCanvas;
end;

procedure TBlkLayout.beginInternalDrag(X, Y:Integer);
var
  it : TUiItem;
begin
  //
  PrepareCanvas;
{$IFDEF MOUSE_DEBUG}
  if GetDebug then
    Writeln('BeginDrag');
{$ENDIF}
  if assigned(FHilighted) then
    dragger.DragCursor := FHilighted.DragCursor
  else
    dragger.DragCursor := crDefault;

  dragger.Left := X;
  dragger.Top := Y;
  FDragLastPos.x := X;
  FDragLastPos.y := Y;

  if assigned(FHilighted) then begin
    FDragLastPos.X := x;
    FDragLastPos.Y := y;
    FDragStart := FDragLastPos;

    assert(assigned(FHilighted));
    if FHilighted is TLink then
      FHilighted.uiOnStartDrag(x,y)
    else if FHilighted is TPin then
      FHilighted.uiOnStartDrag(x,y)
    else begin
      it := firstItem;
      while assigned(it) do begin
        if it.Selected then
          it.uiOnStartDrag(x, y);
        it := nextItem(it);
      end;
    end;
  end;
  
  dragger.BeginDrag(false);

  ReleaseCanvas;
end;

procedure TBlkLayout.draggerEndDrag(Sender, Target: TObject; X, Y: Integer);
var
  r : TRect;
  Cancelled : boolean;
  it : TUiItem;
begin
  // assert(canvas.lockcount>1);
  Cancelled := not assigned(Target);

  case FDragTask of
  dgRect:
  begin
    Canvas.Pen.Mode := pmNotXor;
    Canvas.Pen.Style := psDash;
    Canvas.Brush.Style := bsClear;
    r.Left := dragger.Left;
    r.Top := dragger.Top;
    r.Right := FDragLastPos.x;
    r.Bottom := FDragLastPos.y;
    Canvas.Rectangle(r);
    r.Left := dragger.Left;
    r.Top := dragger.Top;
    r.Right := FDragLastPos.x;
    r.Bottom := FDragLastPos.y;
    Canvas.Pen.Mode := pmCopy;
    Canvas.Pen.Style := psSolid;
    Select(r);
  end;
  else
  begin
    Canvas.Pen.Mode := pmCopy;
    assert(assigned(FHilighted));
    if FHilighted is TLink then
      FHilighted.uiOnEndDrag(Cancelled)
    else if FHilighted is tpin then
      FHilighted.uiOnEndDrag(Cancelled)
    else begin
      // tcanvas.refresh
      it := getfirstselection;
      while assigned(it) do begin
        if (it is TBlk) then
          it.uiOnEndDrag(Cancelled);
        it := getNextSelection(it);
      end;
    end;
    g_bModified := true;
  end;
  end;
  ReleaseCanvas;
  // pnlBiu.SetFocus;
end;

procedure TBlkLayout.draggerStartDrag(Sender: TObject;
  var DragObject: TDragObject);
begin
  PrepareCanvas;
end;

procedure TBlkLayout.vwMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  it : TUiItem;
begin
  //is clicked on an object?
  FbMouseDownCaptured := false;
  
  PrepareCanvas;
  D2L(Canvas, X, Y);
  it := ItemFromPoint(x, y);
  ReleaseCanvas;
end;

procedure TBlkLayout.vwDragDrop(Sender, Source: TObject; X, Y: Integer);
var
  s : Tnewblock;
begin
  {note if source is not Tnewblock then
  the handler code is in draggerEndDrag}
  if source is Tnewblock then begin
    s := source as tnewblock;
    s.pt.x := x;
    s.pt.y := y;
    d2l(canvas, s.pt.x, s.pt.y);
    Normalize(s.pt);
    if assigned(fonaddblock) then
      fonaddblock(self, evtAddBlock, s, nil);
  end;
end;

procedure TBlkLayout.vwDragOver(Sender, Source: TObject; X, Y: Integer;
  State: TDragState; var Accept: Boolean);
var
  b : TNewBlock;
begin
  //
  Accept := false;
  if (Source = dragger) then begin
    {user is dragging contained blocks}
    Accept := true;
    D2L(Canvas, X, Y);
    InternalDragging(X, Y);
  end else if source is TNewBlock then begin
    {user is dragging blocks from the block library onto
    this layout}
    b := source as TNewBlock;
    if(compareStr(lowerCase(b.Blktype), 'base.blank') = 0) then
      Accept := false
    else
      Accept := true;
  end;
end;

procedure TBlkLayout.vwMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  it : TUiItem;
begin
  // show coordinates
  PrepareCanvas;

  D2L(Canvas, X, Y);
  it := ItemFromPoint(x, y);
  if FbMouseDownCaptured then begin
    if not readonly or (FDragTask = dgRect) then
      BeginInternalDrag(X, Y);
  end else begin
    pnlBiu.Cursor := crDefault;
    if assigned(it) then
      pnlBiu.Cursor := it.DragCursor;
  end;

  if assigned(FOnHint) then
    if assigned(it) then
      FOnHint(it.hintMessage)
    else
      FOnHint('');

  ReleaseCanvas;

  FbMouseDownCaptured := false;
  FbMouseDblClickCaptured := false;
end;

procedure TBlkLayout.MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
var
  s : real;
begin
  if ssCtrl in Shift then begin
    // zoom
    s := FScale + WheelDelta/1000;
    if s < 0.5 then
      s := 0.5;
    Scale := s;
    vw.Invalidate;
  end else
    VertScrollBar.Position := VertScrollBar.Position -
      WheelDelta div WHEEL_DELTA * 5;
end;

procedure BlkUnimplemented(h : hwnd);
begin
  MessageBox(h, 'Unimplemented', 'error', mb_ok);
end;

procedure TBlkLayout.vwDblClick(Sender: tobject);
var
  it : TUiItem;
begin
{$IFDEF MOUSE_DEBUG}
  if GetDebug then
    Writeln('DoubleClick');
{$ENDIF}
  FbMouseDownCaptured := false;

  it := Hilighted;
  if not assigned(it) then
    exit;

  FbMouseDblClickCaptured := true;
  if it is tblk then begin
    if tblk(it).blk.h.magic = BBLK_MAGIC THEN begin
      if assigned(FOneditblock) then foneditblock(self,evteditblock,it,nil);
    end else begin
      Bind(tblk(it).blk);
    end;
  end else if (it is TPin) then begin
    if assigned(FOneditpin) then foneditpin(self,evteditpin,it,nil);
  end;
end;

procedure TPin.UiInvalidate;
begin
  inherited;
  UiInvalidateText(text2tip, last_value_str, not FbIsOutput);
  if assigned(ep) then
    ep.UiInvalidate;
end;

procedure TUiItem.UiInvalidateText(p : tpoint; s : string; rightAligned : boolean);
var
  r : trect;
begin
  r := uitextrect(p, s, rightaligned);
  InflateRect(r, Layout.TEXT_SHIFT*2, -Layout.TEXT_SHIFT*2);
  UiInvalidateRect(r);
end;

procedure CanvasRTextout(Canvas : TCanvas; p : tpoint; s : string);
var
  sz : Tsize;
begin
  sz := canvas.TextExtent(s);
  canvas.TextOut(p.x - sz.cx, p.y, s);
end;

function TPin.ValueStr: string;
begin
  result := '';
  if (rp._class=pin_class_ei) or (rp._class=pin_class_eo) then
    exit;
  result := valueByPin(rp);
  if (rp._class=pin_class_di) then begin
    if rp.u1.link.Flink<>@rp.u1.link then
      result := ''
    else if assigned(query_var_by_pin(p)) then
      result := GetBindingStr(rp)
    else if assigned(ep) then
      result := '';
  end else begin
    // pin_class_do
    if assigned(query_var_by_pin(p)) then begin
      if isOnline then
        result := GetBindingStr(rp) + '(' + result + ')'
      else
        result := getBindingStr(rp);
    end else if not isOnline then
      result := '';
  end;
end;

function TPin.Text2Tip: TPoint;
var
  blk : TBLk;
begin
  blk := OwnerBlk;
  Result.X := blk.Left + self.FMetrics.tep2.x;
  Result.y := blk.Top + self.FMetrics.tep2.y;
end;

procedure TBlk.UiInvalidate;
var
  pin : TPin;
  lk : PList_entry;
begin
  inherited;
  lk := m_Pins.flink;
  try
    // the pins might have already been deleted.
    while lk<>@m_Pins do begin
      pin := ContainingBlkPin(lk);
      pin.UiInvalidate;
      lk := lk.Flink;
    end;
  except
    //
  end;
end;

procedure TBlk.UiInvalidateChanged;
var
  pin : TPin;
  lk : PList_entry;
begin
  inherited;
  lk := m_Pins.Flink;
  while lk<>@m_Pins do begin
    pin := ContainingBlkPin(lk);
    pin.UiInvalidateChanged;
    lk := lk.Flink;
  end;
end;

procedure TPin.UiInvalidateChanged;
var
  s : string;
  value_changed : boolean;
  r : TRect;
begin
  inherited;
  s := valuestr;
  if p^._type=pin_t_bool then
    value_changed := last_value.d.u8 <> rp^.dp.d.u8
  else
    value_changed := s <> last_value_str;
  if value_changed then begin
    if p^._type=pin_t_bool then begin
      r := barRect;
      inflateRect(r, 0, -Height div 2);
      UiInvalidateRect(r);
    end;
    UiInvalidateText(text2tip, last_value_str, not FbIsOutput);
    UiInvalidateText(text2tip, valueStr, not FbIsOutput);
    if assigned(ep) then
      ep.UiInvalidateChanged;
  end;
end;

procedure TBlkLayout.setScale(scale : real);
var
  s : TSize;
begin
  if scale < 0 then
    Exit;
  Fscale := scale;

  PrepareCanvas;

  s := Canvas.TextExtent('ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrstuvwxyz_1234567890');
  // FiGridUnit := s.cy div 2 + D2L(Canvas,1);
  FiGridUnit := 112;
  TEXT_SHIFT := D2L(Canvas, 1);

  H_MARGIN := FiGridUnit;
  V_MARGIN := FiGridUnit;

  FDotSize := D2L(Canvas, 3);
  FNotSize := D2L(Canvas, 3);
  FPinHeight := D2L(Canvas, 3);

  pnlBiu.Width := L2D(Canvas,default_canvas_width);
  pnlBiu.height := abs(L2d(canvas,default_canvas_height));

  ReleaseCanvas;
end;

function TBlkLayout.FirstItem: TUiItem;
begin
  result := nil;
  if islistempty(@fitems) then
    exit;
  result := containinguiitem(fitems.Flink);
end;

function TBlkLayout.LastItem: TUiItem;
begin
  result := nil;
  if islistempty(@fitems) then
    exit;
  result := containinguiitem(fitems.blink);
end;

function TBlkLayout.NextItem(it : TUiItem): TUiItem;
begin
  result := nil;
  if it.link.Flink = @fitems then
    exit;
  result := containinguiitem(it.link.flink);
end;

function TBlkLayout.PreviousItem(it : TUiItem): TUiItem;
begin
  result := nil;
  if it.link.blink = @fitems then
    exit;
  result := containinguiitem(it.link.blink);
end;

function TBlkLayout.GetInsertionPoint: TPoint;
begin
  Inc(FInsertionPoint.X, FiGridUnit*2);
  Dec(FinsertionPoint.y, FiGridUnit*2);
  Result := finsertionpoint;
end;

procedure TBlkLayout.PutOnTop(it: TUiItem);
begin
  // move it to end of item list(assigns a highest Z-order)
  removeentrylist(@it.link);
  inserttaillist(@self.FItems, @it.link);
end;

function TBlkLayout.GetStarted: boolean;
begin
  Result := false;
end;

function TBlkLayout.GetReadonly: boolean;
begin
  result := (blk.h.flags and blk_f_readonly)<>0;
end;

function TBlkLayout.GetAllBlocks: TList;
var
  l : TList;
  it : TUiItem;
begin
  l := tlist.Create;
  it := GetFirstSelection;
  while assigned(it) do begin
    if it is Tblk then
      l.Insert(0, it);
    it := GetNextSelection(it);
  end;
  result := l;
end;

procedure TBlkLayout.AddToSelection(it: TUiItem);
begin
  assert(IsListEmpty(@it.selLink));
  InsertTailList(@selLink, @it.selLink);
  Inc(FSelCount);
  if it is TBlk then
    Inc(FSelectedBlocks)
  else if it is TPin then
    Inc(FSelectedPins)
  else if it is TLink then
    Inc(FSelectedLinks);
end;

procedure TBlkLayout.RemoveFromSelection(it: TUiItem);
begin
  assert(not IsListEmpty(@it.selLink));
  RemoveEntryList(@it.selLink);
  Dec(FSelCount);
  if it is TBlk then
    Dec(FSelectedBlocks)
  else if it is TPin then
    dec(FSelectedPins)
  else if it is TLink then
    dec(FSelectedLinks);
end;

function TBlkLayout.GetFirstSelection: TUiItem;
begin
  Result := nil;
  if IsListEmpty(@selLink) then
    Exit;
  Result := ContainingSelectedUiItem(selLink.Flink);
end;

function TBlkLayout.GetNextSelection(it: TUiItem): TUiItem;
begin
  assert(not IsListEmpty(@it.selLink));
  Result := nil;
  if it.selLink.Flink=@selLink then
    Exit;
  Result := ContainingSelectedUiItem(it.selLink.Flink);
end;

function TBlkLayout.ItemByPointer(p : Pointer): TUiItem;
var
  hk : hash_key_t;
begin
  hk := Integer(p);
  result := TUiItem(ke_get_hash(FblkHash, @hk));
end;

procedure TBlkLayout.UiInvalidate;
begin
  InvalidateRect(pnlBiu.Handle, nil, false);
end;

procedure TBlkLayout.makeVisible(pt: TPoint);
begin
  //
  self.L2D(canvas, pt.x, pt.y);
  if self.Width < pnlbiu.Width then begin
    if (self.Width + self.HorzScrollBar.Position < pt.X)
    or
      (self.HorzScrollBar.Position > pt.X)
    then
      self.HorzScrollBar.Position := pt.x - self.width div 2;
  end;
  if self.Height < pnlbiu.Height then begin
    if (self.Height + self.VertScrollBar.Position < pt.Y)
    or
      (self.VertScrollBar.Position > pt.Y)
    then
      self.VertScrollBar.Position := pt.y - self.Height div 2;
  end;
end;

procedure TBlkLayout.D2L(canvas: TCanvas; var p: TPoint);
begin
  DPtoLP(Canvas.Handle, p, 1);
end;

procedure TBlkLayout.L2D(canvas: TCanvas; var p: TPoint);
begin
  LPtoDP(Canvas.Handle, p, 1);
end;

procedure TBlkLayout.print;
begin
  //
  FIsPrinting := true;
  printer.BeginDoc;
  PrepareCanvas;
  render;
  ReleaseCanvas;
  printer.EndDoc;
  FIsPrinting := false;
end;

{ TExtPin }

procedure TExtPin.Bind(o: TPin);
begin
  op := o;
  rp := op.rp;
  p := op.p.ulink;
  assert(assigned(p));
  s := Iblk_pin_name(p);
  op.Associate(self);
  Fpathname := getpinpath(p);
end;

constructor TExtPin.Create;
begin
  inherited;
end;

procedure TExtPin.DebugDump;
begin
  inherited;
  if getdebug then begin
    writeln('extpin object ''', Fpathname, '''');
    writeln('ResCount=', query_resource(@pin2cpin(p).uuid));
  end;
end;

function TExtPin.getPointer: Pointer;
begin
  result := p;
end;

function TExtPin.HitTest(x, y: Integer): TUiItem;
var
  p : tpoint;
  r : trect;
begin
  result := nil;
  p.x := x;
  p.y := y;
  r := boundingrect;
  if pointinrect(p, r) then begin
    result := self;
    exit;
  end;
  r := uitextrect(self.tep, valuestr, not op.FbIsOutput);
  if pointinrect(p, r) then begin
    result := self;
  end;
end;

procedure TExtPin.render;
var
  cl : tcolor;
begin
  inherited;

  pushcanvasstate(canvas);

  if selected then begin
    canvas.Pen.Color := clblue;
    canvas.pen.Width := layout.D2L(canvas, 2);
  end;

  canvas.MoveTo(op.pinTip.X, op.pinTip.Y);
  canvas.lineto(tip.X, tip.y);

  canvas.moveto(left, top);
  canvas.lineTo(left + width, tip.y);
  canvas.lineto(left, top-height);
  canvas.lineTo(left, top);

  if layout.Online and (rp._type=pin_t_bool) then begin
    if rp.dp.d.i8<>0 then
      cl := clgreen
    else
      cl := clred;
    // canvas.FloodFill((tip.X + tep.X) div 2, tip.Y, cl, fsSurface);
    canvas.Brush.Color := cl;
  end;
  canvas.Polygon(
    [
      point(left,top),
      point(left + width, tip.y),
      point(left, top - height)
    ]
    );
  canvas.Brush.Color := layout.Color;

  popcanvasstate(canvas);
end;

procedure TExtPin.renderText;
begin
  pushcanvasstate(canvas);

  canvas.Brush.Color := layout.Color;

  if op.FbIsOutput then
    canvas.textOut(tep.x, tep.y, valueStr)
  else
    canvasrtextout(canvas, tep, valueStr);

  popcanvasstate(canvas);
end;

procedure TExtPin.UiInvalidate;
begin
  inherited;
  uiinvalidatetext(tep, valueStr, not op.FbIsOutput);  
end;

procedure TUiItem.uiOnDrop;
var
  ui : TUiitem;
begin
  if (getdebug and false)then
    Writeln('Dropping ', classname, ':', Fpathname);
  // do nothing
  // UiInvalidate;

  // remove objects whose existence depend on current object
  while Fassociated.Count>0 do begin
    ui := tuiitem(Fassociated.Items[0]);
    // if ui.owner = layout then
      layout.DropItem(ui);
    {
    else begin
      ui.uiOnDrop;
      ui.Owner.RemoveComponent(ui);
    end;
    }
    // because ui itself will make de-association, it will alreay kicked
    // off the association queue, so there's no need to call Deassociate()
    // again
    assert(Fassociated.indexof(ui)<0);
    // Deassociate(ui);
  end;

  // notify objects on which current object depends
  while Fassociatedmaster.Count>0 do begin
    ui := tuiitem(Fassociatedmaster.Items[0]);
    ui.Deassociate(self);
  end;
end;

procedure TUiItem.Associate(ui: TUiItem);
begin
  if FAssociated.IndexOf(ui)>=0 then
    exit;
  Fassociated.Add(ui);
  ui.FAssociatedMaster.Add(self);
end;

procedure TUiItem.Deassociate(ui: TUiItem);
begin
  Fassociated.Remove(ui);
  assert(ui.FAssociatedMaster.indexof(self)>=0);
  ui.FAssociatedMaster.Remove(self);
end;

destructor TUiItem.Destroy;
begin
  FAssociated.free;
  Fassociatedmaster.free;
  inherited;
end;

function TUiItem.UiTextRect(p: tpoint; s: string;
  rightAligned: boolean): TRect;
var
  sz : tsize;
  r : trect;
begin
  sz := canvas.textextent(s);
  r.top := p.y;
  r.Bottom := p.y - sz.cy;
  if rightAligned then begin
    r.Left := p.X - sz.cx;
    r.Right := p.X;
  end else begin
    r.Left := p.x;
    r.Right := p.x + sz.cx;
  end;
  result := r;
end;

function TPin.BoundingRect: TRect;
var
  r1,r2:TRect;
begin
  r1 := uitextrect(texttip, FPinName, FbIsoutput);
  r2 := uitextrect(text2tip, last_value_str, not Fbisoutput);
  uiunionrect(result, r1, r2);
  inflaterect(result, layout.d2l(canvas,1),-layout.d2l(canvas,1));
end;

procedure TPin.uiOnDragging(x, y: Integer);
var
  pr : TUiItem;
  prp : TPin;
begin
  // inherited;
  // draw a rubber line
  pushcanvasstate(canvas);
  
  canvas.Pen.Mode := pmnotxor;
  canvas.Pen.Color := clBlue;
  // canvas.Pen.Style := psDash;
  
  if not FbStartdrag then begin
    canvas.MoveTo(pinTip.X, pinTip.Y);
    canvas.LineTo(FDragLastPos.x, pinTip.Y);
    canvas.LineTo(FDragLastPos.x, FDragLastPos.y);
  end;

  FDragLastPos.X := x;
  FDragLastPos.y := y;
  canvas.MoveTo(pinTip.X, pinTip.Y);
  canvas.LineTo(FDragLastPos.x, pinTip.Y);
  canvas.LineTo(FDragLastPos.x, FDragLastPos.y);

  pr := layout.ItemFromPoint(x, y);
  dragcursor := crCross;
  if assigned(pr) and (pr is TPin) then begin
    prp := pr as tpin;
    if Iblk_linkable(p, prp.p)<>0 then
      dragcursor := crDrag
    else
      dragcursor := crNo;
  end;
    
  Fbstartdrag := false;
  popcanvasstate(canvas);
end;

procedure TPin.uiOnEndDrag(Cancelled: Boolean);
var
  pr : TUiItem;
  prp : TPin;
begin
  // inherited;
  // clear last drawn link line
  pushcanvasstate(canvas);
  canvas.Pen.Mode := pmnotxor;
  canvas.Pen.Color := clBlue;
  // canvas.Pen.Style := psDash;
  if not FbStartdrag then begin
    canvas.MoveTo(pinTip.X, pinTip.Y);
    canvas.LineTo(FDragLastPos.x, pinTip.Y);
    canvas.LineTo(FDragLastPos.x, FDragLastPos.y);
  end;
  popcanvasstate(canvas);

  if Cancelled then
    exit;
  pr := layout.ItemFromPoint(FDragLastPos.x, FDragLastPos.y);
  if not assigned(pr) then
    exit;
  if not (pr is tpin) then
    exit;
  prp := pr as tpin;

  if not (Iblk_linkable(p, prp.p)<>0) then
    exit;

  if assigned(layout.onaddlink) then
    layout.OnAddLink(layout, evtAddLink, self, prp);
end;

procedure TPin.uiOnStartDrag(x, y: Integer);
begin
  inherited;
  dragcursor := crCross;
end;

procedure TBlk.loadMetrics;
var
  i : integer;
  Size : TSize;
  p : Pblk_pin_t;
begin
  ZeroMemory(@FMetrics, sizeof(FMetrics));
  FMetrics.h0 := Layout.FiGridUnit*2;
  FMetrics.h1 := FMetrics.h0*max(eis.Count, eos.Count);

  {2005/6/21, 61131 blocks are now displayed without the cap}
{
  if FMetrics.h1=0 then
    FMetrics.h1 := Layout.FiGridUnit;
}
  for i:=0 to eis.Count -1 do begin
    p := Pblk_pin_t(eis[i]);
    size := Canvas.TextExtent(IBlk_pin_name(p));
    if FMetrics.w1 < size.cx then
      FMetrics.w1 := size.cx;
  end;

  for i:=0 to eos.Count -1 do  begin
    p := Pblk_pin_t(eos[i]);
    size := Canvas.TextExtent(IBlk_pin_name(p));
    if FMetrics.w2 < size.cx then
      FMetrics.w2 := size.cx;
  end;

  if FMetrics.h1<>0 then
    FMetrics.h2 := Layout.FiGridUnit;

  for i:=0 to dis.Count - 1 do  begin
    p := Pblk_pin_t(dis[i]);
    size := Canvas.TextExtent(IBlk_pin_name(p));
    if FMetrics.w1 < size.cx then
      FMetrics.w1 := size.cx;
  end;

  for i:=0 to dos.Count - 1 do  begin
    p := Pblk_pin_t(dos[i]);
    size := Canvas.TextExtent(IBlk_pin_name(p));
    if FMetrics.w2 < size.cx then
      FMetrics.w2 := size.cx;
  end;

  FMetrics.w2 := max(FMetrics.w1, FMetrics.w2);
  FMetrics.w1 := FMetrics.w2;
  FMetrics.w := FMetrics.w1 + FMetrics.w2 + Layout.FiGridUnit;

  size := Canvas.TextExtent(IBlk_class_name(blk));
  FMetrics.w := max(FMetrics.w, size.cx + FMetrics.h0);
{
  size := Canvas.TextExtent(IBlk_name(blk));
  FMetrics.w := max(FMetrics.w, size.cx + FMetrics.h0);
}
  FMetrics.h3 := FMetrics.h0*(max(dis.Count, dos.Count)+1);
  FMetrics.h := FMetrics.h0 + FMetrics.h1 + FMetrics.h2 + FMetrics.h3;
{
  if FMetrics.w < (3*FMetrics.h div 4) then
    FMetrics.w := (3*FMetrics.h div 4);
}    
  if FMetrics.w < (5*Layout.FiGridUnit) then
    FMetrics.w := 5*Layout.FiGridUnit;

  FMetrics.w := Layout.Normalize(FMetrics.w);
  FMetrics.h := Layout.Normalize(FMetrics.h);

  Width := FMetrics.w + 2*FMetrics.h0;
  Height := FMetrics.h;

  // draw bounding rect
  with Fmetrics do begin
    pt[0].X := h0;
    pt[0].Y := - h0;

    pt[1].X := w;
    pt[1].y := 0;

    pt[2].X := 0;
    pt[2].Y := -h1;

    pt[3].x := -layout.Figridunit;
    pt[3].Y := 0;

    pt[4].x := 0;
    pt[4].y := -h2;

    pt[5].x := layout.Figridunit;
    pt[5].y := 0;

    pt[6].x := 0;
    pt[6].y := -h3;

    pt[7].X := -w;
    pt[7].Y := 0;

    pt[8].x := 0;
    pt[8].y := h3;

    pt[9].x := layout.FiGridUnit;
    pt[9].y := 0;

    pt[10].X := 0;
    pt[10].Y := h2;

    pt[11].x := -layout.Figridunit;
    pt[11].y := 0;

    for i:=1 to 11 do begin
      pt[i].X := pt[i-1].x + pt[i].x;
      pt[i].y := pt[i-1].y + pt[i].y;
    end;
  end;
end;

procedure TUiItem.uiOnResize;
begin
  //
end;

procedure TExtPin.uiOnDrop;
begin
  inherited;
  op.ep := nil;
end;

function TExtPin.ValueStr: string;
var
  vstr, cap : string;
begin
  vstr := op.valuestr;
  if vstr<>'' then
    cap := s + '(' + vstr + ')'
  else
    cap := s;
  result := cap;
end;

function ContainingBlkPin(lk : PLIST_ENTRY):TPin;
begin
  Result := TPin(Integer(lk) - g_Pin_BlkPinLinkOffset);
end;

function TBlk.getPointer: Pointer;
begin
  Result := blk;
end;

function TLink.getPointer: Pointer;
begin
  result := FLink; 
end;

function TPin.getPointer: Pointer;
begin
  result := p;
end;

procedure TBlk.renderText;
var
  xx, yy : Integer;
  s : TSize;
  tp : TPin;
  lk : Plist_entry;
  cn : pchar;
begin
  inherited;
  // class name
  cn := get_res_name(@blk^.h.uuid);
  if assigned(cn) then begin
    Canvas.Font.Style := Canvas.Font.Style + [fsBold];
    s := Canvas.TextExtent(cn);
    xx := Left + FMetrics.h0 + (FMetrics.w - s.cx) div 2;
    yy := Top - FMetrics.h0 - FMetrics.h1 - FMetrics.h2;
    canvas.Brush.Style := bsClear;
    Canvas.TextOut(xx + Layout.TEXT_SHIFT, yy - Layout.TEXT_SHIFT, cn);
    Canvas.Font.Style := Canvas.Font.Style - [fsBold];
  end;

  // caption
  canvas.Brush.color := layout.color;
  Canvas.Textout(
    Left + FMetrics.h0 + Layout.TEXT_SHIFT,
    Top - Layout.TEXT_SHIFT,
    IBlk_name(blk)
    );

  lk := m_Pins.flink;
  while lk<>@m_Pins do begin
    tp := containingBlkPin(lk);
    tp.renderText;
    lk := lk.flink;
  end;
end;

procedure TLink.renderText;
begin
  inherited;
end;

{
  有没有连接
  有没有变量
  有没有导出
  是否在线
  控制算法是否启动
  是输入变量还是输出变量
}
procedure TPin.renderText;
begin
  PushCanvasState(Canvas);

  Canvas.Brush.Color := OwnerBlk.Fillcolor;
  if FbIsoutput then
    CanvasRTextOut(Canvas, TextTip, FPinName)
  else
    Canvas.TextOut(TextTip.X, TextTip.y, FPinName);

  canvas.brush.color := layout.color;

  last_value_str := valueStr;
  if FbIsOutput then
    Canvas.TextOut(text2tip.x, text2tip.y, last_value_str)
  else
    CanvasRTextout(canvas, text2tip, last_value_str);

  canvas.brush.color := layout.color;
  if Selected then begin
    canvas.Pen.color := clBlue;
    canvas.Brush.Style := bsClear;
    canvas.Rectangle(boundingrect);
  end;

  PopCanvasState(Canvas);
end;

procedure TBlk.DebugDump;
begin
  inherited;
  if getdebug then
    Writeln(
      'InstResCount=',
      query_resource(@blk.uuid),
      ',ClassResCount=',
      query_resource(@blk.h.uuid)
      );
end;

function TPin.barRect: TRect;
var
  y : integer;
begin
  y := pinTip.y;
  result.left := left;
  result.right := result.Left + width;
  result.top := y + height div 2;
  result.Bottom := y - height div 2;
end;

procedure TUiItem.UiInvalidateRect(r : TRect);
begin
  layout.L2D(canvas, r);
  invalidaterect(canvashandle, @r, false);
end;

function TUiItem.hintMessage: string;
begin
  result := 'Item ' + className;
end;

function TPin.hintMessage: string;
var
  v : Pblk_var_t;
begin
  result := '引脚:' + FPinpath;
  result := result + ',类型:' + _typeName(self.rp._type);
  v := query_var_by_pin(self.rp);
  if assigned(v) then begin
    result := result + ',变量:'+v.name;
    if v.comment^<>chr(0) then
      result := result + '('+v.comment+')';
  end;
end;

function TBlk.hintMessage: string;
var
  cmt : Pchar;
begin
  result := '功能块:'+self.Fpathname;
  cmt := IBlk_get_data(blk,'comment',nil);
  if assigned(cmt) then
    result := result + ',' + cmt;
end;

function TBlk.getPath: string;
begin
  result := getBlkPath(self.blk);
end;

initialization
  g_CanvasStateStackPointer := CANVAS_STATE_STACK_DEPTH - 1;
  InitOffsets;

end.
