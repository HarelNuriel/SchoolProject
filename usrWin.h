#pragma once

#include "wx/wx.h"
#include "mainFrame.h"
#include <filesystem>
#include <fstream>

class mainFrame;

class userWindow : public wxFrame
{
public:
	userWindow(wxWindow* win, const wxString& title, const wxPoint& pos, const wxSize& size);
	~userWindow();

	void encryptFile(wxCommandEvent& evt);
	void decryptFile(wxCommandEvent& evt);
	void back(wxCommandEvent& evt);
	bool IsPathValid(std::string path);

	wxTextCtrl* pathTextBox;
	wxButton* encryptBtn;
	wxButton* decryptBtn;
	wxButton* backBtn;
	wxPanel* panel;
	wxBoxSizer* vbox;
	wxBoxSizer* btnBoxR;
	wxBoxSizer* btnBoxL;
	wxFlexGridSizer* fgs;
	wxFont* font;
	wxStaticText* path;

	DECLARE_EVENT_TABLE()
};
