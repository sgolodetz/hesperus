/***
 * hesperus: DirectoryFinder.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DirectoryFinder.h"
namespace bf = boost::filesystem;

#ifdef _WIN32
#include <windows.h>
#endif

namespace hesp {

bf::path determine_executable_location()
{
#ifdef _WIN32
	std::wstring ws;
	ws.resize(512);
	::GetModuleFileName(NULL, &ws[0], 512);
	std::string s(ws.begin(), ws.end());
	return s;
#else
	#error Can't yet determine the executable location on non-Windows platforms
#endif
}

bf::path determine_settings_directory_from_game()
{
	bf::path settingsDir = determine_executable_location();
	settingsDir = settingsDir.branch_path();	// -> hesperus/bin/
	settingsDir /= "resources/settings/";		// -> hesperus/bin/resources/settings
	return settingsDir;
}

bf::path determine_settings_directory_from_tool()
{
	bf::path settingsDir = determine_executable_location();
	settingsDir = settingsDir.branch_path();	// -> hesperus/bin/tools/
	settingsDir = settingsDir.branch_path();	// -> hesperus/bin/
	settingsDir /= "resources/settings/";		// -> hesperus/bin/resources/settings
	return settingsDir;
}

}
