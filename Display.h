//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#ifndef DisplayH
#define DisplayH
//---------------------------------------------------------------------------
#include <d3dx9.h>
//---------------------------------------------------------------------------
// Display - basic windowed mode DX wrapper
//---------------------------------------------------------------------------
class Display
{
public:
    Display();
    ~Display();
    bool Create(TForm *Form);
    bool Reset(TForm *Form);
    void ClearZ() const;
    void ClearZRGB(BYTE R, BYTE G, BYTE B) const;
    void SetView(D3DXVECTOR3 Eye, D3DXVECTOR3 LookAt, D3DXVECTOR3 Up) const;
    void SetProjection(float FOV, float Aspect, float NearClip, float FarClip) const;
    bool Begin() const;
    void End() const;
    void Present() const;
    LPDIRECT3DDEVICE9 GetDevice() const { return m_Device; }

private:
    Display(const Display& RHS);            // Disallow copy constructor
    Display& operator=(const Display& RHS); // Disallow assignment operator

    LPDIRECT3D9             m_D3D;
    LPDIRECT3DDEVICE9       m_Device;
    D3DPRESENT_PARAMETERS   m_PP;
};

//---------------------------------------------------------------------------
#endif
