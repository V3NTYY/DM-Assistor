#ifndef DM_ASSISTOR_H
#define DM_ASSISTOR_H

#include "Resource.h" // Used for icons and resource file
#include <string>

const std::string PROGRAM_NAME = "DM Assistor";

/// DM-Assistor Main Window Class

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

#endif
