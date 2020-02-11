#include "updateWin.h"
#include "usrWin.h"

BEGIN_EVENT_TABLE(updateWin, wxFrame)
    EVT_BUTTON(10, updateWin::updateUser)
    EVT_BUTTON(11, updateWin::updatePassword)
    EVT_BUTTON(12, updateWin::back)
END_EVENT_TABLE()

updateWin::updateWin(wxWindow* win, const wxString& title, const wxPoint& pos, const wxSize& size, std::string hashedUser, std::string hashedPassword)
	: wxFrame(win, 1001, title, pos, size)
{
    //The panel is where the controls are placed
    panel = new wxPanel(this, 3);

    //construct the BoxSizer for simple geometrical placement
    vbox = new wxBoxSizer(wxVERTICAL);
    btnBoxR = new wxBoxSizer(wxHORIZONTAL);
    btnBoxL = new wxBoxSizer(wxHORIZONTAL);

    //Initializing the GridSizer which lays its children in a grid
    fgs = new wxFlexGridSizer(3, 2, 9, 25);

    //Setting the font 
    font = new wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    //Initializing the static text variable Path to show "Path" on the panel
    previous = new wxStaticText(panel, -1, wxT("Previous"), wxDefaultPosition, wxSize(100, 50));
    updated = new wxStaticText(panel, -1, wxT("New"), wxDefaultPosition, wxSize(100, 50));

    //Setting the path's font
    previous->SetFont(*font);
    updated->SetFont(*font);

    //Initializing the TextBox in which the user will put the path for En/Decryption
    preTextBox = new wxTextCtrl(panel, 109, "", wxDefaultPosition, wxSize(400, 50));
    newTextBox = new wxTextCtrl(panel, 110, "", wxDefaultPosition, wxSize(400, 50));

    //Setting the TextBox's font
    preTextBox->SetFont(*font);
    newTextBox->SetFont(*font);

    //Adding the static text and the text box to the grid sizer
    fgs->Add(previous);
    fgs->Add(preTextBox);
    fgs->Add(updated);
    fgs->Add(newTextBox);

    //Adding the grid sizer to the vertical box for better geometrical placement
    vbox->Add(fgs, 1, wxALL | wxEXPAND, 15);

    //Initializing the Buttons
    updateUsr = new wxButton(panel, 10, "Update Username");
    updatePasswordBtn = new wxButton(panel, 11, "Update Password");
    backBtn = new wxButton(panel, 12, "Back");

    //Adding the Sign out button to the left button box
    btnBoxL->Add(backBtn);

    //Adding the left button box to the vertical box and aligning it to its place
    vbox->Add(btnBoxL, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 10);

    //Adding the En/Decryption buttons to the right button box
    btnBoxR->Add(updateUsr);
    btnBoxR->Add(updatePasswordBtn);

    //Adding the right button box to the vertical box and aligning it the its place
    vbox->Add(btnBoxR, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    //Adding the vertical box to the panel for the controls to be placed
    panel->SetSizer(vbox);

    //placing the window in the center of the screen
    Centre();

    Username = hashedUser;
    password = hashedPassword;
}

updateWin::~updateWin()
{
}

void updateWin::back(wxCommandEvent& evt)
{
    userWindow* usrWin = new userWindow(nullptr, wxT("AES Project"), wxPoint(-1, -1), wxSize(800, 600), Username, password);
    this->Close();
    usrWin->Show();
}

void updateWin::updateUser(wxCommandEvent& evt)
{
    SHA256* Hash = new SHA256();

    std::string prevUsrname = Hash->sha256(preTextBox->GetValue().ToStdString());

    if (prevUsrname != Username) {
        wxMessageBox(wxT("The previous username does not match the corrent one."));
        return;
    }

    std::string newUsrname = Hash->sha256(newTextBox->GetValue().ToStdString());

    delete Hash;

    char** paths = db->updateUsername(newUsrname, password, prevUsrname);
    int count = db->pathCount(Username);
    std::string key = "";

    //Getting the key for the decryption
    for (int i = 0; i < KEY_SIZE; i++) {
        key += Username[i] ^ password[i];
    }

    //Decrypting
    decrypt(paths, count, key);

    //reseting the key
    key = "";

    //Getting the key for the encryption
    for (int i = 0; i < KEY_SIZE; i++) {
        key += newUsrname[i] ^ password[i];
    }

    //Encrypting
    encrypt(paths, count, key);

    wxMessageBox(wxT("Successfully updated your username."));
    //TODO - decrypt all the files and encrypt them using the new username.
}

void updateWin::updatePassword(wxCommandEvent& evt)
{
    SHA256* Hash = new SHA256();

    std::string prevPassword = Hash->sha256(preTextBox->GetValue().ToStdString());

    if (prevPassword != password) {
        wxMessageBox(wxT("The previous username does not match the corrent one."));
        return;
    }

    std::string newPassword = Hash->sha256(newTextBox->GetValue().ToStdString());

    delete Hash;

    char** paths = db->updatePassword(Username, password, prevPassword);
    int count = db->pathCount(Username);
    std::string key = "";

    //Getting the key for the decryption
    for (int i = 0; i < KEY_SIZE; i++) {
        key += Username[i] ^ password[i];
    }

    //Decrypting
    decrypt(paths, count, key);

    //reseting the key
    key = "";

    //Getting the key for the encryption
    for (int i = 0; i < KEY_SIZE; i++) {
        key += Username[i] ^ newPassword[i];
    }

    //Encrypting
    encrypt(paths, count, key);

    wxMessageBox(wxT("Successfully updated your username."));
}

void updateWin::decrypt(char** paths, int pathCount, std::string key)
{
    int fileSize = 0;
    std::ofstream fwriter;
    std::ifstream freader;
    AES128* AES = new AES128();
    std::string data;
    std::string decryptedData;

    for (int i = 0; i < pathCount; i++) {

        fileSize = std::filesystem::file_size(paths[i]);

        if (std::filesystem::is_directory(paths[i])) {
            for (auto& dir : std::filesystem::directory_iterator(paths[i])) {
                if (!std::filesystem::is_directory(dir)) {
                    freader.open(dir.path());
                    freader >> data;
                    freader.close();
                    decryptedData += AES->AES_Decrypt(data, key);
                    fwriter.open(dir.path());
                    fwriter << decryptedData;
                    fwriter.close();
                }
            }
        }
        else {
            freader.open(paths[i]);
            freader >> data;
            freader.close();
            decryptedData = AES->AES_Decrypt(data, key);
            fwriter.open(paths[i]);
            fwriter << data;
            fwriter.close();
        }
    }

    delete AES;
}

void updateWin::encrypt(char** paths, int pathCount, std::string key)
{
    int fileSize = 0;
    std::ofstream fwriter;
    std::ifstream freader;
    AES128* AES = new AES128();
    std::string data;
    std::string decryptedData;

    for (int i = 0; i < pathCount; i++) {

        fileSize = std::filesystem::file_size(paths[i]);

        if (std::filesystem::is_directory(paths[i])) {
            for (auto& dir : std::filesystem::directory_iterator(paths[i])) {
                if (!std::filesystem::is_directory(dir)) {
                    freader.open(dir.path());
                    freader >> data;
                    freader.close();
                    decryptedData += AES->AES_Encrypt(data, key);
                    fwriter.open(dir.path());
                    fwriter << decryptedData;
                    fwriter.close();
                }
            }
        }
        else {
            freader.open(paths[i]);
            freader >> data;
            freader.close();
            decryptedData = AES->AES_Encrypt(data, key);
            fwriter.open(paths[i]);
            fwriter << data;
            fwriter.close();
        }
    }
    
    delete AES;
}
