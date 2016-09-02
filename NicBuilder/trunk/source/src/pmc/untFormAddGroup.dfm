object frmAddGroup: TfrmAddGroup
  Left = 301
  Top = 223
  BorderStyle = bsDialog
  Caption = #22686#21152#23454#26102#25968#25454#20998#32452
  ClientHeight = 265
  ClientWidth = 445
  Color = clCaptionText
  Ctl3D = False
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Label7: TLabel
    Left = 28
    Top = 163
    Width = 240
    Height = 12
    Caption = #26412#32452#20869#30340#23454#26102#25968#25454#30001#19979#38754#30340#39537#21160#31243#24207#36127#36131#26356#26032
  end
  object Label6: TLabel
    Left = 28
    Top = 104
    Width = 36
    Height = 12
    Caption = #20013#25991#21517
  end
  object Label1: TLabel
    Left = 32
    Top = 56
    Width = 36
    Height = 12
    Caption = #20851#38190#23383
  end
  object lblHint: TLabel
    Left = 28
    Top = 240
    Width = 42
    Height = 12
    Caption = 'lblHint'
  end
  object Image1: TImage
    Left = 24
    Top = 36
    Width = 401
    Height = 9
    AutoSize = True
  end
  object Label4: TLabel
    Left = 28
    Top = 16
    Width = 119
    Height = 16
    Caption = #25351#23450#26032#20998#32452#21442#25968
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #40657#20307
    Font.Style = [fsBold]
    ParentFont = False
  end
  object cmbDevice: TComboBoxEx
    Left = 28
    Top = 186
    Width = 393
    Height = 21
    ItemsEx.CaseSensitive = False
    ItemsEx.SortType = stNone
    ItemsEx = <
      item
        Caption = 'S960'#21069#31471
      end
      item
        Caption = 'OMRON PLC'
      end
      item
        Caption = 'MODICON PLC'
      end>
    Style = csExDropDownList
    StyleEx = []
    Ctl3D = False
    ItemHeight = 16
    ParentCtl3D = False
    TabOrder = 2
    DropDownCount = 8
  end
  object edtCName: TEdit
    Left = 28
    Top = 128
    Width = 393
    Height = 18
    TabOrder = 1
    Text = 'edtCName'
  end
  object btnSave: TButton
    Left = 200
    Top = 212
    Width = 75
    Height = 21
    Caption = #19979#19968#20010'(&S)'
    Default = True
    TabOrder = 3
    OnClick = btnSaveClick
  end
  object edtName: TEdit
    Left = 28
    Top = 76
    Width = 393
    Height = 18
    TabOrder = 0
    Text = 'edtCName'
  end
  object Button1: TButton
    Left = 280
    Top = 212
    Width = 69
    Height = 21
    Caption = #20851#38381'(&C)'
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 352
    Top = 212
    Width = 69
    Height = 21
    Caption = #24110#21161'(&H)'
    TabOrder = 5
  end
end
