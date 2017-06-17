object FormAbout: TFormAbout
  Left = 397
  Top = 316
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderStyle = bsDialog
  Caption = 'About Shajun'
  ClientHeight = 164
  ClientWidth = 326
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 113
    Height = 23
    Caption = 'Shajun v2.0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 32
    Width = 96
    Height = 13
    Caption = 'by Rowan Medhurst'
  end
  object Label4: TLabel
    Left = 8
    Top = 72
    Width = 115
    Height = 13
    Caption = 'Copyright '#169' 2004-2017'
  end
  object Label5: TLabel
    Left = 8
    Top = 88
    Width = 93
    Height = 13
    Caption = 'All Rights Reserved'
  end
  object Bevel1: TBevel
    Left = 0
    Top = 112
    Width = 500
    Height = 9
    Shape = bsTopLine
  end
  object Label3: TLabel
    Left = 8
    Top = 48
    Width = 69
    Height = 13
    Caption = 'Futurerave.uk'
  end
  object BtnOK: TButton
    Left = 240
    Top = 120
    Width = 81
    Height = 25
    Caption = 'OK'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = BtnOKClick
  end
end
