/***
 * hesperus: EntitiesFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITIESFILE
#define H_HESP_ENTITIESFILE

#include <string>

#include <boost/filesystem/operations.hpp>

#include <source/level/entities/EntityManager.h>

namespace hesp {

struct EntitiesFile
{
	//#################### LOADING METHODS ####################
	static EntityManager_Ptr load(const std::string& filename, const boost::filesystem::path& baseDir);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const EntityManager_Ptr& entityManager);
};

}

#endif
