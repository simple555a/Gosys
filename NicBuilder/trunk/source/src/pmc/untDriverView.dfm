object framDriverView: TframDriverView
  Left = 0
  Top = 0
  Width = 438
  Height = 315
  Color = clCaptionText
  Ctl3D = False
  ParentColor = False
  ParentCtl3D = False
  TabOrder = 0
  DesignSize = (
    438
    315)
  object lblInitString: TLabel
    Left = 28
    Top = 104
    Width = 72
    Height = 13
    Caption = #21021#22987#21270#21629#20196#23383
  end
  object lblVendor: TLabel
    Left = 72
    Top = 56
    Width = 24
    Height = 13
    Caption = #21378#21830
  end
  object lblVersion: TLabel
    Left = 72
    Top = 80
    Width = 24
    Height = 13
    Caption = #29256#26412
  end
  object edtInit: TEdit
    Left = 108
    Top = 104
    Width = 309
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    Text = 'edtInit'
  end
  object Button1: TButton
    Left = 108
    Top = 128
    Width = 87
    Height = 21
    Caption = #20445#23384'(&S)'
    TabOrder = 1
    OnClick = Button1Click
  end
  object edtVersion: TEdit
    Left = 108
    Top = 80
    Width = 309
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    Enabled = False
    ReadOnly = True
    TabOrder = 2
    Text = 'edtInit'
  end
  object edtVendor: TEdit
    Left = 108
    Top = 56
    Width = 309
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    Enabled = False
    ReadOnly = True
    TabOrder = 3
    Text = 'edtInit'
  end
end
