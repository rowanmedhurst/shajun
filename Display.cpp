//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Display.h"
#include "dxutil.h"
//---------------------------------------------------------------------------
Display::Display() :
    m_D3D(NULL),
    m_Device(NULL)
{
    ZeroMemory(&m_PP, sizeof(m_PP));
}
//---------------------------------------------------------------------------
Display::~Display()
{
    SAFE_RELEASE(m_Device)
    SAFE_RELEASE(m_D3D)
}
//---------------------------------------------------------------------------
bool Display::Create(TForm *Form)
{
    // Create the D3D object

    m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (m_D3D == NULL)
    {
        return false;
    }

    // Set up the structure used to create the D3DDevice

    m_PP.Windowed = true;
    m_PP.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_PP.BackBufferFormat = D3DFMT_UNKNOWN;
    m_PP.EnableAutoDepthStencil = true;
    m_PP.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice

    if (FAILED(m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Form->Handle,
                                   D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                   &m_PP, &m_Device)))
    {
        return false;
    }

    // Turn on the z-buffer

    m_Device->SetRenderState(D3DRS_ZENABLE, true);

    // ----
    return true;
}
//---------------------------------------------------------------------------
bool Display::Reset(TForm *Form)
{
    m_PP.BackBufferWidth = Form->ClientWidth;
    m_PP.BackBufferHeight = Form->ClientHeight;

    if (FAILED(m_Device->Reset(&m_PP))) return false;
    return true;
}
//---------------------------------------------------------------------------
void Display::ClearZ() const
{
    m_Device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1, 0);
}
//---------------------------------------------------------------------------
void Display::ClearZRGB(BYTE R, BYTE G, BYTE B) const
{
    m_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(R, G, B), 1, 0);
}
//---------------------------------------------------------------------------
void Display::SetView(D3DXVECTOR3 Eye, D3DXVECTOR3 LookAt, D3DXVECTOR3 Up) const
{
    D3DXMATRIX mat;
    D3DXMatrixLookAtLH(&mat, &Eye, &LookAt, &Up);
    m_Device->SetTransform(D3DTS_VIEW, &mat);
}
//---------------------------------------------------------------------------
void Display::SetProjection(float FOV, float Aspect, float NearClip, float FarClip) const
{
    D3DXMATRIX mat;
    D3DXMatrixPerspectiveFovLH(&mat, FOV, Aspect, NearClip, FarClip);
    m_Device->SetTransform(D3DTS_PROJECTION, &mat);
}
//---------------------------------------------------------------------------
bool Display::Begin() const
{
    if (FAILED(m_Device->BeginScene())) return false;
    return true;
}
//---------------------------------------------------------------------------
void Display::End() const
{
    m_Device->EndScene();
}
//---------------------------------------------------------------------------
void Display::Present() const
{
    m_Device->Present(NULL, NULL, NULL, NULL);
}
//---------------------------------------------------------------------------

