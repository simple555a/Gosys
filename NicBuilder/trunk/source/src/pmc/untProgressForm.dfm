object frmProgress: TfrmProgress
  Left = 325
  Top = 203
  BorderStyle = bsDialog
  Caption = 'frmProgress'
  ClientHeight = 124
  ClientWidth = 315
  Color = clWindow
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 12
  object lblThis: TLabel
    Left = 8
    Top = 16
    Width = 42
    Height = 12
    Caption = 'lblThis'
  end
  object lblTotal: TLabel
    Left = 8
    Top = 60
    Width = 48
    Height = 12
    Caption = #24635#20307#36827#24230
  end
  object pbThis: TProgressBar
    Left = 8
    Top = 32
    Width = 300
    Height = 17
    Min = 0
    Max = 100
    TabOrder = 0
  end
  object btnCancel: TButton
    Left = 232
    Top = 100
    Width = 75
    Height = 21
    Caption = #21462#28040'(C)'
    TabOrder = 1
    OnClick = btnCancelClick
  end
  object pbTotal: TProgressBar
    Left = 8
    Top = 76
    Width = 300
    Height = 17
    Min = 0
    Max = 100
    TabOrder = 2
  end
  object lb: TListBox
    Left = 8
    Top = 128
    Width = 313
    Height = 209
    ItemHeight = 12
    TabOrder = 3
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 200
    Top = 96
  end
end
