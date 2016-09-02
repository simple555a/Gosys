unit common;

interface
uses
  librtkgui,
  sysutils,
  windows,
  forms,
  f8;
var
  g_AppPath : string;
  g_Projname : string;
  g_bUntitled : boolean;
  g_bModified : boolean;
  g_bCmdInProgress : boolean;
  g_progress : integer;

function isOnline:boolean;
function reConnect(reason : integer; target : string):f8_status;
FUNCTION KProxyShellWithError(h: hwnd; s : string; timeout : integer=2000):f8_status;

implementation

uses
  untProgress;

function isOnline : boolean;
begin
  result := (g_adapter^.flags and adapter_f_online)<>0;
end;

function reConnect(reason : integer; target : string):f8_status;
var
  ctx : THandle;
  i : Integer;
  hint : string;
begin
	ctx := RtkGuiWDStart(WDS_PROGRESS or WDS_STATE or WDS_CANCEL, nil);
  if reason = 1 then
	  RtkGuiWDSetCaption(ctx, '到目标系统的连接丢失，正在重新连接...')
  else
    RtkGuiWDSetCaption(ctx, '正在连接到目标系统...');
  for i:=0 to 4 do
  begin
    RtkGuiWDSetPos(ctx, i * 20);
    if (RtkGuiWDGetBtnState(ctx) and WDC_CANCEL)<>0 then
    begin
      result := f8_cancelled;
      break;
    end;
    hint := '第 ' + intToStr(i+1) + ' 次尝试, 共 5 次';
    RtkGuiWDSetState(ctx, pchar(hint));
    result := kproxy_shell_ex(pchar('online ' + target), 2000, 0);
    if result=0 then
      break;
  end;
  RtkGuiWDDestroy(ctx);
end;

function KProxyShellWithError(h : hwnd; s: string; timeout : integer):f8_status;
begin
  g_bModified := true;
  g_bCmdInProgress := true;
  g_progress := 0;
  Result := kproxy_shell(pchar(s));
  g_bCmdInProgress := false;
  if result = F8_SUCCESS then
    Exit;
  MessageBox(
    h,
    pchar(s + chr(13) + chr(10) + FormatF8Error(Result)),
    PCHAR(Application.title),
    MB_OK
    );
end;

end.
