//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#ifndef ShajunPaletteH
#define ShajunPaletteH
//---------------------------------------------------------------------------
#include <vector>
#include "d3d9types.h"
//---------------------------------------------------------------------------
// ShajunRGB
//---------------------------------------------------------------------------
namespace Shajun
{
    class ShajunRGB
    {
    public:
        ShajunRGB() :
            m_Red(0),
            m_Green(0),
            m_Blue(0)
        {
        }

        ShajunRGB(BYTE R, BYTE G, BYTE B) :
            m_Red(R),
            m_Green(G),
            m_Blue(B)
        {
        }

        void SetRed(BYTE Red)       { m_Red = Red; }
        BYTE GetRed() const         { return m_Red; }

        void SetGreen(BYTE Green)   { m_Green = Green; }
        BYTE GetGreen() const       { return m_Green; }

        void SetBlue(BYTE Blue)     { m_Blue = Blue; }
        BYTE GetBlue() const        { return m_Blue; }

    private:
        BYTE m_Red;
        BYTE m_Green;
        BYTE m_Blue;
    };
}

//---------------------------------------------------------------------------
// ShajunPalette
//---------------------------------------------------------------------------
namespace Shajun
{
    class ShajunPalette
    {
    public:
        void Create(int Size);
        void Load(String FileName);

        int GetNumColours() const { return m_Colours.size(); }
        void SetColour(int Index, ShajunRGB Colour);
        ShajunRGB GetColour(int Index) const;

    private:
        std::vector<ShajunRGB> m_Colours;
    };
}

//---------------------------------------------------------------------------
#endif
