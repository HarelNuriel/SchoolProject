#include "updateWin.h"
#include "usrWin.h"

BEGIN_EVENT_TABLE(updateWin, wxFrame)
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
    updatePasswordBtn = new wxButton(panel, 11, "Update");
    backBtn = new wxButton(panel, 12, "Back");

    //Adding the Sign out button to the left button box
    btnBoxL->Add(backBtn);

    //Adding the left button box to the vertical box and aligning it to its place
    vbox->Add(btnBoxL, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 10);

    //Adding the En/Decryption buttons to the right button box
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
    // Creating a new usrWindow, opening it and closing this window.
    userWindow* usrWin = new userWindow(nullptr, wxT("AES Project"), wxPoint(-1, -1), wxSize(800, 600), Username, password);
    this->Close();
    usrWin->Show();
}

void updateWin::updatePassword(wxCommandEvent& evt)
{
    // Declaring a hash variable to encrypt the username/password using SHA256
    SHA256* Hash = new SHA256();

    // Taking the value from the 'previous' textbox and hashing it
    std::string prevPassword = Hash->sha256(preTextBox->GetValue().ToStdString());

    // Checking if the input old password matches the current one
    if (prevPassword != password) {
        wxMessageBox(wxT("The previous username does not match the corrent one."));
        return;
    }

    // Taking the value from the 'new' textbox and hashing it
    std::string newPassword = Hash->sha256(newTextBox->GetValue().ToStdString());

    // Deleteing the hash variable to free the heap memory
    delete Hash;

    // Declaring 2 vectors for the keys and a vector for the paths and giving it the paths
    std::vector<std::string> paths = db->updatePassword(Username, newPassword);
    std::vector<unsigned char> oldKey;
    std::vector<unsigned char> newKey;

    // Getting the key for the decryption
    for (int i = 0; i < KEY_SIZE; i++) {
        oldKey.push_back(Username[i] ^ prevPassword[i]);
    }

    // Decrypting
    decrypt(paths, oldKey);

    // Getting the key for the encryption
    for (int i = 0; i < KEY_SIZE; i++) {
        newKey.push_back(Username[i] ^ newPassword[i]);
    }

    // Encrypting
    encrypt(paths, newKey);

    // Updating the current password
    password = newPassword;

    wxMessageBox(wxT("Successfully updated your username."));
}

/**
 * @param paths - a vector which stores all the paths the user encrypted
 * @param key - a vector which stores the old encryption key to decrypt the file to re-encrypt them
 */
