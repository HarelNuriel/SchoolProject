#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "wx/wx.h"
#include "mainFrame.h"
#include "AES128.h"
#include "SQLClass.h"
#include <filesystem>
#include <fstream>
#include <regex>

#define KEY_SIZE 16

class mainFrame;
class updateWin;

class userWindow : public wxFrame
{
public:
	userWindow(wxWindow* win, const wxString& title, const wxPoint& pos, const wxSize& size, std::string hashedUser, std::string hashedPassword);
	~userWindow();

	void encryptFile(wxCommandEvent& evt);
	void decryptFile(wxCommandEvent& evt);
	void back(wxCommandEvent& evt);
	void updateFunc(wxCommandEvent& evt);

private:
	wxTextCtrl* pathTextBox;
	wxButton* encryptBtn;
	wxButton* decryptBtn;
	wxButton* backBtn;
	wxButton* updateBtn;
	wxPanel* panel;
	wxBoxSizer* vbox;
	wxBoxSizer* btnBoxR;
	wxBoxSizer* btnBoxL;
	wxFlexGridSizer* fgs;
	wxFont* font;
	wxStaticText* path;
	updateWin* update;
	SQLClass* db = new SQLClass();
	std::string Username;
	std::string password;

	void GetKey(char* key);

public:
	DECLARE_EVENT_TABLE()
};
