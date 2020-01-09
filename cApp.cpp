#include "cApp.h"


IMPLEMENT_APP(cApp) // Initializes the MainApp class and tells our program
// to run it
bool cApp::OnInit()
{
    mainFrame* mainWin = new mainFrame(wxT("AES Project"), wxPoint(-1, -1),
        wxSize(800, 600)); // Create an instance of our frame, or window

    mainWin->Show(TRUE); // show the window
    SetTopWindow(mainWin);// and finally, set it as the main window

    return TRUE;
}
