/***
 * hesperus: DirectoryFinder.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DIRECTORYFINDER
#define H_HESP_DIRECTORYFINDER

#include <boost/filesystem/operations.hpp>

namespace hesp {

/**
Determines the location of the game's base directory. Note that how this is done
varies depending on whether we're trying to determine the location from within
the game or from within one of its tools. It should thus be implemented separately
within each project to call the appropriate location determination function.
*/
boost::filesystem::path determine_base_directory();

boost::filesystem::path determine_audio_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_base_directory_from_game();
boost::filesystem::path determine_base_directory_from_tool();
boost::filesystem::path determine_executable_location();
boost::filesystem::path determine_images_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_levels_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_models_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_scripts_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_settings_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_textures_directory(const boost::filesystem::path& baseDir);

}

#endif
