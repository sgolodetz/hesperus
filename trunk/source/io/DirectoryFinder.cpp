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

bf::path determine_settings_directory_from_tool()
{
#ifdef _WIN32
	// FIXME:	This platform-specific code to determine the executable path should be moved into a separate function.
	std::wstring ws;
	ws.resize(512);
	::GetModuleFileName(NULL, &ws[0], 512);
	std::string s(ws.begin(), ws.end());

	bf::path settingsDir = s;
	settingsDir = settingsDir.branch_path();	// -> hesperus/bin/tools/
	settingsDir = settingsDir.branch_path();	// -> hesperus/bin/
	settingsDir /= "resources/settings/";		// -> hesperus/bin/resources/settings
	return settingsDir;
#else
	#error Can't yet determine the settings directory on non-Windows platforms
#endif
}

}
