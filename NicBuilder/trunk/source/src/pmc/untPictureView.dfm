object framPictureView: TframPictureView
  Left = 0
  Top = 0
  Width = 476
  Height = 530
  TabOrder = 0
  DesignSize = (
    476
    530)
  object Label1: TLabel
    Left = 52
    Top = 40
    Width = 126
    Height = 13
    Caption = #24050#26377#30340#32452#24577#30011#38754#21015#34920
  end
  object lv: TListView
    Left = 52
    Top = 88
    Width = 395
    Height = 396
    Anchors = [akLeft, akTop, akBottom]
    Columns = <
      item
        Caption = #21517#31216
        Width = 350
      end>
    ReadOnly = True
    RowSelect = True
    SmallImages = ImageList1
    TabOrder = 0
    ViewStyle = vsReport
    OnClick = lvClick
    OnDblClick = lvDblClick
  end
  object edtName: TEdit
    Left = 52
    Top = 58
    Width = 235
    Height = 21
    TabOrder = 1
    Text = 'edtName'
  end
  object Button1: TButton
    Left = 292
    Top = 58
    Width = 75
    Height = 21
    Caption = #26032#24314'(&N)'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 370
    Top = 58
    Width = 75
    Height = 21
    Caption = #21024#38500'(&D)'
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 368
    Top = 34
    Width = 75
    Height = 21
    Anchors = [akLeft, akBottom]
    Caption = #32534#36753'(&E)'
    TabOrder = 4
    Visible = False
    OnClick = Button3Click
  end
  object ImageList1: TImageList
    Left = 296
    Top = 24
  end
end
