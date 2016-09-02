object framDeviceView: TframDeviceView
  Left = 0
  Top = 0
  Width = 478
  Height = 362
  Ctl3D = False
  ParentCtl3D = False
  TabOrder = 0
  object lblInitString: TLabel
    Left = 44
    Top = 112
    Width = 84
    Height = 13
    Caption = #21021#22987#21270#21629#20196#23383
  end
  object lblVersion: TLabel
    Left = 68
    Top = 89
    Width = 56
    Height = 13
    Caption = #35774#22791#22320#22336
  end
  object Label1: TLabel
    Left = 68
    Top = 66
    Width = 56
    Height = 13
    Caption = #35774#22791#22411#21495
  end
  object Label2: TLabel
    Left = 68
    Top = 44
    Width = 56
    Height = 13
    Caption = #35774#22791#21378#21830
  end
  object edtInit: TEdit
    Left = 128
    Top = 112
    Width = 277
    Height = 19
    TabOrder = 0
    Text = 'edtInit'
  end
  object edtAddress: TEdit
    Left = 128
    Top = 88
    Width = 277
    Height = 19
    TabOrder = 1
    Text = 'edtInit'
  end
  object Button1: TButton
    Left = 206
    Top = 142
    Width = 200
    Height = 21
    Caption = #20445#23384#24403#21069#35774#32622'(&S)'
    TabOrder = 2
    OnClick = Button1Click
  end
  object edtVendor: TEdit
    Left = 128
    Top = 40
    Width = 277
    Height = 19
    Color = clInactiveBorder
    ReadOnly = True
    TabOrder = 3
  end
  object edtType: TEdit
    Left = 128
    Top = 64
    Width = 277
    Height = 19
    Color = clInactiveBorder
    ReadOnly = True
    TabOrder = 4
  end
  object Button2: TButton
    Left = 207
    Top = 166
    Width = 200
    Height = 21
    Caption = #24110#21161'(&H)'
    TabOrder = 5
    OnClick = Button2Click
  end
end
