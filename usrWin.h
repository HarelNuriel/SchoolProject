#pragma once

#include "wx/wx.h"
#include "mainFrame.h"

class usrWin : public wxFrame
{
public:
	usrWin(wxWindow* win, const wxString& title, const wxPoint& pos, const wxSize& size);
	~usrWin();
};

