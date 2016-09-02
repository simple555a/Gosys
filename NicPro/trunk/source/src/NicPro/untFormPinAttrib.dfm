object frmPinAttrib: TfrmPinAttrib
  Left = 234
  Top = 121
  BorderStyle = bsDialog
  Caption = #24341#33050#23646#24615
  ClientHeight = 564
  ClientWidth = 590
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Label1: TLabel
    Left = 70
    Top = 31
    Width = 60
    Height = 12
    Caption = #21464#37327#31867#22411': '
  end
  object Label3: TLabel
    Left = 6
    Top = 6
    Width = 48
    Height = 12
    Caption = #24341#33050#21517#31216
  end
  object grpBinding: TGroupBox
    Left = 8
    Top = 71
    Width = 325
    Height = 59
    TabOrder = 0
    object rbConst: TRadioButton
      Left = 8
      Top = 9
      Width = 113
      Height = 17
      Caption = #24120#25968
      TabOrder = 0
    end
    object rbAuto: TRadioButton
      Left = 8
      Top = 30
      Width = 113
      Height = 17
      Caption = #21464#37327
      TabOrder = 1
      OnClick = rbAutoClick
    end
    object edtConst: TEdit
      Left = 132
      Top = 8
      Width = 149
      Height = 20
      TabOrder = 2
      Text = 'edtBinding'
      OnEnter = edtConstEnter
    end
    object edtAuto: TEdit
      Left = 132
      Top = 31
      Width = 149
      Height = 20
      TabOrder = 3
      Text = 'edtBinding'
      OnChange = edtAutoChange
      OnEnter = edtAutoEnter
    end
  end
  object Button2: TButton
    Left = 348
    Top = 27
    Width = 134
    Height = 21
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 1
  end
  object Button3: TButton
    Left = 348
    Top = 2
    Width = 134
    Height = 21
    Caption = #30830#23450'(&O)'
    Default = True
    TabOrder = 2
    OnClick = Button3Click
  end
  object chkInvert: TCheckBox
    Left = 8
    Top = 49
    Width = 74
    Height = 17
    Caption = #21462#21453
    TabOrder = 3
  end
  object edtName: TEdit
    Left = 70
    Top = 3
    Width = 262
    Height = 20
    Color = clScrollBar
    Ctl3D = True
    Enabled = False
    ParentCtl3D = False
    TabOrder = 4
  end
  object Button1: TButton
    Left = 348
    Top = 108
    Width = 134
    Height = 20
    Caption = #26597#25214#21464#37327' >>(&V)'
    TabOrder = 5
    OnClick = Button1Click
  end
  inline framVariables1: TframVariables
    Left = 6
    Top = 133
    Width = 561
    Height = 428
    TabOrder = 6
    inherited lvVariables: TListView
      OnChange = framVariables1lvVariablesChange
    end
    inherited Panel1: TPanel
      inherited Button4: TButton
        OnClick = framVariables1Button4Click
      end
      inherited ListBox1: TListBox
        ItemHeight = 12
      end
    end
  end
end
