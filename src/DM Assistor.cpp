// DM Assistor.cpp : Defines the entry point for the application.

#include "headers/framework.h"
#include "headers/DM Assistor.h"

/// CONSTANTS ///

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

std::map<std::string, int> abilityMap = {
    {"strength", Strength},
    {"dexterity", Dexterity},
    {"wisdom", Wisdom},
    {"intelligence", Intelligence},
    {"constitution", Constitution},
    {"charisma", Charisma}
};

std::map<std::string, int> skillMap = {
    {"athletics", Athletics},
    {"acrobatics", Acrobatics},
    {"sleightofhand", SleightOfHand},
    {"stealth", Stealth},
    {"arcana", Arcana},
    {"history", History},
    {"investigation", Investigation},
    {"nature", Nature},
    {"religion", Religion},
    {"animalhandling", AnimalHandling},
    {"insight", Insight},
    {"medicine", Medicine},
    {"perception", Perception},
    {"survival", Survival},
    {"deception", Deception},
    {"intimidation", Intimidation},
    {"performance", Performance},
    {"persuasion", Persuasion}
};

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

void LogMessage(wxTextCtrl* reference, const wxString& message, bool clear)
{
	if (clear)
		reference->Clear();
    reference->AppendText(message + "\n");
}

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

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* debugTextCtrl = new wxTextCtrl(DebugTab, wxID_ANY, "Type 'commands' and hit ENTER to view commands.", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);

	sizer->Add(debugTextCtrl, 1, wxEXPAND);
	DebugTab->SetSizer(sizer);

    SetBackgroundColourForAllChildren(DebugTab, debugTabColor);
    DebugTab->SetBackgroundColour(debugTabColor);
    RIBBON->AddPage(DebugTab, "Debug Tab");

    // Testing variables
    testStat = Stat();
	testRoller = DiceRoller();

    // Key listener for debugging
    Bind(wxEVT_TEXT_ENTER, [this, debugTextCtrl](wxCommandEvent& event) {
    	/// Parse command
    	std::string command = debugTextCtrl->GetValue().ToStdString();
    	std::istringstream iss(command);

    	std::string words[5];
    	for (int i = 0; i < 5; i++)
    		words[i] = "0";

    	int i = 0;
    	for (std::string s; iss >> s && i < 5; ++i)
    		words[i] = s;

    	///// COMMAND HANDLING
    	if (words[0] == "commands")
    		LogMessage(debugTextCtrl, "Commands:"
    		"\nstat -change [ability] [value]"
    		"\nstat -list [skills/saves]"
    		"\nstat -change -proficiency [ability/skill] [0 = none, 1 = prof, 2 = exp]"
    		"\nspell -list"
            "\nspell -consume [slot]"
            "\nspell -replenish [slot]"
            "\nspell -max [slot] [new max slots]"
            "\nspell -add [name] [description_with_no_spaces] [level]"
            "\nspell -remove [name] [level]"
    		"\ndice -roll d[4/8/12/20] [skill]"
    		"\ndice -advantage [0 = none, 1 = advantage, 2 = disadvantage]"
    		"\ndice -karmic (this toggles karmic dice, no other params needed)"
            "\ncondition -toggle [index of condition, goes 1-14]"
            "\ncondition -list"
            "\nfeat -add [name]"
            "\nfeat -remove [name]"
			"\nfeat -list"
    		"", true);
                
    	if (words[0] == "stat") {
    		if (words[1] == "-change") { /// Modify a stat
    			bool saveProf = std::stoi(words[4]) != 0;
    			if (words[2] == "-proficiency") {
    				if (skillMap.count(words[3]) > 0)
    					testStat.updateSkillProf(skillMap[words[3]], std::stoi(words[4]));
    				if (abilityMap.count(words[3]) > 0)
    					testStat.updateSaveProf(abilityMap[words[3]], saveProf);
    			}

    			if (abilityMap.count(words[2]) > 0)
    				testStat.updateScore(abilityMap[words[2]], std::stoi(words[3]));

    			LogMessage(debugTextCtrl, "Update made.", true);
    		}
    		if (words[1] == "-list") { /// Return skills
    			if (words[2] == "skills")
    				LogMessage(debugTextCtrl, testStat.returnSkills(), true);
    			if (words[2] == "saves") {
    				LogMessage(debugTextCtrl, testStat.returnSaves(), true);
    			}
    		}
    	}

    	if (words[0] == "dice")
    	{
    		if (words[1] == "-roll")
    		{
    			/// Cut the "d" out of the d20 param
    			std::string param3 = words[2];
    			param3.erase(std::remove_if(param3.begin(), param3.end(), [](char c) { return !std::isdigit(c); }), param3.end());
    			int diceSides = std::stoi(param3);

    			if (words[3] == "0") {
    				std::string result = std::to_string(testRoller.roll(diceSides, 0, advantage, disadvantage));
    				LogMessage(debugTextCtrl, "Rolled a " + words[2] + ": " + result, true);
    			}
    			else {
    				Skill skill = testStat.getSkill(skillMap[words[3]]);
    				std::string result = std::to_string(testRoller.roll(diceSides, skill.value, advantage, disadvantage));
    				LogMessage(debugTextCtrl, "Rolled for " + skill.getSkillName() + ": " + result, true);
    			}
    		}
    		if (words[1] == "-advantage")
    		{
    			advantage = false;
    			disadvantage = false;

    			if (std::stoi(words[2]) == 1) {
    				advantage = true;
    				disadvantage = false;
    			}
    			if (std::stoi(words[2]) == 2) {
    				advantage = false;
    				disadvantage = true;
    			}

    			std::string result = "";
    			if (advantage)
    				result = "with advantage";
    			if (disadvantage)
    				result = "with disadvantage";
    			if (advantage && disadvantage || (!advantage && !disadvantage))
    				result = "normally";
    			LogMessage(debugTextCtrl, "Dice now roll " + result, true);
    		}
    		if (words[1] == "-karmic") {
    			testRoller.toggleKarmicDice();
    			LogMessage(debugTextCtrl, "Toggle invert for Karmic", true);
    		}
    	}

        if (words[0] == "spell")
        {
            if (words[1] == "-list")
                LogMessage(debugTextCtrl, testStat.getSpellBook().printBook(), true);

            if (words[1] == "-consume")
            {
                if (testStat.getSpellBook().expendSlot(std::stoi(words[2])))
					LogMessage(debugTextCtrl, "Slot " + words[2] + " expended.", true);
                else
                    LogMessage(debugTextCtrl, "Failure to expend slot.", true);
            }
            if (words[1] == "-replenish")
            {
                if (testStat.getSpellBook().replenishSlot(std::stoi(words[2])))
					LogMessage(debugTextCtrl, "Slot " + words[2] + " replenished.", true);
				else
					LogMessage(debugTextCtrl, "Failure to replenish slot.", true);
            }
            if (words[1] == "-max")
            {
				if (testStat.getSpellBook().modifySlotMax(std::stoi(words[2]), std::stoi(words[3])))
					LogMessage(debugTextCtrl, "Slot " + words[2] + " max set to " + words[3], true);
                else
					LogMessage(debugTextCtrl, "Failure to modify slot max.", true);
            }
        	if (words[1] == "-add")
        	{
        		Spell newSpell = Spell(words[2], words[3], std::stoi(words[4]));
        		if (testStat.getSpellBook().addSpell(newSpell))
                    LogMessage(debugTextCtrl, "Spell " + words[2] + " added.", true);
        		else
        			LogMessage(debugTextCtrl, "Failure to add spell.", true);
        	}
			if (words[1] == "-remove")
			{
                Spell newSpell = Spell(words[2], "debug", std::stoi(words[3]));
				if (testStat.getSpellBook().removeSpell(newSpell))
                    LogMessage(debugTextCtrl, "Spell " + words[2] + " removed.", true);
				else 
                    LogMessage(debugTextCtrl, "Failure to remove spell.", true);
			}
        }

        if (words[0] == "condition") 
        {
			if (words[1] == "-toggle")
			{
				if (testStat.toggleCondition(std::stoi(words[2])))
					LogMessage(debugTextCtrl, "Condition toggled.", true);
				else
					LogMessage(debugTextCtrl, "Failure to toggle condition.", true);
			}
			if (words[1] == "-list")
			{
				std::string result = "";
				for (Condition c : testStat.getActiveConditions())
					result += c.getName() + ": " + c.getDesc() + "\n";
                if (result == "")
					LogMessage(debugTextCtrl, "No conditions active.", true);
				else
					LogMessage(debugTextCtrl, "Active conditions:\n" + result, true);
			}
        }

        int samArray[6][1] = {
                    {-1},
                    {4},
                    {-1},
                    {-1},
                    {-1},
                    {-1}
        };
        int initArray[6][1] = {
                {-1},
                {-1},
                {-1},
                {-1},
                {-1},
                {-1}
        };
        int initSArray[18][1] = {
          {-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1},{-1}
        };
        Feature newFeat = Feature();
        newFeat.init(words[2], "Sample feat. +2 Dex", NORMAL, samArray, initArray, initSArray, -1, -1, -1);

        if (words[0] == "feat")
        {
			if (words[1] == "-add")
			{
				if (testStat.addFeat(newFeat))
					LogMessage(debugTextCtrl, "Feat " + words[2] + " added.", true);
				else
					LogMessage(debugTextCtrl, "Failure to add feat.", true);
			}
			if (words[1] == "-remove")
			{
				if (testStat.removeFeat(newFeat))
					LogMessage(debugTextCtrl, "Feat " + words[2] + " removed.", true);
				else
					LogMessage(debugTextCtrl, "Failure to remove feat.", true);
			}
			if (words[1] == "-list")
			{
				std::string result = "";
				for (Feature f : testStat.getFeatures())
					result += f.printFeat() + "\n";
				if (result == "")
					LogMessage(debugTextCtrl, "No feats active.", true);
				else
					LogMessage(debugTextCtrl, "Active feats:\n" + result, true);
			}
        }
    });


};