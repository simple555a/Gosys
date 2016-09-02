object framDevice: TframDevice
  Left = 0
  Top = 0
  Width = 438
  Height = 135
  TabOrder = 0
  object Image1: TImage
    Left = -2
    Top = 3
    Width = 432
    Height = 128
    OnMouseDown = Image1MouseDown
  end
  object Panel1: TPanel
    Left = 6
    Top = 5
    Width = 87
    Height = 116
    BevelOuter = bvLowered
    Caption = 'Panel1'
    Color = clBtnText
    TabOrder = 0
    OnDblClick = Panel1DblClick
    object lblPos: TLabel
      Left = 5
      Top = 7
      Width = 38
      Height = 13
      Caption = 'lblName'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
    object lblName: TLabel
      Left = 5
      Top = 22
      Width = 38
      Height = 13
      Caption = 'lblName'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
  end
end
