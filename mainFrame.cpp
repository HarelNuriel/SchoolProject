#include "mainFrame.h"
#include "usrWin.h"

BEGIN_EVENT_TABLE(mainFrame, wxFrame)
    EVT_BUTTON(2, mainFrame::loginFunc)
    EVT_BUTTON(5, mainFrame::signinFunc)
END_EVENT_TABLE()

mainFrame::mainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, 1000, title, pos, size)
{
    //The panel is where the controls are placed
    panel = new wxPanel(this, 3);

    //construct the BoxSizer for simple geometrical placement
    vbox = new wxBoxSizer(wxVERTICAL);
    btnBox = new wxBoxSizer(wxHORIZONTAL);

    //Initializing the GridSizer which lays its children in a grid
    fgs = new wxFlexGridSizer(3, 2, 9, 25);

    //Setting the font 
    font = new wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    //Initializing the static text variable usrname to show "Username" and password to show "Password" on the panel
    usrname = new wxStaticText(panel, -1, wxT("Username"), wxDefaultPosition, wxSize(100, 50));
    password = new wxStaticText(panel, -1, wxT("Password"), wxDefaultPosition, wxSize(100, 50));

    //Setting the static texts' font
    usrname->SetFont(*font);
    password->SetFont(*font);

    //Initializing the TextBox in which the user will put the username/password for log/sign in
    usrTextBox = new wxTextCtrl(panel, 109, "", wxDefaultPosition, wxSize(400, 50));
    passTextBox = new wxTextCtrl(panel, 110, "", wxDefaultPosition, wxSize(400, 50), wxTE_PASSWORD | wxOK);

    //Setting the TextBoxs' font
    usrTextBox->SetFont(*font);
    passTextBox->SetFont(*font);

    //Adding the static texts and the text boxes to the grid sizer
    fgs->Add(usrname);
    fgs->Add(usrTextBox);
    fgs->Add(password);
    fgs->Add(passTextBox);

    //Adding the grid sizer to the vertical box for better geometrical placement
    vbox->Add(fgs, 1, wxALL | wxEXPAND, 15);

    //Initializing the Buttons
    loginBtn = new wxButton(panel, 2, "Login");
    signinBtn = new wxButton(panel, 5, "Sign up");

    //Adding the Sign\log in button to the button box
    btnBox->Add(loginBtn);
    btnBox->Add(signinBtn);

    //Adding the button box to the vertical box and aligning it the its place
    vbox->Add(btnBox, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    //Adding the vertical box to the panel for the controls to be placed
    panel->SetSizer(vbox);

    //placing the window in the center of the screen
    Centre();
}

mainFrame::~mainFrame()
{
    // Cleaning up memory
    delete db;
    delete panel;
}

void mainFrame::loginFunc(wxCommandEvent &evt)
{
    // Hashing the username and password
    // Crearting the hash variable
    SHA256* hash = new SHA256();

    // Hashing the username/password
    std::string usrname =  hash->sha256(usrTextBox->GetValue().ToStdString());
    std::string password = hash->sha256(passTextBox->GetValue().ToStdString());

    // Deleting the hash variable
    delete hash;
   
    // Checking if the user exists
    // True - opening a new window
    // False - returning an error message
    if (db->IsUserExists(usrname, password)) {
        window = new userWindow(nullptr, wxT("AES Project"), wxPoint(-1, -1), wxSize(800, 600), usrname, password);
        this->Close();
        window->Show();
    }
    else
        wxMessageBox(wxT("Error: Wrong Username or Password."));
}

void mainFrame::signinFunc(wxCommandEvent& evt)
{
    // Hashing the username and password
    // Crearting the hash variable
    SHA256* hash = new SHA256();

    // Hashing the username/password
    std::string usrname = hash->sha256(usrTextBox->GetValue().ToStdString());
    std::string password = hash->sha256(passTextBox->GetValue().ToStdString());

    // Deleting the hash variable
    delete hash;

    // Checking if the password is valid
    // True - return error message
    if (!IsPasswordValid(passTextBox->GetValue().ToStdString())) {
        wxMessageBox(wxT("Error: Invalid Password."));
        return;
    }
    
    // Checking if the username exists in the database
    // True - trying to addd the username into the table
    // Incase of an exception returning an error message
    // False - returning and error message
    if (!db->IsUserExists(usrname, password)) {
        try {
            db->addAccount(usrname, password);
        }
        catch(int e){
            wxMessageBox(wxT("An Error Has Occured."));
        }
    }
    else
        wxMessageBox(wxT("Error: That username already exists."));
}

bool mainFrame::IsPasswordValid(std::string password) {
    // Checking if the password is valid 
    return (password.length() >= 8);    //return true if the password matches the exp and has length of 8+ chars.
}

//http://zetcode.com/gui/wxwidgets/layoutmanagement/
