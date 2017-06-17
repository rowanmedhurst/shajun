//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#include <vector>
#pragma hdrstop
#include "TForm3D.h"
#include "LVERTEX.h"
#include "TFormMain.h"
#include "ShajunUtil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3D *Form3D;
using std::vector;
using Shajun::ShajunVertex;
using Shajun::ShajunRGB;
//---------------------------------------------------------------------------
__fastcall TForm3D::TForm3D(TComponent* Owner)
:
    TForm(Owner),
    m_D3DOK(false),
    m_CamAngle(0.0),
    m_CamElev(0.7),
    m_CamDistMul(1.0)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3D::FormActivate(TObject *Sender)
{
    static bool active = false;
    if (active) return;
    active = true;

    if (m_Display.Create(this))
    {
        m_D3DOK = true;
    }
    else
    {
        Application->MessageBox("Could not initialise Direct3D.\n3D view will not be available.", "Error!", MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm3D::FormClose(TObject *Sender, TCloseAction &Action)
{
    FormMain->MenuView3D->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm3D::FormResize(TObject *Sender)
{
    if (!m_D3DOK) return;
    m_Display.Reset(this);
    RenderMap();
}
//---------------------------------------------------------------------------
void __fastcall TForm3D::FormPaint(TObject *Sender)
{
    if (!m_D3DOK) return;
    RenderMap();
}
//---------------------------------------------------------------------------
void TForm3D::RenderMap()
{
	if (!m_D3DOK) return;

	const float pixelScale = FormMain->EditScalePixel->Text.ToIntDef(0);
	const float maxHeightScale = FormMain->EditScaleMaxHeight->Text.ToIntDef(0);
	const float hsDiv255 = maxHeightScale / 255.0f;
	vector<ShajunVertex>::iterator itShajunVert = FormMain->GetMap()->GetBegin();
	LVERTEX d3dVerts[4];

	// Camera

	D3DXVECTOR3 eye, lookat;
	float camDist, horzDist;

	lookat.x = 0.5 * FormMain->GetMap()->GetWidth() * pixelScale;
	lookat.z = 0.5 * FormMain->GetMap()->GetDepth() * pixelScale;
	lookat.y = 0;

	camDist = m_CamDistMul * pixelScale * 0.5 * (FormMain->GetMap()->GetWidth() + FormMain->GetMap()->GetDepth());
	horzDist = camDist * cos(m_CamElev);

	eye.x = lookat.x - horzDist * (sin(m_CamAngle));
	eye.z = lookat.z - horzDist * (cos(m_CamAngle));
	eye.y = lookat.y + camDist * sin(m_CamElev);
	m_Display.SetView(eye, lookat, D3DXVECTOR3(0, 1, 0));

	//----
	m_Display.ClearZRGB(255, 255, 255);
	m_Display.SetProjection(1.1, static_cast<float>(ClientWidth) / ClientHeight, 1, 1000);
	m_Display.GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	m_Display.GetDevice()->SetFVF(FVF_LVERTEX);

	if (m_Display.Begin())
	{
		for (int z = 0; z < FormMain->GetMap()->GetDepth() - 1; z++)
		{
			d3dVerts[0].z = d3dVerts[2].z = z * pixelScale;
			d3dVerts[1].z = d3dVerts[3].z = (z + 1) * pixelScale;

			for (int x = 0; x < FormMain->GetMap()->GetWidth() - 1; x++)
			{
				const int offset[4] = { 0, FormMain->GetMap()->GetWidth(), 1, FormMain->GetMap()->GetWidth() + 1 };

				d3dVerts[0].x = d3dVerts[1].x = x * pixelScale;
				d3dVerts[2].x = d3dVerts[3].x = (x + 1) * pixelScale;

				for (int i = 0; i < 4; i++)
				{
					d3dVerts[i].y = itShajunVert[offset[i]].GetHeight() * hsDiv255;

					if (FormMain->MenuViewHeight->Checked)
					{
						BYTE height = itShajunVert[offset[i]].GetHeight();
						d3dVerts[i].colour = D3DCOLOR_XRGB(height, height, height);
					}
					else if (FormMain->MenuViewNormal->Checked)
					{
						ShajunRGB rgb = Shajun::NormalToRGB(itShajunVert[offset[i]].GetNormal());
						d3dVerts[i].colour = D3DCOLOR_XRGB(rgb.GetRed(), rgb.GetGreen(), rgb.GetBlue());
					}
					else if (FormMain->MenuViewShade->Checked)
					{
						BYTE shade = itShajunVert[offset[i]].GetShade();
						d3dVerts[i].colour = D3DCOLOR_XRGB(shade, shade, shade);
					}
					else if (FormMain->MenuViewColour->Checked)
					{
						ShajunRGB rgb = itShajunVert[offset[i]].GetColour();
						d3dVerts[i].colour = D3DCOLOR_XRGB(rgb.GetRed(), rgb.GetGreen(), rgb.GetBlue());
					}
					else if (FormMain->MenuViewShadedColour->Checked)
					{
						ShajunRGB rgb = itShajunVert[offset[i]].GetColour();
						BYTE shade = itShajunVert[offset[i]].GetShade();
						d3dVerts[i].colour = D3DCOLOR_XRGB(shade * rgb.GetRed() >> 8,
														   shade * rgb.GetGreen() >> 8,
														   shade * rgb.GetBlue() >> 8);
					}
				}

				m_Display.GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, d3dVerts, sizeof(LVERTEX));
				itShajunVert++;
			}
			itShajunVert++;
		}
		m_Display.End();
	}
	m_Display.Present();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Mouse event handlers
//---------------------------------------------------------------------------
void __fastcall TForm3D::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		m_CamDistMul *= 1.5f;
		RenderMap();
	}
	else
	{
		m_MousePos = TPoint(X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3D::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		m_CamDistMul /= 1.5f;
		RenderMap();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3D::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	TPoint p, d;
	p = TPoint(X, Y);
	d.x = p.x - m_MousePos.x;
	d.y = p.y - m_MousePos.y;
	m_MousePos = p;

	if (Shift.Contains(ssLeft))
	{
		if (Shift.Contains(ssShift))
		{
			m_CamDistMul += 0.005 * d.y;
			if (m_CamDistMul < 0.5) m_CamDistMul = 0.5;
			else if (m_CamDistMul > 2.0) m_CamDistMul = 2.0;
		}
		else
		{
			m_CamAngle += 0.005 * d.x;
			if (m_CamAngle < 0) m_CamAngle += Shajun::PiMul2;
			else if (m_CamAngle >= Shajun::PiMul2) m_CamAngle -= Shajun::PiMul2;

			m_CamElev += 0.005 * d.y;
			if (m_CamElev < 0) m_CamElev = 0;
			else if (m_CamElev > 1.5) m_CamElev = 1.5;
		}
		RenderMap();
	}
}
//---------------------------------------------------------------------------

