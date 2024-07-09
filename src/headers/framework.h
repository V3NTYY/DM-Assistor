#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <filesystem>
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// WXWIDGETS
#include <wx/wx.h>
#include <wx/notebook.h>

// All other headers
#include "Stat.h"
#include "DiceRoller.h"

// Json
#include "headers/json.hpp"
#include <fstream>
using json = nlohmann::json;

#endif