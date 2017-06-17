//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ShajunUtil.h"
//---------------------------------------------------------------------------
using Shajun::ShajunRGB;
//---------------------------------------------------------------------------
BYTE Shajun::NormalFloatToByte(float Value) // Scales from (-1 to 1) to (0 to 255)
{
    Value = (Value + 1) * 128;

    if (Value < 0)         return 0;
    else if (Value >= 255) return 255;
    else                   return Value;
}
//---------------------------------------------------------------------------
ShajunRGB Shajun::NormalToRGB(D3DXVECTOR3 Normal)
{
    return ShajunRGB(NormalFloatToByte(Normal.x),
                     NormalFloatToByte(Normal.y),
                     NormalFloatToByte(Normal.z));
}
//---------------------------------------------------------------------------
void Shajun::NormalFromPts(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 &Normal)
{
	D3DXVECTOR3 ab, ac, cross;
	ab = B - A;
	ac = C - A;
	D3DXVec3Cross(&cross, &ab, &ac);
	D3DXVec3Normalize(&Normal, &cross);
}
//---------------------------------------------------------------------------
BYTE Shajun::QuantizeHeight(BYTE InputHeight, int NumSteps)
{
	int f = 256 / NumSteps;
	InputHeight /= f;
	InputHeight *= f;
	return InputHeight;
}
//---------------------------------------------------------------------------

