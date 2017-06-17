//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ShajunPalette.h"
//---------------------------------------------------------------------------
using Shajun::ShajunRGB;
using Shajun::ShajunPalette;
//---------------------------------------------------------------------------
void ShajunPalette::Create(int Size)
{
    m_Colours.clear();
    m_Colours.resize(Size);
}
//---------------------------------------------------------------------------
void ShajunPalette::Load(String FileName)
{
    // Load bitmap + create array

    std::auto_ptr<Graphics::TBitmap> bm(new Graphics::TBitmap);
    bm->LoadFromFile(FileName);
    bm->PixelFormat = pf24bit;

    m_Colours.clear();
    m_Colours.resize(bm->Width);

    // Load colours from the bitmap's top row

    BYTE *pixel = reinterpret_cast<BYTE*>(bm->ScanLine[0]);

    for (int i = 0; i < bm->Width; i++)
    {
        m_Colours[i] = ShajunRGB(pixel[2], pixel[1], pixel[0]);
        pixel += 3;
    }
}
//---------------------------------------------------------------------------
void ShajunPalette::SetColour(int Index, ShajunRGB Colour)
{
    if ((Index < 0) || (Index >= GetNumColours())) return;
    m_Colours[Index] = Colour;
}
//---------------------------------------------------------------------------
ShajunRGB ShajunPalette::GetColour(int Index) const
{
    if ((Index < 0) || (Index >= GetNumColours())) return ShajunRGB();
    return m_Colours[Index];
}
//---------------------------------------------------------------------------

