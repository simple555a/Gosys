; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDiaSystem
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mcview.h"
LastPage=0

ClassCount=14
Class1=CDiaAlarmList
Class2=CDiaInputA
Class3=CDiaInputD
Class4=CDiaPassword
Class5=CDiaSystem
Class6=CDlgAbout
Class7=CDlgDnyTip
Class8=CDlgLoop
Class9=CDlgSelect
Class10=CDlgTrendComSav
Class11=CDlgTrendTime
Class12=CFView
Class13=CMcviewApp
Class14=CMcviewDlg

ResourceCount=15
Resource1=IDD_ABOUT
Resource2=IDD_LOOP
Resource3=IDD_POPUP
Resource4=IDD_MCVIEW_DIALOG_S
Resource5=IDD_INPUT_D
Resource6=IDD_PASSWORD
Resource7=IDD_TRENDCOMPS
Resource8=IDD_DNYTIP
Resource9=IDD_SYSTEM
Resource10=IDD_TREND_TIME
Resource11=IDD_FVIEW_DIALOG
Resource12=IDD_MCVIEW_DIALOG
Resource13=IDD_INPUT_A
Resource14=IDD_ALARMLIST
Resource15=IDD_SELECT

[CLS:CDiaAlarmList]
Type=0
BaseClass=CDialog
HeaderFile=DiaAlarmList.h
ImplementationFile=DiaAlarmList.cpp

[CLS:CDiaInputA]
Type=0
BaseClass=CDialog
HeaderFile=DiaInputA.h
ImplementationFile=DiaInputA.cpp
LastObject=CDiaInputA
Filter=D

[CLS:CDiaInputD]
Type=0
BaseClass=CDialog
HeaderFile=DiaInputD.h
ImplementationFile=DiaInputD.cpp

[CLS:CDiaPassword]
Type=0
BaseClass=CDialog
HeaderFile=DiaPassword.h
ImplementationFile=DiaPassword.cpp

[CLS:CDiaSystem]
Type=0
BaseClass=CDialog
HeaderFile=DiaSystem.h
ImplementationFile=DiaSystem.cpp
LastObject=CDiaSystem

[CLS:CDlgAbout]
Type=0
BaseClass=CDialog
HeaderFile=DlgAbout.h
ImplementationFile=DlgAbout.cpp

[CLS:CDlgDnyTip]
Type=0
BaseClass=CDialog
HeaderFile=DlgDnyTip.h
ImplementationFile=DlgDnyTip.cpp

[CLS:CDlgLoop]
Type=0
BaseClass=CDialog
HeaderFile=DlgLoop.h
ImplementationFile=DlgLoop.cpp

[CLS:CDlgSelect]
Type=0
BaseClass=CDialog
HeaderFile=DlgSelect.h
ImplementationFile=DlgSelect.cpp

[CLS:CDlgTrendComSav]
Type=0
BaseClass=CDialog
HeaderFile=DlgTrendComSav.h
ImplementationFile=DlgTrendComSav.cpp

[CLS:CDlgTrendTime]
Type=0
BaseClass=CDialog
HeaderFile=DlgTrendTime.h
ImplementationFile=DlgTrendTime.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_MIN5

[CLS:CFView]
Type=0
BaseClass=CDialog
HeaderFile=FView.h
ImplementationFile=FView.cpp
LastObject=CFView

[CLS:CMcviewApp]
Type=0
BaseClass=CWinApp
HeaderFile=mcview.h
ImplementationFile=mcview.cpp

[CLS:CMcviewDlg]
Type=0
BaseClass=CDialog
HeaderFile=mcviewDlg.h
ImplementationFile=mcviewDlg.cpp

[DLG:IDD_ALARMLIST]
Type=1
Class=CDiaAlarmList
ControlCount=5
Control1=IDC_ALARMLIST,SysListView32,1350664205
Control2=IDC_ACK,button,1342242816
Control3=IDC_ACKALL,button,1342242816
Control4=IDCANCEL,button,1342242816
Control5=IDC_CHECK1,button,1342242819

