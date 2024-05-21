// DM Assistor.cpp : Defines the entry point for the application.

#include "headers/framework.h"
#include "headers/DM Assistor.h"

/// CONSTANTS ///

const int SCREEN_WIDTH = 1080;
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

	DMRolls = DiceRoller();
    advantage = false;
    disadvantage = false;

    /// AI Query Positions & Sizes
    int AITextInputData[4] = { 10, 10, 200, 100 };
    int AIButtonData[4] = { AITextInputData[0] + AITextInputData[2] + 5, AITextInputData[1], 125, 25 };
    int encounterCheckData[4] = { AIButtonData[0], AIButtonData[1] + AIButtonData[3] + 5, AIButtonData[2], AIButtonData[3]};
    int dialogCheckData[4] = { AIButtonData[0], encounterCheckData[1] + encounterCheckData[3] + 5, AIButtonData[2], AIButtonData[3]};
    int AITextOutputData[4] = { encounterCheckData[0] + encounterCheckData[2] + 5, AITextInputData[1], SCREEN_WIDTH - (encounterCheckData[0] + encounterCheckData[2]) - 40, AITextInputData[3]};

	/// Dice Roller Positions & Sizes
	int diceRollTextInputData[4] = { AITextInputData[0], AITextInputData[1] + AITextInputData[3] + 5, 200, 50};
	int diceRollButtonData[4] = { diceRollTextInputData[0] + diceRollTextInputData[2] + 5, diceRollTextInputData[1] + (diceRollTextInputData[3] / 4), 125, 25};
	int diceRollTextOutputData[4] = { diceRollButtonData[0] + diceRollButtonData[2] + 5, diceRollTextInputData[1], SCREEN_WIDTH - (diceRollButtonData[0] + diceRollButtonData[2]) - 40, diceRollTextInputData[3] };
	int advantageCheckData[4] = { diceRollButtonData[0], diceRollButtonData[1] + diceRollButtonData[3] + 5, diceRollButtonData[2], diceRollButtonData[3] };
	int disadvantageCheckData[4] = { diceRollButtonData[0], advantageCheckData[1] + advantageCheckData[3] + 5, diceRollButtonData[2], diceRollButtonData[3] };
	int karmicDiceCheckData[4] = { disadvantageCheckData[0], disadvantageCheckData[1] + disadvantageCheckData[3] + 5, disadvantageCheckData[2], disadvantageCheckData[3] };

	/// AI Query elements
    wxTextCtrl* AITextInput = new wxTextCtrl(DebugTab, wxID_ANY, "", wxPoint(AITextInputData[0], AITextInputData[1]), wxSize(AITextInputData[2], AITextInputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP);
    wxTextCtrl* AITextOutput = new wxTextCtrl(DebugTab, wxID_ANY, "No response.", wxPoint(AITextOutputData[0], AITextOutputData[1]), wxSize(AITextOutputData[2], AITextOutputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_NO_VSCROLL | wxTE_READONLY);
    wxButton* AIButton = new wxButton(DebugTab, wxID_ANY, "Debug AI Prompt", wxPoint(AIButtonData[0], AIButtonData[1]), wxSize(AIButtonData[2], AIButtonData[3]));
	wxCheckBox* encounterCheck = new wxCheckBox(DebugTab, wxID_ANY, "Generate encounter", wxPoint(encounterCheckData[0], encounterCheckData[1]), wxSize(encounterCheckData[2], encounterCheckData[3]));
    wxCheckBox* dialogCheck = new wxCheckBox(DebugTab, wxID_ANY, "Generate dialogue", wxPoint(dialogCheckData[0], dialogCheckData[1]), wxSize(dialogCheckData[2], dialogCheckData[3]));

	//// Dice Roller elements
	wxTextCtrl* diceRollTextInput = new wxTextCtrl(DebugTab, wxID_ANY, "20", wxPoint(diceRollTextInputData[0], diceRollTextInputData[1]), wxSize(diceRollTextInputData[2], diceRollTextInputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP);
	wxTextCtrl* diceRollTextOutput = new wxTextCtrl(DebugTab, wxID_ANY, "Awaiting roll...", wxPoint(diceRollTextOutputData[0], diceRollTextOutputData[1]), wxSize(diceRollTextOutputData[2], diceRollTextOutputData[3]), wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_NO_VSCROLL | wxTE_READONLY);
	wxButton* diceRollButton = new wxButton(DebugTab, wxID_ANY, "Roll Dice", wxPoint(diceRollButtonData[0], diceRollButtonData[1]), wxSize(diceRollButtonData[2], diceRollButtonData[3]));
	wxCheckBox* advantageCheck = new wxCheckBox(DebugTab, wxID_ANY, "Advantage", wxPoint(advantageCheckData[0], advantageCheckData[1]), wxSize(advantageCheckData[2], advantageCheckData[3]));
	wxCheckBox* disadvantageCheck = new wxCheckBox(DebugTab, wxID_ANY, "Disadvantage", wxPoint(disadvantageCheckData[0], disadvantageCheckData[1]), wxSize(disadvantageCheckData[2], disadvantageCheckData[3]));
	wxCheckBox* karmicDiceCheck = new wxCheckBox(DebugTab, wxID_ANY, "Karmic Dice", wxPoint(karmicDiceCheckData[0], karmicDiceCheckData[1]), wxSize(karmicDiceCheckData[2], karmicDiceCheckData[3]));

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


	/// Listeners for diceRoll query elements
    advantageCheck->Bind(wxEVT_CHECKBOX, [this, advantageCheck](wxCommandEvent& event)
        {
            advantage = static_cast<wxCheckBox*>(event.GetEventObject())->GetValue();
        });
    disadvantageCheck->Bind(wxEVT_CHECKBOX, [this, disadvantageCheck](wxCommandEvent& event)
        {
            disadvantage = static_cast<wxCheckBox*>(event.GetEventObject())->GetValue();
        });
	karmicDiceCheck->Bind(wxEVT_CHECKBOX, [this, karmicDiceCheck](wxCommandEvent& event)
		{
			DMRolls.toggleKarmicDice();
		});
    diceRollButton->Bind(wxEVT_BUTTON, [this, diceRollTextInput, diceRollTextOutput](wxCommandEvent& event)
        {
            int diceSides = 20;

            wxString diceRollQuery = diceRollTextInput->GetValue();
            diceRollQuery.ToInt(&diceSides);

            int rolledValue = DMRolls.roll(diceSides, 0, advantage, disadvantage);

            diceRollTextOutput->SetValue(std::to_wstring(rolledValue) + "\n" + DMRolls.debugPrint());
        });

    SetBackgroundColourForAllChildren(DebugTab, debugTabColor);
    DebugTab->SetBackgroundColour(debugTabColor);
    RIBBON->AddPage(DebugTab, "Debug Tab");
};
