object framVariables: TframVariables
  Left = 0
  Top = 0
  Width = 561
  Height = 425
  TabOrder = 0
  object lvVariables: TListView
    Left = 0
    Top = 0
    Width = 561
    Height = 324
    Align = alTop
    Columns = <
      item
        Width = 18
      end
      item
        Caption = #21517#31216
        Width = 120
      end
      item
        Caption = #31867#22411
        Width = 75
      end
      item
        Caption = #24341#29992
      end
      item
        Caption = #20540
      end
      item
        Caption = #21021#22987#20540
      end
      item
        Caption = #22320#22336
        Width = 75
      end
      item
        Caption = #27880#37322
        Width = 300
      end>
    Ctl3D = False
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    SortType = stData
    TabOrder = 0
    ViewStyle = vsReport
    OnColumnClick = lvVariablesColumnClick
    OnCompare = lvVariablesCompare
    OnDblClick = lvVariablesDblClick
  end
  object Panel1: TPanel
    Left = -1
    Top = 326
    Width = 562
    Height = 99
    BevelOuter = bvNone
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 1
    DesignSize = (
      562
      99)
    object SpeedButton1: TSpeedButton
      Left = 264
      Top = 4
      Width = 23
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      OnClick = SpeedButton1Click
    end
    object Button4: TButton
      Left = 288
      Top = 5
      Width = 65
      Height = 21
      Anchors = [akTop, akRight]
      Caption = #22686#21152'(&A)'
      TabOrder = 0
      OnClick = Button4Click
    end
    object Button1: TButton
      Left = 353
      Top = 5
      Width = 65
      Height = 21
      Anchors = [akTop, akRight]
      Caption = #21024#38500'(&D)'
      TabOrder = 1
      OnClick = Button1Click
    end
    object Button5: TButton
      Left = 419
      Top = 5
      Width = 65
      Height = 21
      Anchors = [akTop, akRight]
      Caption = #20462#25913'(&M)'
      TabOrder = 2
      OnClick = Button5Click
    end
    object Button2: TButton
      Left = 483
      Top = 5
      Width = 65
      Height = 21
      Anchors = [akTop, akRight]
      Caption = #21047#26032'(&R)'
      TabOrder = 3
      OnClick = Button2Click
    end
    object ListBox1: TListBox
      Left = 8
      Top = 28
      Width = 549
      Height = 69
      Anchors = [akLeft, akTop, akRight, akBottom]
      ItemHeight = 13
      TabOrder = 4
      OnClick = ListBox1Click
    end
    object Edit1: TEdit
      Left = 8
      Top = 8
      Width = 253
      Height = 19
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      TabOrder = 5
      OnKeyPress = Edit1KeyPress
    end
  end
  object Timer1: TTimer
    Left = 92
    Top = 95
  end
  object ppm: TPopupMenu
    Left = 122
    Top = 96
  end
end