[DLG:IDD_INPUT_A]
Type=1
Class=CDiaInputA
ControlCount=23
Control1=IDC_STATIC,static,1342308866
Control2=IDC_VALUE,edit,1342242946
Control3=IDC_OUTPUT,button,1342245888
Control4=IDC_LIMIT_MAX,static,1342312962
Control5=IDC_STATIC,static,1342308866
Control6=IDC_STATUS,static,1342312962
Control7=IDC_LIMIT_MIN,static,1342312962
Control8=IDC_STATIC,static,1342308866
Control9=IDC_STATIC,static,1342308866
Control10=IDCANCEL,button,1342242816
Control11=IDC_0,button,1342242816
Control12=IDC_CLR,button,1342242816
Control13=IDC_1,button,1342242816
Control14=IDC_2,button,1342242816
Control15=IDC_3,button,1342242816
Control16=IDC_4,button,1342242816
Control17=IDC_5,button,1342242816
Control18=IDC_6,button,1342242816
Control19=IDC_7,button,1342242816
Control20=IDC_8,button,1342242816
Control21=IDC_9,button,1342242816
Control22=IDC_SUB,button,1342242816
Control23=IDC_DOT,button,1342242816

[DLG:IDD_INPUT_D]
Type=1
Class=CDiaInputD
ControlCount=6
Control1=IDC_STATIC,static,1342308864
Control2=IDC_STATIC,button,1342177287
Control3=IDC_SET,button,1342242816
Control4=IDC_RESET,button,1342242816
Control5=IDC_STATUS,static,1342312960
Control6=IDCANCEL,button,1342242816

[DLG:IDD_PASSWORD]
Type=1
Class=CDiaPassword
ControlCount=17
Control1=IDC_STATIC,button,1342177287
Control2=IDC_RANK0,button,1476591625
Control3=IDC_RANK2,button,1342177289
Control4=IDC_RANK3,button,1342177289
Control5=IDC_RANK4,button,1342177289
Control6=IDC_RANK5,button,1342177289
Control7=IDC_RANK6,button,1342177289
Control8=IDC_RANK7,button,1342177289
Control9=IDC_RANK8,button,1342177289
Control10=IDC_RANK9,button,1342177289
Control11=IDC_RANK10,button,1342177289
Control12=IDC_STATIC,static,1342308866
Control13=IDC_NEW_PWD,edit,1350631584
Control14=IDC_STATIC,static,1342308866
Control15=IDC_CFM_PWD,edit,1350631584
Control16=IDOK,button,1342242817
Control17=IDCANCEL,button,1342242816

[DLG:IDD_SYSTEM]
Type=1
Class=CDiaSystem
ControlCount=21
Control1=IDC_PWD,edit,1350631584
Control2=IDC_RANK0,button,1342373892
Control3=IDC_RANK1,button,1342177284
Control4=IDC_RANK2,button,1342177284
Control5=IDC_RANK3,button,1342177284
Control6=IDC_RANK4,button,1342177284
Control7=IDC_RANK5,button,1342177284
Control8=IDC_RANK6,button,1342177284
Control9=IDC_RANK7,button,1342177284
Control10=IDC_RANK8,button,1342177284
Control11=IDC_RANK9,button,1342177284
Control12=IDOK,button,1342242816
Control13=IDC_RETURN_WND,button,1342242816
Control14=IDC_PRINT,button,1342242816
Control15=IDC_CHG_PWD,button,1342242816
Control16=IDC_ABOUT,button,1342242816
Control17=IDC_NOSWITCH,button,1342242819
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,static,1342308866
Control20=IDC_ERR,static,1342308876
Control21=IDC_SW2,listbox,1352728835

[DLG:IDD_ABOUT]
Type=1
Class=CDlgAbout
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_DNYTIP]
Type=1
Class=CDlgDnyTip
ControlCount=3
Control1=IDC_LINKS,SysListView32,1350631425
Control2=IDOK,button,1073741825
Control3=IDC_TREND,button,1342242816

[DLG:IDD_LOOP]
Type=1
Class=CDlgLoop
ControlCount=7
Control1=IDC_LOOPLIST,listbox,1352728833
Control2=IDC_INTERVAL,edit,1350631552
Control3=IDC_ADD,button,1342242817
Control4=IDC_DELETE,button,1342242817
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,button,1342177287

[DLG:IDD_SELECT]
Type=1
Class=CDlgSelect
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_OBJECTS,listbox,1352728835

[DLG:IDD_TRENDCOMPS]
Type=1
Class=CDlgTrendComSav
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_OBJECTS,listbox,1352728835

