// DM Assistor.cpp : Defines the entry point for the application.

#include "headers/framework.h"
#include "headers/DM Assistor.h"

/// CONSTANTS ///

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;

/////////////////

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
    this->SetIcon(wxICON(IDI_DMASSISTOR));

    // Create all tabs in the ribbon
    createCharTab();
    createDebugTab();
}

// Method that auto-sets the background color of all children of a given window
void SetBackgroundColourForAllChildren(wxWindow* parent, const wxColour& colour)
{
    for (auto& child : parent->GetChildren())
    {
        child->SetBackgroundColour(colour);
        SetBackgroundColourForAllChildren(child, colour); // Recursively set color for children of child
    }
}

// Method that handles the character tab
void MyFrame::createCharTab()
{
    wxPanel* CharTab = new wxPanel(RIBBON, wxID_ANY);
    CharTab->SetBackgroundColour(wxColour(184, 140, 195));

    RIBBON->AddPage(CharTab, "Character Sheet");
};

// Method that handles the debug tab
void MyFrame::createDebugTab()
{
    wxPanel* DebugTab = new wxPanel(RIBBON, wxID_ANY);
	wxColour debugTabColor = wxColour(246, 219, 253);

    /// AI Query Positions & Sizes
    int AITextInputData[4] = { 10, 10, 200, 100 };
    int AIButtonData[4] = { AITextInputData[0] + AITextInputData[2] + 5, AITextInputData[1], 125, 25 };
    int encounterCheckData[4] = { AIButtonData[0], AIButtonData[1] + AIButtonData[3] + 5, AIButtonData[2], AIButtonData[3]};
    int dialogCheckData[4] = { AIButtonData[0], encounterCheckData[1] + encounterCheckData[3] + 5, AIButtonData[2], AIButtonData[3]};
    int AITextOutputData[4] = { encounterCheckData[0] + encounterCheckData[2] + 5, AITextInputData[1], SCREEN_WIDTH - (encounterCheckData[0] + encounterCheckData[2]) - 40, AITextInputData[3]};

	/// Monster Query Positions & Sizes
	int monsterTextInputData[4] = { AITextInputData[0], AITextInputData[1] + AITextInputData[3] + 5, 200, 50};
	int monsterButtonData[4] = { monsterTextInputData[0] + monsterTextInputData[2] + 5, monsterTextInputData[1] + (monsterTextInputData[3] / 4), 125, 25};
	int monsterTextOutputData[4] = { monsterButtonData[0] + monsterButtonData[2] + 5, monsterTextInputData[1], SCREEN_WIDTH - (monsterButtonData[0] + monsterButtonData[2]) - 40, monsterTextInputData[3] };

	/// Placeholder Query Positions & Sizes
    int sampleQueryTextInputData[4] = { monsterTextInputData[0], monsterTextInputData[1] + monsterTextInputData[3] + 5, monsterTextInputData[2], monsterTextInputData[3] };
    int sampleQueryButtonData[4] = { sampleQueryTextInputData[0] + sampleQueryTextInputData[2] + 5, sampleQueryTextInputData[1] + (sampleQueryTextInputData[3] / 4), monsterButtonData[2], monsterButtonData[3] };
    int sampleQueryTextOutputData[4] = { sampleQueryButtonData[0] + sampleQueryButtonData[2] + 5, sampleQueryTextInputData[1], monsterTextOutputData[2], monsterTextOutputData[3] };

	/// AI Query elements
    wxTextCtrl* AITextInput = new wxTextCtrl(DebugTab, wxID_ANY, "", wxPoint(AITextInputData[0], AITextInputData[1]), wxSize(AITextInputData[2], AITextInputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP);
    wxTextCtrl* AITextOutput = new wxTextCtrl(DebugTab, wxID_ANY, "No response.", wxPoint(AITextOutputData[0], AITextOutputData[1]), wxSize(AITextOutputData[2], AITextOutputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_NO_VSCROLL | wxTE_READONLY);
    wxButton* AIButton = new wxButton(DebugTab, wxID_ANY, "Debug AI Prompt", wxPoint(AIButtonData[0], AIButtonData[1]), wxSize(AIButtonData[2], AIButtonData[3]));
	wxCheckBox* encounterCheck = new wxCheckBox(DebugTab, wxID_ANY, "Generate encounter", wxPoint(encounterCheckData[0], encounterCheckData[1]), wxSize(encounterCheckData[2], encounterCheckData[3]));
    wxCheckBox* dialogCheck = new wxCheckBox(DebugTab, wxID_ANY, "Generate dialogue", wxPoint(dialogCheckData[0], dialogCheckData[1]), wxSize(dialogCheckData[2], dialogCheckData[3]));

	//// Monster Query elements
	wxTextCtrl* monsterTextInput = new wxTextCtrl(DebugTab, wxID_ANY, "", wxPoint(monsterTextInputData[0], monsterTextInputData[1]), wxSize(monsterTextInputData[2], monsterTextInputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP);
	wxTextCtrl* monsterTextOutput = new wxTextCtrl(DebugTab, wxID_ANY, "No response.", wxPoint(monsterTextOutputData[0], monsterTextOutputData[1]), wxSize(monsterTextOutputData[2], monsterTextOutputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_NO_VSCROLL | wxTE_READONLY);
	wxButton* monsterButton = new wxButton(DebugTab, wxID_ANY, "Debug Pull Monster", wxPoint(monsterButtonData[0], monsterButtonData[1]), wxSize(monsterButtonData[2], monsterButtonData[3]));

    /// Placeholder Query elements
    wxTextCtrl* sampleQueryTextInput = new wxTextCtrl(DebugTab, wxID_ANY, "", wxPoint(sampleQueryTextInputData[0], sampleQueryTextInputData[1]), wxSize(sampleQueryTextInputData[2], sampleQueryTextInputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP);
    wxTextCtrl* sampleQueryTextOutput = new wxTextCtrl(DebugTab, wxID_ANY, "No response.", wxPoint(sampleQueryTextOutputData[0], sampleQueryTextOutputData[1]), wxSize(sampleQueryTextOutputData[2], sampleQueryTextOutputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_NO_VSCROLL | wxTE_READONLY);
    wxButton* sampleQueryButton = new wxButton(DebugTab, wxID_ANY, "Sample Query", wxPoint(sampleQueryButtonData[0], sampleQueryButtonData[1]), wxSize(sampleQueryButtonData[2], sampleQueryButtonData[3]));

	// Listeners for AI query elements
    AIButton->Bind(wxEVT_BUTTON, [AITextInput, AITextOutput, encounterCheck, dialogCheck](wxCommandEvent& event)
		{
    	wxString userQuery = AITextInput->GetValue();
		if (userQuery.IsEmpty())
			AITextOutput->SetValue("No response.");
		else if (encounterCheck->GetValue())
			AITextOutput->SetValue("ENCOUNTER: " + userQuery);
		else if (dialogCheck->GetValue())
			AITextOutput->SetValue("DIALOGUE: " + userQuery);

		else
			AITextOutput->SetValue(userQuery);
		});

    encounterCheck->Bind(wxEVT_CHECKBOX, [encounterCheck, dialogCheck](wxCommandEvent& event)
        {
            bool isChecked = static_cast<wxCheckBox*>(event.GetEventObject())->GetValue();
            if (isChecked)
                dialogCheck->SetValue(false);
        });

    dialogCheck->Bind(wxEVT_CHECKBOX, [encounterCheck, dialogCheck](wxCommandEvent& event)
        {
            bool isChecked = static_cast<wxCheckBox*>(event.GetEventObject())->GetValue();
            if (isChecked)
                encounterCheck->SetValue(false);
        });

	/// Listeners for monster query elements
    monsterButton->Bind(wxEVT_BUTTON, [monsterTextInput, monsterTextOutput](wxCommandEvent& event)
        {
            wxString monsterQuery = monsterTextInput->GetValue();
            if (monsterQuery.IsEmpty())
                monsterTextOutput->SetValue("No response.");
            else
                monsterTextOutput->SetValue(monsterQuery);
        });

	/// Listeners for placeholder query elements
    sampleQueryButton->Bind(wxEVT_BUTTON, [sampleQueryTextInput, sampleQueryTextOutput](wxCommandEvent& event)
        {
            wxString sampleQuery = sampleQueryTextInput->GetValue();
            if (sampleQuery.IsEmpty())
                sampleQueryTextOutput->SetValue("No response.");
            else
                sampleQueryTextOutput->SetValue(sampleQuery);
        });

    SetBackgroundColourForAllChildren(DebugTab, debugTabColor);

    DebugTab->SetBackgroundColour(debugTabColor);
    RIBBON->AddPage(DebugTab, "Debug Tab");
};
