object framAddTag: TframAddTag
  Left = 0
  Top = 0
  Width = 489
  Height = 555
  Ctl3D = False
  ParentCtl3D = False
  TabOrder = 0
  object lblHint: TLabel
    Left = 62
    Top = 460
    Width = 29
    Height = 13
    Caption = 'lblHint'
  end
  inline framTag1: TframTag
    Left = 25
    Top = 15
    Width = 436
    Height = 434
    Color = clBtnHighlight
    Ctl3D = False
    ParentColor = False
    ParentCtl3D = False
    TabOrder = 0
    inherited pnlAdvanced: TPanel
      Left = 4
      Top = 26
      Height = 395
      inherited edtTagName: TEdit
        Color = clWindow
        ReadOnly = False
      end
      inherited cmbType: TComboBox
        OnChange = framTag1cmbTypeChange
      end
    end
    inherited btnSave: TButton
      Visible = False
    end
    inherited tmrUpdater: TTimer
      Enabled = False
    end
  end
  object btnNext: TButton
    Left = 328
    Top = 459
    Width = 100
    Height = 21
    Caption = #23436#25104#28155#21152'(&S)'
    TabOrder = 1
    OnClick = btnNextClick
  end
  object btnHelp: TButton
    Left = 124
    Top = 459
    Width = 85
    Height = 21
    Caption = #24110#21161'(&H)'
    TabOrder = 2
    Visible = False
    OnClick = btnHelpClick
  end
end
