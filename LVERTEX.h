//---------------------------------------------------------------------------
#ifndef LVERTEXH
#define LVERTEXH
//---------------------------------------------------------------------------
struct LVERTEX
{
	LVERTEX()
    {
	}

	LVERTEX(float _x, float _y, float _z, DWORD _colour)
    {
		x = _x;
		y = _y;
		z = _z;
		colour = _colour;
	}

    float x, y, z;
	DWORD colour;
};

#define FVF_LVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//---------------------------------------------------------------------------
#endif