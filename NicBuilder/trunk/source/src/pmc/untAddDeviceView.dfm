object framAddDevice: TframAddDevice
  Left = 0
  Top = 0
  Width = 487
  Height = 364
  TabOrder = 0
  object Label1: TLabel
    Left = 28
    Top = 24
    Width = 306
    Height = 13
    Caption = #30446#26631#33410#28857#19978#25903#25345#30340#35774#22791#31867#22411#21015#34920', '#35831#20174#20013#36873#25321#26032#35774#22791#30340#31867#22411
  end
  object Label8: TLabel
    Left = 28
    Top = 52
    Width = 48
    Height = 13
    Caption = #29983#20135#21378#21830
  end
  object Label9: TLabel
    Left = 204
    Top = 52
    Width = 48
    Height = 13
    Caption = #35774#22791#22411#21495
  end
  object Button1: TButton
    Left = 260
    Top = 322
    Width = 100
    Height = 21
    Caption = #28155#21152'(S)'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 364
    Top = 322
    Width = 100
    Height = 21
    Caption = #24110#21161'(H)'
    TabOrder = 1
    OnClick = Button2Click
  end
  object GroupBox2: TGroupBox
    Left = 28
    Top = 208
    Width = 437
    Height = 101
    Caption = #26032#35774#22791#21442#25968
    TabOrder = 2
    object Label5: TLabel
      Left = 20
      Top = 48
      Width = 24
      Height = 13
      Caption = #22320#22336
    end
    object Label6: TLabel
      Left = 20
      Top = 72
      Width = 72
      Height = 13
      Caption = #21021#22987#21270#21629#20196#23383
    end
    object Label7: TLabel
      Left = 20
      Top = 24
      Width = 24
      Height = 13
      Caption = #21517#31216
    end
    object edtAddress: TEdit
      Left = 116
      Top = 44
      Width = 297
      Height = 21
      TabOrder = 0
      Text = 'edtAddress'
    end
    object edtInit: TEdit
      Left = 116
      Top = 68
      Width = 297
      Height = 21
      TabOrder = 1
      Text = 'edtInit'
    end
    object edtName: TEdit
      Left = 116
      Top = 20
      Width = 297
      Height = 21
      TabOrder = 2
      Text = 'edtAddress'
    end
  end
  object lstVendors: TListBox
    Left = 28
    Top = 72
    Width = 169
    Height = 125
    ItemHeight = 13
    TabOrder = 3
    OnClick = lstVendorsClick
  end
  object lstTypes: TListBox
    Left = 204
    Top = 72
    Width = 261
    Height = 125
    ItemHeight = 13
    TabOrder = 4
  end
end
