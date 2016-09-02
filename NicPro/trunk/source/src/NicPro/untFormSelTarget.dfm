object frmSelTarget: TfrmSelTarget
  Left = 306
  Top = 292
  BorderStyle = bsDialog
  Caption = #30446#26631#31995#32479#36873#25321
  ClientHeight = 78
  ClientWidth = 317
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 4
    Top = 3
    Width = 72
    Height = 13
    Caption = #30446#26631#31995#32479#22320#22336
  end
  object cmbTarget: TComboBox
    Left = 4
    Top = 21
    Width = 300
    Height = 21
    ItemHeight = 13
    TabOrder = 0
    Text = 'panxh'
  end
  object Button1: TButton
    Left = 57
    Top = 49
    Width = 81
    Height = 21
    Caption = #32852#26426'(&O)'
    Default = True
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 140
    Top = 49
    Width = 81
    Height = 21
    Cancel = True
    Caption = #21462#28040'(&C)'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 223
    Top = 49
    Width = 81
    Height = 21
    Caption = #24110#21161'(&H)'
    TabOrder = 3
    OnClick = actHelpExecute
  end
end
