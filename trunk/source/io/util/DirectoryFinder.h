/***
 * hesperus: DirectoryFinder.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DIRECTORYFINDER
#define H_HESP_DIRECTORYFINDER

#include <boost/filesystem/operations.hpp>

namespace hesp {

boost::filesystem::path determine_base_directory_from_game();
boost::filesystem::path determine_base_directory_from_tool();
boost::filesystem::path determine_executable_location();
boost::filesystem::path determine_images_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_levels_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_models_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_settings_directory(const boost::filesystem::path& baseDir);
boost::filesystem::path determine_textures_directory(const boost::filesystem::path& baseDir);

}

#endif
