//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#ifndef TFormMainH
#define TFormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
#include "ShajunMap.h"
#include "ShajunPalette.h"
//---------------------------------------------------------------------------
//#define DEBUG_TIME
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:
    TPanel *Panel1;
    TScrollBox *ScrollBox;
    TImage *ImgMain;
    TMainMenu *MainMenu;
    TMenuItem *File1;
    TMenuItem *MenuLoadHeightMap;
    TOpenPictureDialog *OpenPictureDialog;
    TMenuItem *View1;
    TMenuItem *MenuViewHeight;
    TMenuItem *MenuViewNormal;
    TMenuItem *MenuViewShade;
    TGroupBox *GroupBox1;
    TTrackBar *TrackBarDirection;
    TLabel *Label1;
    TEdit *EditDirection;
    TLabel *Label2;
    TTrackBar *TrackBarElevation;
    TEdit *EditElevation;
    TLabel *Label3;
    TTrackBar *TrackBarAmbient;
    TEdit *EditAmbient;
    TGroupBox *GroupBox2;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *EditScalePixel;
    TEdit *EditScaleMaxHeight;
    TGroupBox *GroupBox3;
    TBevel *Bevel1;
    TImage *ImgHeightPalette;
    TLabel *Label7;
    TBevel *Bevel2;
    TMenuItem *MenuViewColour;
    TMenuItem *MenuViewShadedColour;
    TMenuItem *MenuSaveImage;
    TMenuItem *N1;
    TMenuItem *MenuExit;
    TSavePictureDialog *SavePictureDialog;
    TLabel *Label6;
    TComboBox *ComboPreset;
    TImage *ImgHeightPresets;
    TMenuItem *N2;
    TMenuItem *MenuLoadHeightPalette;
    TMenuItem *Help1;
    TMenuItem *MenuAbout;
    TMenuItem *N3;
    TMenuItem *MenuView3D;
    TMenuItem *Zoom1;
    TMenuItem *MenuZoom1;
    TMenuItem *MenuZoom2;
    TMenuItem *MenuZoom4;
    TMenuItem *MenuZoom8;
	TCheckBox *CheckBoxSun;
	TCheckBox *CheckBoxQuantize;
	TTrackBar *TrackBarQuantization;
	TLabel *LabelQuantization;
    void __fastcall MenuLoadHeightMapClick(TObject *Sender);
    void __fastcall TrackBarDirectionChange(TObject *Sender);
    void __fastcall TrackBarElevationChange(TObject *Sender);
    void __fastcall TrackBarAmbientChange(TObject *Sender);
    void __fastcall EditScalePixelExit(TObject *Sender);
    void __fastcall EditScaleMaxHeightExit(TObject *Sender);
    void __fastcall MenuViewHeightClick(TObject *Sender);
    void __fastcall MenuViewNormalClick(TObject *Sender);
    void __fastcall MenuViewShadeClick(TObject *Sender);
	void __fastcall EditScalePixelKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall EditScaleMaxHeightKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall ImgHeightPaletteClick(TObject *Sender);
    void __fastcall MenuViewColourClick(TObject *Sender);
    void __fastcall MenuViewShadedColourClick(TObject *Sender);
    void __fastcall MenuSaveImageClick(TObject *Sender);
    void __fastcall MenuExitClick(TObject *Sender);
    void __fastcall ComboPresetChange(TObject *Sender);
    void __fastcall MenuLoadHeightPaletteClick(TObject *Sender);
    void __fastcall MenuAboutClick(TObject *Sender);
    void __fastcall MenuView3DClick(TObject *Sender);
    void __fastcall MenuZoom1Click(TObject *Sender);
    void __fastcall MenuZoom2Click(TObject *Sender);
    void __fastcall MenuZoom4Click(TObject *Sender);
    void __fastcall MenuZoom8Click(TObject *Sender);
	void __fastcall CheckBoxSunClick(TObject *Sender);
	void __fastcall TrackBarQuantizationChange(TObject *Sender);
	void __fastcall CheckBoxQuantizeClick(TObject *Sender);

private:
    void ValidatePixelScale();
    void ValidateMaxHeightScale();
    void UpdateNormalMap();
    void UpdateShadeMap();
    void UpdateColourMap();
    void DrawMap();
	void DrawPalette(const Shajun::ShajunPalette &Palette, TImage *Image);
	void QuantizeMap();

    Shajun::ShajunMap       m_Map;
	Shajun::ShajunMap       m_QuantizedMap;
	Shajun::ShajunPalette   m_HeightPalette;
	int                     m_Zoom;

public:
	__fastcall TFormMain(TComponent* Owner);
	Shajun::ShajunMap* GetMap() {
		if (CheckBoxQuantize->Checked)
			return &m_QuantizedMap;
		else
			return &m_Map;
	}
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
