//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#include <vector>
#pragma hdrstop
#include "TFormMain.h"
#include "ShajunUtil.h"
#include "TFormAbout.h"
#include "TForm3D.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
using std::vector;
using Shajun::ShajunVertex;
using Shajun::ShajunRGB;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
:
    TForm(Owner)
{
    TrackBarDirection->OnChange(this);
    TrackBarElevation->OnChange(this);
    TrackBarAmbient->OnChange(this);

    ComboPreset->ItemIndex = 0;
    ComboPreset->OnChange(this);

	MenuZoom1->OnClick(this);

	Panel1->DoubleBuffered = true;


	//************************************************//
		try
		{
			m_Map.LoadHeightMap("sample256.bmp");
			QuantizeMap();
			UpdateNormalMap();
			DrawMap();
		}
		catch (...)
		{
		}


}
//---------------------------------------------------------------------------
void TFormMain::ValidatePixelScale()
{
    if (EditScalePixel->Text.ToIntDef(0) <= 0)
    {
        Application->MessageBox("Pixel scale must be a positive integer.", "Error!", MB_OK);
        EditScalePixel->Text = 1;
    }
}
//---------------------------------------------------------------------------
void TFormMain::ValidateMaxHeightScale()
{
    if (EditScaleMaxHeight->Text.ToIntDef(0) <= 0)
    {
        Application->MessageBox("Max height scale must be a positive integer.", "Error!", MB_OK);
        EditScaleMaxHeight->Text = 1;
    }
}
//---------------------------------------------------------------------------
void TFormMain::UpdateNormalMap()
{
    float pixel, maxHeight;
    ValidatePixelScale();
    pixel = EditScalePixel->Text.ToIntDef(0);
    maxHeight = EditScaleMaxHeight->Text.ToIntDef(0);

	m_Map.CalcNormalMap(maxHeight / pixel);
	m_QuantizedMap.CalcNormalMap(maxHeight / pixel);
	UpdateShadeMap();
}
//---------------------------------------------------------------------------
void TFormMain::UpdateShadeMap()
{
	D3DXVECTOR3 sunVec;
	float dir, elev, z, ambient;

	dir  = Shajun::PiDiv180 * TrackBarDirection->Position;
	elev = Shajun::PiDiv180 * TrackBarElevation->Position;
	z = cos(elev);
	sunVec.x = sin(dir) * z;
	sunVec.y = -sin(elev);
	sunVec.z = cos(dir) * z;
	ambient = static_cast<float>(TrackBarAmbient->Position) / 100;

	m_Map.CalcShadeMap(CheckBoxSun->Checked, sunVec, ambient);
	m_QuantizedMap.CalcShadeMap(CheckBoxSun->Checked, sunVec, ambient);
	UpdateColourMap();
}
//---------------------------------------------------------------------------
void TFormMain::UpdateColourMap()
{
    m_Map.CalcColourMap(m_HeightPalette);
	m_QuantizedMap.CalcColourMap(m_HeightPalette);
}
//---------------------------------------------------------------------------
void TFormMain::DrawMap()
{
    ImgMain->Picture->Bitmap->Width = m_Map.GetWidth();
    ImgMain->Picture->Bitmap->Height = m_Map.GetDepth();
    ImgMain->Picture->Bitmap->PixelFormat = pf24bit;
    ImgMain->Width = m_Zoom * m_Map.GetWidth();
    ImgMain->Height = m_Zoom * m_Map.GetDepth();
	vector<ShajunVertex>::iterator itShajunVert;

	if (CheckBoxQuantize->Checked)
		itShajunVert = m_QuantizedMap.GetBegin();
	else
		itShajunVert = m_Map.GetBegin();

#ifdef DEBUG_TIME
    LARGE_INTEGER freq, start, end;
    double elapsedSeconds;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif
    // 2D view

    for (int z = 0; z < m_Map.GetDepth(); z++)
    {
        BYTE *pixel = reinterpret_cast<BYTE*>(ImgMain->Picture->Bitmap->ScanLine[m_Map.GetDepth()-1-z]);

        for (int x = 0; x < m_Map.GetWidth(); x++)
        {
            if (MenuViewHeight->Checked)
			{
				pixel[2] = pixel[1] = pixel[0] = itShajunVert->GetHeight();
            }
            else if (MenuViewNormal->Checked)
            {
                ShajunRGB rgb = Shajun::NormalToRGB(itShajunVert->GetNormal());
                pixel[2] = rgb.GetRed();
                pixel[1] = rgb.GetGreen();
                pixel[0] = rgb.GetBlue();
            }
            else if (MenuViewShade->Checked)
            {
                pixel[0] = pixel[1] = pixel[2] = itShajunVert->GetShade();
            }
            else if (MenuViewColour->Checked)
            {
                ShajunRGB rgb = itShajunVert->GetColour();
                pixel[2] = rgb.GetRed();
                pixel[1] = rgb.GetGreen();
                pixel[0] = rgb.GetBlue();
            }
            else if (MenuViewShadedColour->Checked)
            {
                ShajunRGB rgb = itShajunVert->GetColour();
                BYTE shade = itShajunVert->GetShade();
                pixel[2] = shade * rgb.GetRed() >> 8;
                pixel[1] = shade * rgb.GetGreen() >> 8;
                pixel[0] = shade * rgb.GetBlue() >> 8;
            }
            itShajunVert++;
            pixel += 3;
        }
    }

    // 3D view

    if (Form3D && Form3D->Visible)
    {
        Form3D->RenderMap();
    }

#ifdef DEBUG_TIME
    QueryPerformanceCounter(&end);
    elapsedSeconds = static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
    Caption = elapsedSeconds;
#endif
    //----
    ImgMain->Repaint();
}
//---------------------------------------------------------------------------
void TFormMain::DrawPalette(const Shajun::ShajunPalette &Palette, TImage *Image)
{
    BYTE *pixel;
    Image->Picture->Bitmap->Width = Palette.GetNumColours();
    Image->Picture->Bitmap->Height = 1;
    Image->Picture->Bitmap->PixelFormat = pf24bit;
    pixel = reinterpret_cast<BYTE*>(Image->Picture->Bitmap->ScanLine[0]);

    for (int i = 0; i < Palette.GetNumColours(); i++)
    {
        ShajunRGB rgb = Palette.GetColour(i);
        pixel[2] = rgb.GetRed();
        pixel[1] = rgb.GetGreen();
        pixel[0] = rgb.GetBlue();
        pixel += 3;
    }
    Image->Repaint();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// File menu event handlers
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuLoadHeightMapClick(TObject *Sender)
{
    if (OpenPictureDialog->Execute())
    {
        try
        {
            m_Map.LoadHeightMap(OpenPictureDialog->FileName);
        }
        catch (...)
        {
            Application->MessageBox("Could not open file.", "Error!", MB_OK);
            return;
        }

		CheckBoxQuantize->Checked = false;
        UpdateNormalMap();
        DrawMap();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuLoadHeightPaletteClick(TObject *Sender)
{
    if (OpenPictureDialog->Execute())
    {
        try
        {
            m_HeightPalette.Load(OpenPictureDialog->FileName);
        }
        catch (...)
        {
            Application->MessageBox("Could not open file.", "Error!", MB_OK);
            return;
        }

        DrawPalette(m_HeightPalette, ImgHeightPalette);
        UpdateColourMap();
        DrawMap();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuSaveImageClick(TObject *Sender)
{
    if (SavePictureDialog->Execute())
    {
        ImgMain->Picture->Bitmap->SaveToFile(SavePictureDialog->FileName);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// View menu event handlers
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuViewHeightClick(TObject *Sender)
{
    MenuViewHeight->Checked = true;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuViewNormalClick(TObject *Sender)
{
    MenuViewNormal->Checked = true;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuViewShadeClick(TObject *Sender)
{
    MenuViewShade->Checked = true;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuViewColourClick(TObject *Sender)
{
    MenuViewColour->Checked = true;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuViewShadedColourClick(TObject *Sender)
{
    MenuViewShadedColour->Checked = true;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuView3DClick(TObject *Sender)
{
    MenuView3D->Checked = !MenuView3D->Checked;
    Form3D->Visible = MenuView3D->Checked;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Zoom menu event handlers
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuZoom1Click(TObject *Sender)
{
    MenuZoom1->Checked = true;
    m_Zoom = 1;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuZoom2Click(TObject *Sender)
{
    MenuZoom2->Checked = true;
    m_Zoom = 2;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuZoom4Click(TObject *Sender)
{
    MenuZoom4->Checked = true;
    m_Zoom = 4;
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuZoom8Click(TObject *Sender)
{
    MenuZoom8->Checked = true;
    m_Zoom = 8;
    DrawMap();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Help menu event handlers
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuAboutClick(TObject *Sender)
{
    FormMain->Enabled = false;
    Form3D->Enabled = false;
    FormAbout->Show();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Light panel event handlers
//---------------------------------------------------------------------------
void __fastcall TFormMain::TrackBarDirectionChange(TObject *Sender)
{
    EditDirection->Text = TrackBarDirection->Position;
    UpdateShadeMap();
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TrackBarElevationChange(TObject *Sender)
{
    EditElevation->Text = TrackBarElevation->Position;
    UpdateShadeMap();
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TrackBarAmbientChange(TObject *Sender)
{
    EditAmbient->Text = TrackBarAmbient->Position;
	UpdateShadeMap();
    DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::CheckBoxSunClick(TObject *Sender)
{
	if (CheckBoxSun->Checked) {
		TrackBarDirection->Enabled = true;
		EditDirection->Enabled = true;
		TrackBarElevation->Enabled = true;
		EditElevation->Enabled = true;
	}
	else {
		TrackBarDirection->Enabled = false;
		EditDirection->Enabled = false;
		TrackBarElevation->Enabled = false;
		EditElevation->Enabled = false;
	}
	UpdateShadeMap();
	DrawMap();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Colour panel event handlers
//---------------------------------------------------------------------------
void __fastcall TFormMain::ImgHeightPaletteClick(TObject *Sender)
{
    MenuLoadHeightPalette->OnClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ComboPresetChange(TObject *Sender)
{
    Graphics::TBitmap *bitmap = ImgHeightPresets->Picture->Bitmap;

    if (ComboPreset->ItemIndex >= bitmap->Height)
    {
        Application->MessageBox("Preset not found.", "Error!", MB_OK);
        return;
    }

    // ----
    BYTE *pixel = reinterpret_cast<BYTE*>(bitmap->ScanLine[ComboPreset->ItemIndex]);
    m_HeightPalette.Create(bitmap->Width);

    for (int i = 0; i < m_HeightPalette.GetNumColours(); i++)
    {
        ShajunRGB rgb(pixel[2], pixel[1], pixel[0]);
        m_HeightPalette.SetColour(i, rgb);
        pixel += 3;
    }

    DrawPalette(m_HeightPalette, ImgHeightPalette);
    UpdateColourMap();
    DrawMap();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Scale panel event handlers
//---------------------------------------------------------------------------
void __fastcall TFormMain::EditScalePixelExit(TObject *Sender)
{
	ValidatePixelScale();
	UpdateNormalMap();
	DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::EditScaleMaxHeightExit(TObject *Sender)
{
	ValidateMaxHeightScale();
	UpdateNormalMap();
	DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::EditScalePixelKeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		EditScalePixelExit(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::EditScaleMaxHeightKeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		EditScaleMaxHeightExit(this);
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Quantization functions
//---------------------------------------------------------------------------
void __fastcall TFormMain::TrackBarQuantizationChange(TObject *Sender)
{
	int numSteps = pow(2, TrackBarQuantization->Position + 1);
	LabelQuantization->Caption = numSteps;
	TrackBarQuantization->Tag = numSteps;
	QuantizeMap();
	UpdateNormalMap();
	DrawMap();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::CheckBoxQuantizeClick(TObject *Sender)
{
	if (CheckBoxQuantize->Checked) {
		TrackBarQuantization->Enabled = true;
		QuantizeMap();
		UpdateNormalMap();
	}
	else {
		TrackBarQuantization->Enabled = false;
	}
	DrawMap();
}
//---------------------------------------------------------------------------
void TFormMain::QuantizeMap()
{
	m_QuantizedMap.QuantizeFrom(m_Map, TrackBarQuantization->Tag);
}
//---------------------------------------------------------------------------

