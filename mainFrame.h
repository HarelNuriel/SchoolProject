#pragma once

#include "wx/wx.h"
#include "usrWin.h"

class mainFrame : public wxFrame // MainFrame is the class for our window,
{ // It contains the window and all objects in it
public:
    mainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~mainFrame();
    wxTextCtrl* usrTextBox;
    wxTextCtrl* passTextBox;
    wxButton* loginBtn;
    wxButton* signinBtn;

    void loginFunc(wxCommandEvent &evt);

    DECLARE_EVENT_TABLE()
};
