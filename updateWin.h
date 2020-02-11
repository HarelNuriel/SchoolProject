#pragma once

#include "wx/wx.h"
#include "SQLClass.h"
#include "usrWin.h"
#include "sha256.h"

#define KEY_SIZE 16

class userWindow;

class updateWin : public wxFrame
{
public:
	updateWin(wxWindow* win, const wxString& title, const wxPoint& pos, const wxSize& size, std::string hashedUser, std::string hashedPassword);
	~updateWin();

	wxTextCtrl* preTextBox;
	wxTextCtrl* newTextBox;
	wxButton* updateUsr;
	wxButton* updatePasswordBtn;
	wxButton* backBtn;
	wxPanel* panel;
	wxBoxSizer* vbox;
	wxBoxSizer* btnBoxR;
	wxBoxSizer* btnBoxL;
	wxFlexGridSizer* fgs;
	wxFont* font;
	wxStaticText* previous;
	wxStaticText* updated;
	SQLClass* db = new SQLClass();
	std::string Username;
	std::string password;

private:
	void back(wxCommandEvent& evt);
	void updateUser(wxCommandEvent& evt);
	void updatePassword(wxCommandEvent& evt);
	void decrypt(char** paths, int pathCount, std::string key);
	void encrypt(char** paths, int pathCount, std::string key);

	DECLARE_EVENT_TABLE()
};
