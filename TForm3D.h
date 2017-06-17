//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#ifndef TForm3DH
#define TForm3DH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include "Display.h"
#include "ShajunMap.h"
//---------------------------------------------------------------------------
class TForm3D : public TForm
{
__published:
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private:
    Display m_Display;
    bool    m_D3DOK;
    TPoint  m_MousePos;
    float   m_CamAngle;
    float   m_CamElev;
    float   m_CamDistMul;

public:
    __fastcall TForm3D(TComponent* Owner);
    void RenderMap();
};

//---------------------------------------------------------------------------
extern PACKAGE TForm3D *Form3D;
//---------------------------------------------------------------------------
#endif
