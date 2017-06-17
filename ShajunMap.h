//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#ifndef ShajunMapH
#define ShajunMapH
//---------------------------------------------------------------------------
#include <d3dx9.h>
#include <vector>
#include "ShajunPalette.h"
//---------------------------------------------------------------------------
// ShajunVertex
//---------------------------------------------------------------------------
namespace Shajun
{
    class ShajunVertex
    {
    public:
        ShajunVertex::ShajunVertex() :
            m_Height(0),
            m_Normal(D3DXVECTOR3(0,0,0)),
            m_Shade(0)
        {
        }

        void SetHeight(BYTE Height)         { m_Height = Height; }
        BYTE GetHeight() const              { return m_Height; }

        void SetNormal(D3DXVECTOR3 Normal)  { m_Normal = Normal; }
        D3DXVECTOR3 GetNormal() const       { return m_Normal; }

        void SetShade(BYTE Shade)           { m_Shade = Shade; }
        BYTE GetShade() const               { return m_Shade; }

        void SetColour(ShajunRGB Colour)    { m_Colour = Colour; }
        ShajunRGB GetColour() const         { return m_Colour; }

    private:
        BYTE        m_Height;
        D3DXVECTOR3 m_Normal;
        BYTE        m_Shade;
        ShajunRGB   m_Colour;
    };
}

//---------------------------------------------------------------------------
// ShajunMap
//---------------------------------------------------------------------------
namespace Shajun
{
    class ShajunMap
    {
    public:
        ShajunMap();
		void LoadHeightMap(String FileName);
		void QuantizeFrom(ShajunMap &OtherMap, int NumSteps);
        void CalcNormalMap(float HeightScale);
		void CalcShadeMap(bool Sun, D3DXVECTOR3 SunDirection, float Ambient);
        void CalcColourMap(const ShajunPalette &HeightPalette);

        std::vector<ShajunVertex>::iterator GetBegin() { return m_Vertices.begin(); }
        int GetWidth() const { return m_Width; }
        int GetDepth() const { return m_Depth; }

    private:
        std::vector<ShajunVertex> m_Vertices;
        int m_Width;
        int m_Depth;
    };
}

//---------------------------------------------------------------------------
#endif
