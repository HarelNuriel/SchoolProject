#pragma once

#include "wx/wx.h"
#include "SQLClass.h"
#include "sha256.h"
#include <regex>

class userWindow;

class mainFrame : public wxFrame    // MainFrame is the class for our window,
{                                   // It contains the window and all objects in it
public:
    mainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~mainFrame();

    wxTextCtrl* usrTextBox;
    wxTextCtrl* passTextBox;
    wxButton* loginBtn;
    wxButton* signinBtn;
    userWindow* window;
    wxPanel* panel;
    wxBoxSizer* vbox;
    wxBoxSizer* btnBox;
    wxFlexGridSizer* fgs;
    wxFont* font;
    wxStaticText* usrname;
    wxStaticText* password;

    void loginFunc(wxCommandEvent &evt);
    void signinFunc(wxCommandEvent &evt);

    DECLARE_EVENT_TABLE()

private:
    bool IsPasswordValid(std::string password);
    SQLClass* db = new SQLClass();
};

