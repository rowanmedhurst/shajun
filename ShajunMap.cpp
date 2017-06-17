//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ShajunMap.h"
#include "ShajunUtil.h"
#include "LVERTEX.h"
//---------------------------------------------------------------------------
using std::vector;
using Shajun::ShajunMap;
using Shajun::ShajunVertex;
using Shajun::ShajunPalette;
//---------------------------------------------------------------------------
ShajunMap::ShajunMap()
:
    m_Width(0),
    m_Depth(0)
{
}
//---------------------------------------------------------------------------
void ShajunMap::LoadHeightMap(String FileName)
{
    // Load bitmap

    std::auto_ptr<Graphics::TBitmap> bm(new Graphics::TBitmap);
    bm->LoadFromFile(FileName);
    bm->PixelFormat = pf24bit;

    // Create array

    m_Width = bm->Width;
    m_Depth = bm->Height;
	m_Vertices.resize(m_Width * m_Depth);

	// Copy the height data

	vector<ShajunVertex>::iterator it = m_Vertices.begin();

	for (int z = 0; z < m_Depth; z++)
	{
		BYTE *pixel = reinterpret_cast<BYTE*>(bm->ScanLine[m_Depth - 1 - z]);

		for (int x = 0; x < m_Width; x++)
		{
			BYTE h = (pixel[0] + pixel[1] + pixel[2]) / 3;
			it->SetHeight(h);

			it++;
			pixel += 3;
		}
	}
}
//---------------------------------------------------------------------------
void ShajunMap::QuantizeFrom(ShajunMap &OtherMap, int NumSteps)
{
	m_Width = OtherMap.GetWidth();
	m_Depth = OtherMap.GetDepth();
	m_Vertices.resize(m_Width * m_Depth);

	vector<ShajunVertex>::iterator vertex = m_Vertices.begin();
	vector<ShajunVertex>::iterator otherVertex = OtherMap.GetBegin();

	for (int z = 0; z < m_Depth; z++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			vertex->SetHeight(QuantizeHeight(otherVertex->GetHeight(), NumSteps));
			vertex++;
			otherVertex++;
		}
	}
}
//---------------------------------------------------------------------------
void ShajunMap::CalcNormalMap(float HeightScale)
{
	const float hsDiv255 = HeightScale / 255.0f;
	vector<ShajunVertex>::iterator it = m_Vertices.begin();

    for (int z = 0; z < m_Depth; z++)
    {
        for (int x = 0; x < m_Width; x++)
        {
            // Determine which of our 4-neighbours we actually have
            // (can be 2 or 3 if on a corner or edge)

            bool left  = x > 0;
            bool right = x < m_Width - 1;
            bool back  = z > 0;
            bool fwd   = z < m_Depth - 1;

            // Get actual 3D locations of this vertex and its neighbours

            D3DXVECTOR3 p, pLeft, pRight, pBack, pFwd;

            p.y = hsDiv255 * it->GetHeight();
            if (left)  pLeft.y  = hsDiv255 * (it - 1)->GetHeight();
            if (right) pRight.y = hsDiv255 * (it + 1)->GetHeight();
            if (back)  pBack.y  = hsDiv255 * (it - m_Width)->GetHeight();
            if (fwd)   pFwd.y   = hsDiv255 * (it + m_Width)->GetHeight();

            p.x = pBack.x = pFwd.x = x;
            pLeft.x = x - 1;
            pRight.x = x + 1;

            p.z = pLeft.z = pRight.z = z;
            pBack.z = z - 1;
            pFwd.z = z + 1;

            // Calculate the 'average' normal

            D3DXVECTOR3 tmpNormal, sumNormal(0,0,0);

            if (left && back)
            {
                Shajun::NormalFromPts(p, pBack, pLeft, tmpNormal);
                sumNormal += tmpNormal;
            }
            if (left && fwd)
            {
                Shajun::NormalFromPts(p, pLeft, pFwd, tmpNormal);
                sumNormal += tmpNormal;
            }
            if (right && fwd)
            {
                Shajun::NormalFromPts(p, pFwd, pRight, tmpNormal);
                sumNormal += tmpNormal;
            }
            if (right && back)
            {
                Shajun::NormalFromPts(p, pRight, pBack, tmpNormal);
                sumNormal += tmpNormal;
            }

            D3DXVec3Normalize(&tmpNormal, &sumNormal);
            if (tmpNormal.y < -1) tmpNormal.y = -1;
            else if (tmpNormal.y > 1) tmpNormal.y = 1;
            it->SetNormal(tmpNormal);

            it++;
        }
    }
}
//---------------------------------------------------------------------------
void ShajunMap::CalcShadeMap(bool Sun, D3DXVECTOR3 SunDirection, float Ambient)
{
    vector<ShajunVertex>::iterator it = m_Vertices.begin();

    for (int z = 0; z < m_Depth; z++)
    {
        for (int x = 0; x < m_Width; x++)
        {
			float shade = Ambient;

			if (Sun)
			{
				D3DXVECTOR3 norm = it->GetNormal();
				float dot = D3DXVec3Dot(&norm, &SunDirection);
				if (dot < 0)
				{
					shade += (1 - Ambient) * -dot;
				}
			}

            if (shade < 0)       it->SetShade(0);
            else if (shade >= 1) it->SetShade(255);
            else                 it->SetShade(256 * shade);

            it++;
        }
    }
}
//---------------------------------------------------------------------------
void ShajunMap::CalcColourMap(const ShajunPalette &HeightPalette)
{
    vector<ShajunVertex>::iterator it = m_Vertices.begin();

    for (int z = 0; z < m_Depth; z++)
    {
        for (int x = 0; x < m_Width; x++)
        {
            int palIndex = HeightPalette.GetNumColours() * it->GetHeight() / 256;
            it->SetColour(HeightPalette.GetColour(palIndex));

            it++;
        }
    }
}
//---------------------------------------------------------------------------

