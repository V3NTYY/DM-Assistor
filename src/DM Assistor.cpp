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

std::map<std::string, int> conditionMap = {
    {"blinded", Blinded},
    {"charmed", Charmed},
    {"deafened", Deafened},
    {"frightened", Frightened},
    {"grappled", Grappled},
    {"incapacitated", Incapacitated},
    {"invisible", Invisible},
    {"paralyzed", Paralyzed},
    {"petrified", Petrified},
    {"poisoned", Poisoned},
    {"prone", Prone},               
    {"restrained", Restrained},              
    {"stunned", Stunned},      
    {"unconscious", Unconscious}
};

std::map<std::string, int> skillMap = {
    {"athletics", Athletics},
    {"acrobatics", Acrobatics},
    {"sleight of hand", SleightOfHand},
    {"stealth", Stealth},
    {"arcana", Arcana},
    {"history", History},
    {"investigation", Investigation},
    {"nature", Nature},
    {"religion", Religion},
    {"animal handling", AnimalHandling},
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

        int c = 0;
        const int argSize = 5;
        std::string args[argSize];
        std::string arg;
        bool inQuotes = false;

        /// Initialize the array
		for (int i = 0; i < argSize; i++)
			args[i] = "0";

        for (char ch : command) {
            if (ch == '\"') { // Toggle the inQuotes state
                inQuotes = !inQuotes;
                if (!inQuotes) { // End of quoted argument
                    args[c] = arg;
                    arg.clear();
                    c++;
                    if (c >= argSize) {
                        LogMessage(debugTextCtrl, "Too many arguments.", true);
                        return;
                    }
                }
            }
            else if (std::isspace(ch) && !inQuotes) { // Space outside quotes
                if (!arg.empty()) {
                    args[c] = arg;
                    arg.clear();
                    c++;
					if (c >= argSize) {
						LogMessage(debugTextCtrl, "Too many arguments.", true);
						return;
					}
                }
            }
            else
                arg += ch; // Add character to current argument
        }

        if (!arg.empty()) // Add last argument if present
            args[c] = arg;

        // Set the entire args to lowercase
        for (int i = 0; i < argSize; i++)
            std::transform(args[i].begin(), args[i].end(), args[i].begin(), ::tolower);

    	///// COMMAND HANDLING
    	if (args[0] == "commands")
    		LogMessage(debugTextCtrl, "COMMANDS:"
    		"\nstat -list [skills/saves]"
    		"\nstat -change [ability] [value]"
    		"\nstat -change -proficiency \"[skill/save]\" [0 = none, 1 = prof, 2 = exp]"
    		"\nspell -list"
            "\nspell -consume [slot]"
            "\nspell -replenish [slot]"
            "\nspell -max [slot] [new max slots]"
            "\nspell -add \"[name]\" \"[description]\" [level]"
            "\nspell -remove \"[name]\" [level]"
    		"\ndice -roll d[4/8/12/20] \"[skill]\""
    		"\ndice -advantage [0 = none, 1 = advantage, 2 = disadvantage]"
    		"\ndice -karmic (this toggles karmic dice, no other params needed)"
            "\ncondition -toggle \"[name]\""
            "\ncondition -list"
            "\nfeat -add \"[filename]\" [ability] [ability] (NOTE: only enter a second parameter if you are adding the feat twice, such as an ASI or another repeatable feat)"
            "\nfeat -remove \"[name]\""
            "\nfeat -save \"[name]"
			"\nfeat -list"
    		"", true);
                
    	if (args[0] == "stat") {
    		if (args[1] == "-change") { /// Modify a stat
    			bool saveProf = std::stoi(args[4]) != 0;
    			if (args[2] == "-proficiency") {
    				if (skillMap.count(args[3]) > 0)
    					testStat.updateSkillProf(skillMap[args[3]], std::stoi(args[4]));
    				if (abilityMap.count(args[3]) > 0)
    					testStat.updateSaveProf(abilityMap[args[3]], saveProf);
    			}

    			if (abilityMap.count(args[2]) > 0)
    				testStat.updateScore(abilityMap[args[2]], std::stoi(args[3]));

    			LogMessage(debugTextCtrl, "Update made.", true);
    		}
    		if (args[1] == "-list") { /// Return skills
    			if (args[2] == "skills")
    				LogMessage(debugTextCtrl, testStat.returnSkills(), true);
    			if (args[2] == "saves") {
    				LogMessage(debugTextCtrl, testStat.returnSaves(), true);
    			}
    		}
    	}

    	if (args[0] == "dice")
    	{
    		if (args[1] == "-roll")
    		{
    			/// Cut the "d" out of the d20 param
    			std::string param3 = args[2];
    			param3.erase(std::remove_if(param3.begin(), param3.end(), [](char c) { return !std::isdigit(c); }), param3.end());
    			int diceSides = std::stoi(param3);

    			if (args[3] == "0") {
    				std::string result = std::to_string(testRoller.roll(diceSides, 0, advantage, disadvantage));
    				LogMessage(debugTextCtrl, "Rolled a " + args[2] + ": " + result, true);
    			}
    			else {
    				Skill skill = testStat.getSkill(skillMap[args[3]]);
    				std::string result = std::to_string(testRoller.roll(diceSides, skill.value, advantage, disadvantage));
    				LogMessage(debugTextCtrl, "Rolled for " + skill.getSkillName() + ": " + result, true);
    			}
    		}
    		if (args[1] == "-advantage")
    		{
    			advantage = false;
    			disadvantage = false;

    			if (std::stoi(args[2]) == 1) {
    				advantage = true;
    				disadvantage = false;
    			}
    			if (std::stoi(args[2]) == 2) {
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
    		if (args[1] == "-karmic") {
    			testRoller.toggleKarmicDice();
    			LogMessage(debugTextCtrl, "Toggle invert for Karmic", true);
    		}
    	}

        if (args[0] == "spell")
        {
            if (args[1] == "-list")
                LogMessage(debugTextCtrl, testStat.getSpellBook().printBook(), true);

            if (args[1] == "-consume")
            {
                if (testStat.getSpellBook().expendSlot(std::stoi(args[2])))
					LogMessage(debugTextCtrl, "Slot " + args[2] + " expended.", true);
                else
                    LogMessage(debugTextCtrl, "Failure to expend slot.", true);
            }
            if (args[1] == "-replenish")
            {
                if (testStat.getSpellBook().replenishSlot(std::stoi(args[2])))
					LogMessage(debugTextCtrl, "Slot " + args[2] + " replenished.", true);
				else
					LogMessage(debugTextCtrl, "Failure to replenish slot.", true);
            }
            if (args[1] == "-max")
            {
				if (testStat.getSpellBook().modifySlotMax(std::stoi(args[2]), std::stoi(args[3])))
					LogMessage(debugTextCtrl, "Slot " + args[2] + " max set to " + args[3], true);
                else
					LogMessage(debugTextCtrl, "Failure to modify slot max.", true);
            }
        	if (args[1] == "-add")
        	{
        		Spell newSpell = Spell(args[2], args[3], std::stoi(args[4]));
        		if (testStat.getSpellBook().addSpell(newSpell))
                    LogMessage(debugTextCtrl, "Spell " + args[2] + " added.", true);
        		else
        			LogMessage(debugTextCtrl, "Failure to add spell.", true);
        	}
			if (args[1] == "-remove")
			{
                Spell newSpell = Spell(args[2], "debug", std::stoi(args[3]));
				if (testStat.getSpellBook().removeSpell(newSpell))
                    LogMessage(debugTextCtrl, "Spell " + args[2] + " removed.", true);
				else 
                    LogMessage(debugTextCtrl, "Failure to remove spell.", true);
			}
        }

        if (args[0] == "condition") 
        {
			if (args[1] == "-toggle")
			{
				if (testStat.toggleCondition(conditionMap[args[2]]))
					LogMessage(debugTextCtrl, "Condition toggled.", true);
				else
					LogMessage(debugTextCtrl, "Failure to toggle condition.", true);
			}
			if (args[1] == "-list")
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

        if (args[0] == "feat")
        {
			if (args[1] == "-add")
			{
                int variant = -1;
                if (args[3] != '0')
                    variant = abilityMap[args[3]];

                if (testStat.addFeat(Feature::loadFeat(args[2] + ".json", variant)))
					LogMessage(debugTextCtrl, "Feat " + args[2] + " added.", true);
				else
					LogMessage(debugTextCtrl, "Failure to add feat. Does the file exist?", true);

                if (args[4] != '0') {
                    variant = abilityMap[args[4]];

                    if (testStat.addFeat(Feature::loadFeat(args[2] + ".json", variant)))
                        LogMessage(debugTextCtrl, "Feat " + args[2] + " added.", true);
                    else
                        LogMessage(debugTextCtrl, "Failure in adding the second parameter", true);
                }
			}
			if (args[1] == "-remove")
			{
				Feature newFeat = Feature();
				newFeat.init(args[2], "Null", -1, -1);
				if (testStat.removeFeat(newFeat))
					LogMessage(debugTextCtrl, "Feat " + args[2] + " removed.", true);
				else
					LogMessage(debugTextCtrl, "Failure to remove feat.", true);
			}
			if (args[1] == "-list")
			{
				std::string result = "";
                for (Feature f : testStat.getFeatures()) {
                    result += f.printFeat() + "\n";
					if (f.getChain() != nullptr)
						result += "[Chain]" + f.printChain() + "\n";
                }
				if (result == "")
					LogMessage(debugTextCtrl, "No feats active.", true);
				else
					LogMessage(debugTextCtrl, "Active feats:\n" + result, true);
			}
            if (args[1] == "-save") {
                std::vector<Feature> featList = testStat.getFeatures();
                Feature* temp = nullptr;

                /// Grab a pointer reference to the feature to save
                for (Feature& feat : featList)
                    if (feat.name == args[2])
                        temp = &feat;

				// Feature not found
				if (temp == nullptr)
					LogMessage(debugTextCtrl, "Feature '" + args[2] + "' not found.", true);
                else {
                    Feature::saveFeat(*temp, temp->name + ".json");
                    LogMessage(debugTextCtrl, "Feature '" + args[2] + "' successfully saved.", true);
                }
            }
        }
    });


}
;