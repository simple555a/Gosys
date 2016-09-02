; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "evtlq.h"
LastPage=0

ClassCount=7
Class1=CChildView
Class2=CDlgQuery
Class3=CDlgQueryById
Class4=CDlgServerIP
Class5=CMcEvtQueryApp
Class6=CAboutDlg
Class7=CMainFrame

ResourceCount=5
Resource1=IDD_QUERY_BY_ID
Resource2=IDD_QUERY
Resource3=IDD_ABOUTBOX
Resource4=IDR_MAINFRAME
Resource5=IDD_IP

[CLS:CChildView]
Type=0
BaseClass=CWnd
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
LastObject=CChildView

[CLS:CDlgQuery]
Type=0
BaseClass=CDialog
HeaderFile=DlgQuery.h
ImplementationFile=DlgQuery.cpp

[CLS:CDlgQueryById]
Type=0
BaseClass=CDialog
HeaderFile=DlgQueryById.h
ImplementationFile=DlgQueryById.cpp

[CLS:CDlgServerIP]
Type=0
BaseClass=CDialog
HeaderFile=DlgServerIP.h
ImplementationFile=DlgServerIP.cpp

[CLS:CMcEvtQueryApp]
Type=0
BaseClass=CWinApp
HeaderFile=evtlq.h
ImplementationFile=evtlq.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=evtlq.cpp
ImplementationFile=evtlq.cpp
LastObject=CAboutDlg

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_QUERY]
Type=1
Class=CDlgQuery
ControlCount=12
Control1=IDC_STATIC,button,1342177287
Control2=IDC_DATE_STA,SysDateTimePick32,1342242852
Control3=IDC_TIME_STA,SysDateTimePick32,1342242857
Control4=IDC_STATIC,button,1342177287
Control5=IDC_DATE_END,SysDateTimePick32,1342242852
Control6=IDC_TIME_END,SysDateTimePick32,1342242857
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816
Control9=IDC_EVENT_CLASS,combobox,1344339971
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_FILTER,edit,1350631552

[DLG:IDD_QUERY_BY_ID]
Type=1
Class=CDlgQueryById
ControlCount=6
Control1=IDC_STATIC,static,1342308866
Control2=IDC_FIRSTPT,edit,1350631552
Control3=IDC_STATIC,static,1342308866
Control4=IDC_COUNT,edit,1350631552
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[DLG:IDD_IP]
Type=1
Class=CDlgServerIP
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SERVERIP,SysIPAddress32,1342242816
Control4=IDC_STATIC,static,1342308866
Control5=IDC_STATIC,static,1342308866
Control6=IDC_CURIP,static,1342308864

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_OPT_QUERY
Command2=ID_OPT_OUTFILE
Command3=ID_OPT_PRINT
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_APP_ABOUT
CommandCount=6

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_OPT_QUERY
Command2=ID_OPT_OUTFILE
Command3=ID_OPT_PRINT
Command4=ID_APP_ABOUT
CommandCount=4

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

