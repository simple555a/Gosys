object frmBlkAttrib: TfrmBlkAttrib
  Left = 247
  Top = 225
  BorderStyle = bsDialog
  Caption = #21151#33021#22359#23646#24615
  ClientHeight = 203
  ClientWidth = 410
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 5
    Top = 11
    Width = 42
    Height = 12
    Caption = #21517#31216'(&S)'
    FocusControl = edtName
  end
  object Label2: TLabel
    Left = 5
    Top = 34
    Width = 42
    Height = 12
    Caption = #31867#22411'(&X)'
    FocusControl = edtCName
  end
  object Label3: TLabel
    Left = 17
    Top = 62
    Width = 42
    Height = 12
    Caption = #27880#37322'(&E)'
    FocusControl = mmComment
  end
  object Label4: TLabel
    Left = 9
    Top = 128
    Width = 48
    Height = 12
    Caption = #21151#33021#35828#26126
    FocusControl = Memo1
  end
  object Button1: TButton
    Left = 285
    Top = 9
    Width = 120
    Height = 21
    Caption = #30830#23450'(&O)'
    Default = True
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 285
    Top = 32
    Width = 120
    Height = 21
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 1
  end
  object edtName: TEdit
    Left = 60
    Top = 8
    Width = 219
    Height = 20
    TabOrder = 2
    Text = 'edtName'
  end
  object edtCName: TEdit
    Left = 60
    Top = 34
    Width = 219
    Height = 20
    ReadOnly = True
    TabOrder = 3
    Text = 'edtCName'
  end
  object mmComment: TMemo
    Left = 60
    Top = 59
    Width = 221
    Height = 60
    TabOrder = 4
  end
  object Button4: TButton
    Left = 286
    Top = 81
    Width = 120
    Height = 21
    Caption = #21151#33021#22359#24110#21161
    TabOrder = 5
    OnClick = Button4Click
  end
  object btnViewSource: TButton
    Left = 285
    Top = 56
    Width = 120
    Height = 21
    Caption = #26597#30475#28304#20195#30721'(&S)'
    ModalResult = 2
    TabOrder = 6
    OnClick = btnViewSourceClick
  end
  object chkDisableBlk: TCheckBox
    Left = 58
    Top = 184
    Width = 97
    Height = 17
    Caption = #31105#29992#27492#21151#33021#22359
    TabOrder = 7
    Visible = False
  end
  object Memo1: TMemo
    Left = 60
    Top = 123
    Width = 221
    Height = 60
    Color = clScrollBar
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 8
  end
end
