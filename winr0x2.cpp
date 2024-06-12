//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
USEFORM("wrUnit.cpp", WRForm);
USEFORM("rebUnit.cpp", RebootForm);

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
        	Application->Initialize();
		Application->CreateForm(__classid(TWRForm), &WRForm);
		Application->CreateForm(__classid(TRebootForm), &RebootForm);
		Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
