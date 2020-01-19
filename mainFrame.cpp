#include "mainFrame.h"

BEGIN_EVENT_TABLE(mainFrame, wxFrame)
    EVT_BUTTON(2, mainFrame::loginFunc)
    EVT_BUTTON(5, mainFrame::signinFunc)
END_EVENT_TABLE()

mainFrame::mainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, 1000, title, pos, size)
{
    wxPanel* panel = new wxPanel(this, 3);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* btnBox = new wxBoxSizer(wxHORIZONTAL);

    wxFlexGridSizer* fgs = new wxFlexGridSizer(3, 2, 9, 25);

    wxFont* font = new wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    wxStaticText* usrname = new wxStaticText(panel, -1, wxT("Username"), wxDefaultPosition, wxSize(100, 50));
    wxStaticText* password = new wxStaticText(panel, -1, wxT("Password"), wxDefaultPosition, wxSize(100, 50));

    usrname->SetFont(*font);
    password->SetFont(*font);

    usrTextBox = new wxTextCtrl(panel, 109, "", wxDefaultPosition, wxSize(400, 50));
    passTextBox = new wxTextCtrl(panel, 110, "", wxDefaultPosition, wxSize(400, 50), wxTE_PASSWORD | wxOK);

    usrTextBox->SetFont(*font);
    passTextBox->SetFont(*font);

    fgs->Add(usrname);
    fgs->Add(usrTextBox);
    fgs->Add(password);
    fgs->Add(passTextBox);

    vbox->Add(fgs, 1, wxALL | wxEXPAND, 15);

    loginBtn = new wxButton(panel, 2, "Login");
    signinBtn = new wxButton(panel, 5, "Signin");

    btnBox->Add(loginBtn);
    btnBox->Add(signinBtn);

    vbox->Add(btnBox, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);
    panel->SetSizer(vbox);

    Centre();
}

mainFrame::~mainFrame()
{
}

void mainFrame::loginFunc(wxCommandEvent &evt)
{

    wxString usrname =  usrTextBox->GetValue();
    wxString password = passTextBox->GetValue();
    
    if (db->IsUserExists(usrname.ToStdString(), password.ToStdString())) {
        usrWin* userWin = new usrWin(nullptr, wxT("AES Project"), wxPoint(-1, -1), wxSize(800, 600));
        this->Close();
        userWin->Show();
    }
    else
        wxMessageBox(wxT("Error: Wrong Username or Password."));
}

void mainFrame::signinFunc(wxCommandEvent& evt)
{
    /*if (!IsPasswordValid(passTextBox->GetValue().ToStdString())) {
        wxMessageBox(wxT("Error: Invalid Password."));
        return;
    }*/
    if (!db->IsUserExists(usrTextBox->GetValue().ToStdString(), passTextBox->GetValue().ToStdString())) {
        try {
            db->addAccount(usrTextBox->GetValue().ToStdString(), passTextBox->GetValue().ToStdString());
        }
        catch(int e){
            wxMessageBox(wxT("An Error Has Occured."));
        }
    }
    else
        wxMessageBox(wxT("Error: That username already exists."));
}

bool mainFrame::IsPasswordValid(std::string password) {
    return (password.length() >= 8);    //return true if the password matches the exp and has length of 8+ chars.
}

//http://zetcode.com/gui/wxwidgets/layoutmanagement/
