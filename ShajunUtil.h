//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#ifndef ShajunUtilH
#define ShajunUtilH
#include <d3dx9.h>
#include "ShajunPalette.h"
//---------------------------------------------------------------------------
namespace Shajun
{
    const float Pi          = 3.14159265359f;
    const float PiMul2      = 6.28318530718f;
    const float PiDiv180    = 0.01745329252f;

    BYTE        NormalFloatToByte(float Value);
    ShajunRGB   NormalToRGB(D3DXVECTOR3 Normal);
	void        NormalFromPts(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 &Normal);

	BYTE		QuantizeHeight(BYTE InputHeight, int NumSteps);
}

//---------------------------------------------------------------------------
#endif
