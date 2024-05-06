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

// MyFrame Class, defines the ribbon and the main window/frame of the application //
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);
    void createCharTab();
    void createDebugTab();
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
MyFrame::MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
    // Set the frame/window size
    this->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Prevent resizing of the window
    this->SetMinSize(wxSize(SCREEN_WIDTH, SCREEN_HEIGHT));
    this->SetMaxSize(wxSize(SCREEN_WIDTH, SCREEN_HEIGHT));

	// Create the ribbon "notebook" that will hold all tabs
    RIBBON = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

    // Load the icon
    wxIcon icon;
    icon.LoadFile("src/resources/DMAssistorMain.ico", wxBITMAP_TYPE_ICO);
    this->SetIcon(icon);

    // Create all tabs in the ribbon
    createCharTab();
    createDebugTab();
}

// Method that handles the character tab
void MyFrame::createCharTab()
{
    wxPanel* CharTab = new wxPanel(RIBBON, wxID_ANY);
    CharTab->SetBackgroundColour(wxColour(111, 55, 205));

    RIBBON->AddPage(CharTab, "Character Sheet");
};

// Method that handles the debug tab
void MyFrame::createDebugTab()
{
    wxPanel* DebugTab = new wxPanel(RIBBON, wxID_ANY);
    DebugTab->SetBackgroundColour(wxColour(255, 255, 255));

    /// Define all positions and sizes of all elements on the debug tab
    int inTxtBx[4] = { 10, 10, 200, 100 };
    int queryBtn[4] = { inTxtBx[0] + inTxtBx[2] + 5, inTxtBx[1], 125, 25 };
    int encountBx[4] = { queryBtn[0], queryBtn[1] + queryBtn[3] + 5, queryBtn[2], queryBtn[3]};
    int dialogBx[4] = { queryBtn[0], encountBx[1] + encountBx[3] + 5, queryBtn[2], queryBtn[3]};
    int outTxtBx[4] = { encountBx[0] + encountBx[2] + 5, inTxtBx[1], SCREEN_WIDTH - (encountBx[0] + encountBx[2]) - 40, inTxtBx[3]};

	int monsterInBx[4] = { inTxtBx[0], inTxtBx[1] + inTxtBx[3] + 5, 200, 50};
	int monsterQryBtn[4] = { monsterInBx[0] + monsterInBx[2] + 5, monsterInBx[1] + (monsterInBx[3] / 4), 125, 25};
	int monsterOutBx[4] = { monsterQryBtn[0] + monsterQryBtn[2] + 5, monsterInBx[1], SCREEN_WIDTH - (monsterQryBtn[0] + monsterQryBtn[2]) - 40, monsterInBx[3] };

	/// Create all elements on the debug tab
    wxTextCtrl* inTxtBox = new wxTextCtrl(DebugTab, wxID_ANY, "", wxPoint(inTxtBx[0], inTxtBx[1]), wxSize(inTxtBx[2], inTxtBx[3]), wxTE_MULTILINE | wxTE_WORDWRAP);
    wxTextCtrl* outTxtBox = new wxTextCtrl(DebugTab, wxID_ANY, "No response.", wxPoint(outTxtBx[0], outTxtBx[1]), wxSize(outTxtBx[2], outTxtBx[3]), wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_NO_VSCROLL | wxTE_READONLY);
    wxButton* aiQueryBtn = new wxButton(DebugTab, wxID_ANY, "Debug AI Prompt", wxPoint(queryBtn[0], queryBtn[1]), wxSize(queryBtn[2], queryBtn[3]));
	wxCheckBox* encounterCBx = new wxCheckBox(DebugTab, wxID_ANY, "Generate encounter", wxPoint(encountBx[0], encountBx[1]), wxSize(encountBx[2], encountBx[3]));
    wxCheckBox* dialogueCBx = new wxCheckBox(DebugTab, wxID_ANY, "Generate dialogue", wxPoint(dialogBx[0], dialogBx[1]), wxSize(dialogBx[2], dialogBx[3]));

	wxTextCtrl* monsterInTxtBox = new wxTextCtrl(DebugTab, wxID_ANY, "", wxPoint(monsterInBx[0], monsterInBx[1]), wxSize(monsterInBx[2], monsterInBx[3]), wxTE_MULTILINE | wxTE_WORDWRAP);
	wxTextCtrl* monsterOutTxtBox = new wxTextCtrl(DebugTab, wxID_ANY, "No response.", wxPoint(monsterOutBx[0], monsterOutBx[1]), wxSize(monsterOutBx[2], monsterOutBx[3]), wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_NO_VSCROLL | wxTE_READONLY);
	wxButton* monsterQueryBtn = new wxButton(DebugTab, wxID_ANY, "Debug Pull Monster", wxPoint(monsterQryBtn[0], monsterQryBtn[1]), wxSize(monsterQryBtn[2], monsterQryBtn[3]));

    // Event listener for button clicks
    aiQueryBtn->Bind(wxEVT_BUTTON, [inTxtBox, outTxtBox, encounterCBx, dialogueCBx](wxCommandEvent& event)
    {
    	wxString userAIPrompt = inTxtBox->GetValue();
		if (userAIPrompt.IsEmpty())
			outTxtBox->SetValue("No response.");
		else if (encounterCBx->GetValue())
			outTxtBox->SetValue("ENCOUNTER: " + userAIPrompt);
		else if (dialogueCBx->GetValue())
			outTxtBox->SetValue("DIALOGUE: " + userAIPrompt);
		else
			outTxtBox->SetValue(userAIPrompt);
    });

    monsterQueryBtn->Bind(wxEVT_BUTTON, [monsterInTxtBox, monsterOutTxtBox](wxCommandEvent& event)
        {
            wxString monsterPulled = monsterInTxtBox->GetValue();
            if (monsterPulled.IsEmpty())
                monsterOutTxtBox->SetValue("No response.");
            else
                monsterOutTxtBox->SetValue(monsterPulled);
        });

    // Event listeners for checkbox clicks
    encounterCBx->Bind(wxEVT_CHECKBOX, [encounterCBx, dialogueCBx](wxCommandEvent& event)
    {
    	bool isChecked = static_cast<wxCheckBox*>(event.GetEventObject())->GetValue();
		if (isChecked)
			dialogueCBx->SetValue(false);
    });

	dialogueCBx->Bind(wxEVT_CHECKBOX, [encounterCBx, dialogueCBx](wxCommandEvent& event)
	{
		bool isChecked = static_cast<wxCheckBox*>(event.GetEventObject())->GetValue();
        if (isChecked)
            encounterCBx->SetValue(false);
	});

    RIBBON->AddPage(DebugTab, "Debug Tab");
};