[DLG:IDD_TREND_TIME]
Type=1
Class=CDlgTrendTime
ControlCount=61
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308866
Control3=IDC_DATE,SysDateTimePick32,1342242852
Control4=IDC_TIME,SysDateTimePick32,1342242857
Control5=IDC_STATIC,static,1342308866
Control6=IDC_LENGTH,combobox,1344339971
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308866
Control9=IDC_TAG1,edit,1350631552
Control10=IDC_CA1,button,1073807363
Control11=IDC_STATIC,static,1342308866
Control12=IDC_MIN1,edit,1350631552
Control13=IDC_STATIC,static,1342308866
Control14=IDC_MAX1,edit,1350631552
Control15=IDC_STATIC,static,1342308866
Control16=IDC_TAG2,edit,1350631552
Control17=IDC_CA2,button,1073807363
Control18=IDC_STATIC,static,1342308866
Control19=IDC_MIN2,edit,1350631552
Control20=IDC_STATIC,static,1342308866
Control21=IDC_MAX2,edit,1350631552
Control22=IDC_STATIC,static,1342308866
Control23=IDC_TAG3,edit,1350631552
Control24=IDC_CA3,button,1073807363
Control25=IDC_STATIC,static,1342308866
Control26=IDC_MIN3,edit,1350631552
Control27=IDC_STATIC,static,1342308866
Control28=IDC_MAX3,edit,1350631552
Control29=IDC_STATIC,static,1342308866
Control30=IDC_TAG4,edit,1350631552
Control31=IDC_CA4,button,1073807363
Control32=IDC_STATIC,static,1342308866
Control33=IDC_MIN4,edit,1350631552
Control34=IDC_STATIC,static,1342308866
Control35=IDC_MAX4,edit,1350631552
Control36=IDC_STATIC,static,1342308866
Control37=IDC_TAG5,edit,1350631552
Control38=IDC_CA5,button,1073807363
Control39=IDC_STATIC,static,1342308866
Control40=IDC_MIN5,edit,1350631552
Control41=IDC_STATIC,static,1342308866
Control42=IDC_MAX5,edit,1350631552
Control43=IDC_STATIC,button,1342177287
Control44=IDC_SAVE,button,1342242816
Control45=IDC_LOAD,button,1342242816
Control46=IDOK,button,1342242817
Control47=IDCANCEL,button,1342242816
Control48=IDC_AUTO_SCALE,button,1342242819
Control49=IDC_10,button,1342242816
Control50=IDC_DEL,button,1342242816
Control51=IDC_1,button,1342242816
Control52=IDC_2,button,1342242816
Control53=IDC_3,button,1342242816
Control54=IDC_4,button,1342242816
Control55=IDC_5,button,1342242816
Control56=IDC_6,button,1342242816
Control57=IDC_7,button,1342242816
Control58=IDC_8,button,1342242816
Control59=IDC_9,button,1342242816
Control60=IDC_SUB,button,1342242816
Control61=IDC_DOT,button,1342242816

[DLG:IDD_FVIEW_DIALOG]
Type=1
Class=CFView
ControlCount=0

[DLG:IDD_MCVIEW_DIALOG]
Type=1
Class=CMcviewDlg
ControlCount=12
Control1=IDC_SYSTEM,button,1342177280
Control2=IDCANCEL,button,1073741824
Control3=IDC_NOW,static,1342181889
Control4=IDC_ALARM,static,1342182145
Control5=IDC_ALARMLIST,button,1342177280
Control6=IDC_ACKALARM,button,1342177280
Control7=IDC_INDEX,button,1342177280
Control8=IDC_PREVIOUS,button,1342177280
Control9=IDC_NEXT,button,1342177280
Control10=IDC_KEY,button,1073741824
Control11=IDOK,button,1073741824
Control12=IDC_LOOP,button,1342177280

[DLG:IDD_POPUP]
Type=1
Class=?
ControlCount=1
Control1=IDC_ERROR,static,1073872896

[DLG:IDD_MCVIEW_DIALOG_S]
Type=1
Class=?
ControlCount=12
Control1=IDC_SYSTEM,button,1342177280
Control2=IDCANCEL,button,1073741824
Control3=IDC_NOW,static,1342181889
Control4=IDC_ALARM,static,1342182145
Control5=IDC_ALARMLIST,button,1342177280
Control6=IDC_ACKALARM,button,1342177280
Control7=IDC_INDEX,button,1342177280
Control8=IDC_PREVIOUS,button,1342177280
Control9=IDC_NEXT,button,1342177280
Control10=IDC_KEY,button,1073741824
Control11=IDOK,button,1073741824
Control12=IDC_LOOP,button,1342177280

