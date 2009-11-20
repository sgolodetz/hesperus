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

bf::path determine_audio_directory()
{
	return determine_base_directory() / "resources/audio/";
}

bf::path determine_base_directory_from_game()
{
	bf::path exePath = determine_executable_location();		// hesperus/bin/?.exe
	return exePath.branch_path();							// -> hesperus/bin
}

bf::path determine_base_directory_from_tool()
{
	bf::path exePath = determine_executable_location();		// hesperus/bin/tools/?.exe
	return exePath.branch_path().branch_path();				// -> hesperus/bin
}

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

bf::path determine_images_directory()
{
	return determine_base_directory() / "resources/images/";
}

bf::path determine_levels_directory()
{
	return determine_base_directory() / "resources/levels/";
}

bf::path determine_models_directory()
{
	return determine_base_directory() / "resources/models/";
}

bf::path determine_profiles_directory()
{
	return determine_base_directory() / "resources/profiles/";
}

bf::path determine_scripts_directory()
{
	return determine_base_directory() / "resources/scripts/";
}

bf::path determine_settings_directory()
{
	return determine_base_directory() / "resources/settings/";
}

bf::path determine_sprites_directory()
{
	return determine_base_directory() / "resources/sprites/";
}

bf::path determine_textures_directory()
{
	return determine_base_directory() / "resources/textures/";
}

}
