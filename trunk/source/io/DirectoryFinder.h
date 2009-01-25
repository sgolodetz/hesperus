/***
 * hesperus: DirectoryFinder.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DIRECTORYFINDER
#define H_HESP_DIRECTORYFINDER

#include <boost/filesystem/operations.hpp>

namespace hesp {

boost::filesystem::path determine_executable_location();
boost::filesystem::path determine_settings_directory_from_game();
boost::filesystem::path determine_settings_directory_from_tool();

}

#endif
