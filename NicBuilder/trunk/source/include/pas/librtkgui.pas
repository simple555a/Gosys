unit librtkgui;

interface
uses
  windows,
  messages;

const
{
	�ȴ��Ի������ʶ
	��ʾʲôԪ���ǿ����������õ�
}
WDS_CANCEL	  =  $00000001;
WDS_PAUSE	=	$00000002      ;
WDS_STATE   =    $00000004  ;
WDS_ANIMATE  =   $00000008   ;
WDS_TIMELEFT  =  $00000010    ;
WDS_TIMEREMAIN = $00000020     ;
WDS_PROGRESS  =  $00000040      ;


{�ȴ��Ի���ť��ʶ WaitDialogControl->WDC}
WDC_NONE   =     $00000000;	// �ް�ť����
WDC_CANCEL  =    $00000002 ; // ȡ����ť
WDC_PAUSE    =   $00000004  ;// ��ͣ��ť
WDC_RESUME	 =	$00000008	;// ������ť

{�ȴ��Ի��򷵻ص�֪ͨ WaitDialogNotify->WDN}
WDN_BTNCLICK   = (WM_USER + $00000001);    // ��ť������

{�ȴ��Ի���ص���������}
type

{
	RtkGuiAboutBox @1 NONAME
	RtkGuiWDGetHwnd @2 NONAME
	RtkGuiWDStart @3 NONAME
	RtkGuiWDSetPos @4 NONAME
	RtkGuiWDSetState @5 NONAME
	RtkGuiWDDestroy @6 NONAME
	RtkGuiWDSetCaption @7 NONAME
	RtkGuiWDSetBtnText @8 NONAME
	RtkGuiWDGetBtnState @9 NONAME
	RtkGuiWDEnableSkin @10 NONAME
	RtkGuiMsgBox @11 NONAME
	RtkGuiMsgBoxEx @12 NONAME
	RtkGuiGetFileVersion @13 NONAME
}
TWaitDlgProc = function (hWD : THandle;uMsg : dword;lParam : dword):integer;stdcall;

function RtkGuiWDGetHwnd(hWaitDlg : THandle):hWND;stdcall;external 'librtkgui.dll' index 2;
function RtkGuiWDStart(uType : dword; fnWaitDlg : TWaitDlgProc):THandle;stdcall;external 'librtkgui.dll' index 3;
function RtkGuiWDSetPos(hWaitDlg : THandle; nPos : dword):longbool;stdcall;external 'librtkgui.dll' index 4;
function RtkGuiWDSetState(hWaitDlg : THandle; lpszStateText : pchar):longbool;stdcall;external 'librtkgui.dll' index 5;
function RtkGuiWDDestroy(hWaitDlg : THandle):longbool;stdcall;external 'librtkgui.dll' index 6;
function RtkGuiWDSetCaption(hWaitDlg : THandle; lpszCaption : pchar):longbool;stdcall;external 'librtkgui.dll' index 7;
function RtkGuiWDSetBtnText(hWaitDlg : THandle; uBtnID : dword; lpszBtnText : pchar):longbool;stdcall;external 'librtkgui.dll' index 8;
function RtkGuiWDGetBtnState(hWaitDlg : THandle):dword;stdcall;external 'librtkgui.dll' index 9;
procedure RtkGuiWDEnableSkin(bEnable : longbool);stdcall;external 'librtkgui.dll' index 10;

implementation

end.