void updateWin::decrypt(std::vector<std::string> paths, std::vector<unsigned char> key)
{
    // Declaring the file reader/writer, AES variable to decrypt with, decryptedData to store the data
    // in before writing it back and a queue to store the sub-directories in
    std::ofstream fwriter;
    std::ifstream freader;
    AES128* AES = new AES128();
    std::string decryptedData;
    std::queue<std::filesystem::path> Q_paths;

    // Scanning the paths vector to get all the paths the user encrypted
    for (std::vector<std::string>::iterator it = paths.begin(); it < paths.end(); it++) {

        // Pushing the paths in the vector to the queue to fully scan them and decrypt all the sub
        // directories
        Q_paths.push(*it);

        // Scanning the queue to get all the sub-directories
        while (!Q_paths.empty()) {

            // Checking if the path is directory.
            // True - Scanning the path for all the files and sub-directory
            // False - Decrypting the file
            if (std::filesystem::is_directory(Q_paths.front())) {
                for (auto& dir : std::filesystem::directory_iterator(Q_paths.front())) {

                    // Checking if the directory_iterator return a file path or a directory path
                    // True - Decrypting the file
                    // False - Adding that directory to the queue
                    if (!std::filesystem::is_directory(dir)) {

                        // Opening the file to read from it
                        // The 'freader' reads the file in binary which means it returns byte (unsigned char)
                        freader.open(dir.path(), std::ios_base::binary);

                        // Storing the data from the file in an ussigned char vector
                        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(freader), {});

                        // Closing the file
                        freader.close();

                        // Decrypting the data
                        decryptedData = AES->AES_Decrypt(buffer, key);

                        // Opening the file to overwrite it
                        // The 'fwriter' write to the file in binary which means it returns byte (unsigned char)
                        fwriter.open(dir.path(), std::ios_base::binary);
                        
                        // Writing to the file
                        fwriter << decryptedData;

                        // Closing the file
                        fwriter.close();
                    }
                    else {
                        // Pushing the sub-directory to the queue
                        Q_paths.push(dir.path());
                    }
                }
                // Finished Scanning a directory => removing the directory from the queue
                Q_paths.pop();
            }
            else {

		// Opening the file for reading the binaries
                freader.open(Q_paths.front(), std::ios_base::binary);

		// Creating a vector to store the data and storing the data inside
                std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(freader), {});

	 	// Closing the file to not cause a conflict when creating a reader
                freader.close();

		// Decrypting the data
                decryptedData = AES->AES_Decrypt(buffer, key);

		// Opening the file for writing the decrypted data inside, writing the data and cloning the file
                fwriter.open(Q_paths.front(), std::ios_base::binary);
                fwriter << decryptedData;
                fwriter.close();
            }
	    Q_path.pop();	// After the scan is finished poping the directory out of the queue
        }
    }
    // Deleting the AES variable to free memory
    delete AES;
}

void updateWin::encrypt(std::vector<std::string> paths, std::vector<unsigned char> key)
{
    // Creating the file reader/writer, the AES variable for encrypting,
    // creating a string to store the encrypted data inside and creating a queue
    // to store the sub-directories inside
    std::ofstream fwriter;
    std::ifstream freader;
    AES128* AES = new AES128();
    std::string encryptedData;
    std::queue<std::filesystem::path> Q_paths;

    // Scanning the vector for all the paths
    for (std::vector<std::string>::iterator it = paths.begin(); it < paths.end(); it++) {

	// Adding the first path to the queue to start scanning
        Q_paths.push(*it);

	// Scanning the queue for all the sub-directories
        while (!Q_paths.empty()) {
	    // Checking if the path is a directory
            if (std::filesystem::is_directory(Q_paths.front())) {
		// Scanning the path if its a directory
                for (auto& dir : std::filesystem::directory_iterator(Q_paths.front())) {
		    // Checking if the path is directory
                    if (!std::filesystem::is_directory(dir)) {

			// Opening the file in binary form
                        freader.open(dir.path(), std::ios_base::binary);

			// Copying the file content into the vector
                        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(freader), {});

		        // Closing the file
                        freader.close();

			// Calling the encryption function to encrypt the file content
                        encryptedData = AES->AES_Encrypt(buffer, key);

			// Opening the file in binary form
                        fwriter.open(dir.path(), std::ios_base::binary);
                        fwriter << encryptedData;	// Replacing the content with the encrypted content
                        fwriter.close();		// Closing the file
                    }
                    else {
			// If the path is directory, insert the directory into the queue to scan later
                        Q_paths.push(dir.path());
                    }
                }
		// After the scan is finished poping the directory out of the queue
                Q_paths.pop();
            }
            else {
		// Opening thefile in binary form
                freader.open(Q_paths.front(), std::ios_base::binary);

		// Copying the content into the vector
                std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(freader), {});

		// Closing the file
                freader.close();

		// Encrypting the content
                encryptedData = AES->AES_Encrypt(buffer, key);

		// Opening the file
                fwriter.open(Q_paths.front(), std::ios_base::binary);
                fwriter << encryptedData;	// Replacing the file content with the encrypted content 
                fwriter.close();		// Closing the file
            }
	    Q_path.pop();	// After the scan is finished poping the directory out of the queue
        }
    }
    delete AES; // Freeing up memory
}
