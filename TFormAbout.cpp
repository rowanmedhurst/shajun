//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAbout.h"
#include "TFormMain.h"
#include "TForm3D.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAbout *FormAbout;
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    FormMain->Enabled = true;
    Form3D->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::BtnOKClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------


