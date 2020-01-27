#include "usrWin.h"
#include "mainFrame.h"

BEGIN_EVENT_TABLE(userWindow, wxFrame)
    EVT_BUTTON(2, userWindow::encryptFile)
    EVT_BUTTON(5, userWindow::decryptFile)
    EVT_BUTTON(6, userWindow::back)
END_EVENT_TABLE()

userWindow::userWindow(wxWindow* win, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(win, 1001, title, pos, size)
{
    panel = new wxPanel(this, 3);

    vbox = new wxBoxSizer(wxVERTICAL);
    btnBoxR = new wxBoxSizer(wxHORIZONTAL);
    btnBoxL = new wxBoxSizer(wxHORIZONTAL);

    fgs = new wxFlexGridSizer(3, 2, 9, 25);

    font = new wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    path = new wxStaticText(panel, -1, wxT("Path"), wxDefaultPosition, wxSize(100, 50));

    path->SetFont(*font);

    pathTextBox = new wxTextCtrl(panel, 109, "", wxDefaultPosition, wxSize(400, 50));

    pathTextBox->SetFont(*font);

    fgs->Add(path);
    fgs->Add(pathTextBox);

    vbox->Add(fgs, 1, wxALL | wxEXPAND, 15);

    encryptBtn = new wxButton(panel, 2, "Encrypt");
    decryptBtn = new wxButton(panel, 5, "Decrypt");
    backBtn = new wxButton(panel, 6, "Sign out");

    btnBoxL->Add(backBtn);

    vbox->Add(btnBoxL, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 10);

    btnBoxR->Add(encryptBtn);
    btnBoxR->Add(decryptBtn);

    vbox->Add(btnBoxR, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);
    panel->SetSizer(vbox);

    Centre();
}

userWindow::~userWindow()
{
    delete pathTextBox;
    delete backBtn;
    delete encryptBtn;
    delete decryptBtn;
    delete vbox;
    delete btnBoxR;
    delete btnBoxL;
    delete fgs;
    delete font;
    delete path;
    delete panel;
}

void userWindow::back(wxCommandEvent& evt)
{
    mainFrame* mainWin  = new mainFrame(wxT("AES Project"), wxPoint(-1, -1), wxSize(800, 600));
    this->Close();
    mainWin->Show();
}

void userWindow::decryptFile(wxCommandEvent& evt)
{
    std::string path = pathTextBox->GetValue().ToStdString();
    std::filesystem::file_status status = std::filesystem::file_status{};

    if (!(std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(path))) {
        wxMessageBox(wxT("Error: Path not exists."));
        return;
    }

    const int fileSize = std::filesystem::file_size(path);
    std::ofstream fwriter;
    std::ifstream freader;
    char* data = new char[fileSize];

    if (std::filesystem::is_directory(path)) {
        for (auto& dir : std::filesystem::directory_iterator(path)) {
            if (!std::filesystem::is_directory(dir)) {
                freader.open(dir.path());
                freader >> data;
                freader.close();
                //Decrypt data
                fwriter.open(dir.path());
                fwriter << data;
                fwriter.close();
            }
        }
        wxMessageBox(wxT("Successfully Encrypted The Files"));
    }
    else {
        freader.open(path);
        freader >> data;
        freader.close();
        //Decrypt data
        fwriter.open(path);
        fwriter << data;
        fwriter.close();
        wxMessageBox(wxT("Successfully Encrypted The File"));
    }

    delete[] data;
}

void userWindow::encryptFile(wxCommandEvent& evt)
{
    std::string path = pathTextBox->GetValue().ToStdString();
    std::filesystem::file_status status = std::filesystem::file_status{};

    if (!(std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(path))) {
        wxMessageBox(wxT("Error: Path not exists."));
        return;
    }

    const int fileSize = std::filesystem::file_size(path);
    std::ofstream fwriter;
    std::ifstream freader;
    char* data = new char[fileSize];
    
    if (std::filesystem::is_directory(path)) {
        for (auto& dir : std::filesystem::directory_iterator(path)) {
            if (!std::filesystem::is_directory(dir)) {
                freader.open(dir.path());
                freader >> data;
                freader.close();
                //Encrypt data
                fwriter.open(dir.path());
                fwriter << data;
                fwriter.close();
            }
        }
        wxMessageBox(wxT("Successfully Encrypted The Files"));
    }
    else {
        freader.open(path);
        freader >> data;
        freader.close();
        //Encrypt data
        fwriter.open(path);
        fwriter << data;
        fwriter.close();
        wxMessageBox(wxT("Successfully Encrypted The File"));
    }
}

bool userWindow::IsPathValid(std::string path)
{
    std::ifstream test(path);
    return (test) ? true : false;
}
