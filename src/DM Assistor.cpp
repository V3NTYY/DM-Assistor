// DM Assistor.cpp : Defines the entry point for the application.

#include "headers/framework.h"
#include "headers/DM Assistor.h"
#include <wx/wx.h>
#include <wx/notebook.h>

/// CONSTANTS ///

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;

/////////////////

/// Main app declaration ///
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

// MyFrame Class, defines the ribbon and the main window/frame of the applicaton //
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    wxNotebook* RIBBON;
};

// Defines initialization behavior (creation of main frame)
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("DM Assistor - Beta Development");
    frame->Show(true);
    return true;
}

// Macro that acts as main() entry point
wxIMPLEMENT_APP(MyApp);

/// Method that handles creation of the main frame/window.
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    // Set the frame/window size, and create "notebook" (a ribbon that holds tabs) at the top.
    this->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    RIBBON = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

    // Load the icon
    wxIcon icon;
    icon.LoadFile("src/resources/DMAssistorMain.ico", wxBITMAP_TYPE_ICO);
    this->SetIcon(icon);

    // Create a sample page
    wxPanel* CharTab = new wxPanel(RIBBON, wxID_ANY);
    CharTab->SetBackgroundColour(wxColour(111, 55, 205));
    
    // Add text
    wxStaticText* sampleText = new wxStaticText(CharTab, wxID_ANY, "Character Sheet", wxPoint(10, 10), wxSize(100, 100));
	sampleText->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Create a sample debugging page
    wxPanel* DebugTab = new wxPanel(RIBBON, wxID_ANY);

	// Add the pages to the ribbon
    RIBBON->AddPage(CharTab, "Character Sheet");
    RIBBON->AddPage(DebugTab, "Debug Tab");
}