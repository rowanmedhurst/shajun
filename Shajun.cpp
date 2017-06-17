//---------------------------------------------------------------------------
//                                 SHAJUN
//         Copyright (c) 2004 Rowan Medhurst. All Rights Reserved.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("TFormMain.cpp", FormMain);
USEFORM("TForm3D.cpp", Form3D);
USEFORM("TFormAbout.cpp", FormAbout);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
		Application->Initialize();
		Application->Title = "Shajun";
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TForm3D), &Form3D);
		Application->CreateForm(__classid(TFormAbout), &FormAbout);
		Application->Run();
    }
    catch (Exception &exception)
    {
		Application->ShowException(&exception);
    }
	return 0;
}
//---------------------------------------------------------------------------
