#include "usrWin.h"
#include "mainFrame.h"
#include "updateWin.h"

BEGIN_EVENT_TABLE(userWindow, wxFrame)
    EVT_BUTTON(10, userWindow::encryptFile)
    EVT_BUTTON(11, userWindow::decryptFile)
    EVT_BUTTON(12, userWindow::back)
    EVT_BUTTON(13, userWindow::updateFunc)
END_EVENT_TABLE()

userWindow::userWindow(wxWindow* win, const wxString& title, const wxPoint& pos, const wxSize& size, std::string hashedUser, std::string hashedPassword)
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
    path = new wxStaticText(panel, -1, wxT("Path"), wxDefaultPosition, wxSize(100, 50));

    //Setting the path's font
    path->SetFont(*font);

    //Initializing the TextBox in which the user will put the path for En/Decryption
    pathTextBox = new wxTextCtrl(panel, 109, "", wxDefaultPosition, wxSize(400, 50));

    //Setting the TextBox's font
    pathTextBox->SetFont(*font);

    //Adding the static text and the text box to the grid sizer
    fgs->Add(path);
    fgs->Add(pathTextBox);

    //Adding the grid sizer to the vertical box for better geometrical placement
    vbox->Add(fgs, 1, wxALL | wxEXPAND, 15);

    //Initializing the Buttons
    encryptBtn = new wxButton(panel, 10, "Encrypt");
    decryptBtn = new wxButton(panel, 11, "Decrypt");
    backBtn = new wxButton(panel, 12, "Sign out");
    updateBtn = new wxButton(panel, 13, "Update");

    //Adding the Sign out button to the left button box
    btnBoxL->Add(backBtn);

    //Adding the left button box to the vertical box and aligning it to its place
    vbox->Add(btnBoxL, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 10);

    //Adding the En/Decryption buttons to the right button box
    btnBoxR->Add(updateBtn);
    btnBoxR->Add(encryptBtn);
    btnBoxR->Add(decryptBtn);

    //Adding the right button box to the vertical box and aligning it the its place
    vbox->Add(btnBoxR, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    //Adding the vertical box to the panel for the controls to be placed
    panel->SetSizer(vbox);

    //placing the window in the center of the screen
    Centre();

    Username = hashedUser;
    password = hashedPassword;
}

userWindow::~userWindow()
{
    delete db;
    delete panel;
}

void userWindow::back(wxCommandEvent& evt)
{
    mainFrame* mainWin  = new mainFrame(wxT("AES Project"), wxPoint(-1, -1), wxSize(800, 600));
    this->Close();
    mainWin->Show();
}

void userWindow::updateFunc(wxCommandEvent& evt)
{
    update = new updateWin(nullptr, wxT("AES Project"), wxPoint(-1, -1), wxSize(800, 600), Username, password);
    this->Close();
    update->Show();
}

void userWindow::decryptFile(wxCommandEvent& evt)
{
    std::regex exp("^.:(\\.+)*(\..+)*");
    std::string path = pathTextBox->GetValue().ToStdString();
    

    if (!std::regex_match(path, exp)) {
        wxMessageBox(wxT("Error: Invalid Path."));
        return;
    }

    //checking if the path exists (path encrypted) in the data base
    if (!db->IsPathExists(pathTextBox->GetValue().ToStdString(), Username)) {
        wxMessageBox(wxT("Error: Path Does Not exist."));
        return;
    }
    else {
        db->removePath(Username, pathTextBox->GetValue().ToStdString());
    }

    std::filesystem::file_status status = std::filesystem::file_status{};

    if (!(std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(path))) {
        wxMessageBox(wxT("Error: Path not exists."));
        return;
    }

    const int fileSize = std::filesystem::file_size(path);
    std::ofstream fwriter;
    std::ifstream freader;
    AES128* decrypt = new AES128();
    std::string data;
    char key [KEY_SIZE];
    std::string decryptedData;

    GetKey(key);

    if (std::filesystem::is_directory(path)) {
        for (auto& dir : std::filesystem::directory_iterator(path)) {
            if (!std::filesystem::is_directory(dir)) {
                freader.open(dir.path());
                freader >> data;
                freader.close();
                decryptedData += decrypt->AES_Decrypt(data, key);
                fwriter.open(dir.path());
                fwriter << decryptedData;
                fwriter.close();
            }
        }
        wxMessageBox(wxT("Successfully Decrypted The Files"));
    }
    else {
        freader.open(path);
        freader >> data;
        freader.close();
        decryptedData = decrypt->AES_Decrypt(data, key);
        fwriter.open(path);
        fwriter << data;
        fwriter.close();
        wxMessageBox(wxT("Successfully Decrypted The File"));
    }

    delete decrypt;
}

void userWindow::encryptFile(wxCommandEvent& evt)
{
    std::regex exp("^.:(\\.+)*(\..+)*");
    std::string path = pathTextBox->GetValue().ToStdString();


    if (!std::regex_match(path, exp)) {
        wxMessageBox(wxT("Error: Invalid Path."));
        return;
    }

    if (db->IsPathExists(pathTextBox->GetValue().ToStdString(), Username)) {
        wxMessageBox(wxT("Error: Path already exists."));
        return;
    }
    else {
        db->addPath(Username, pathTextBox->GetValue().ToStdString());
    }

    std::filesystem::file_status status = std::filesystem::file_status{};

    if (!(std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(path))) {
        wxMessageBox(wxT("Error: Path not exists on the computer."));
        return;
    }

    const int fileSize = std::filesystem::file_size(path);
    std::ofstream fwriter;
    std::ifstream freader;
    AES128* encrypt = new AES128();
    char key [KEY_SIZE];
    std::string data;
    std::string EncryptedData;

    GetKey(key);
    
    if (std::filesystem::is_directory(path)) {
        for (auto& dir : std::filesystem::directory_iterator(path)) {
            if (!std::filesystem::is_directory(dir)) {
                freader.open(dir.path());
                freader >> data;
                freader.close();
                EncryptedData = encrypt->AES_Encrypt(data, key);
                fwriter.open(dir.path());
                fwriter << EncryptedData;
                fwriter.close();
                data = "";
            }
        }
        wxMessageBox(wxT("Successfully Encrypted The Files"));
    }
    else {
        freader.open(path);
        freader >> data;
        freader.close();
        EncryptedData = encrypt->AES_Encrypt(data, key);
        fwriter.open(path);
        fwriter << data;
        fwriter.close();
        wxMessageBox(wxT("Successfully Encrypted The File"));
    }
}

void userWindow::GetKey(char* key) {
    //creating the key for en\decryption
    for (int i = 0; i < 16; i++) {
        key[i] += Username[i] ^ password[i];
    }
}
