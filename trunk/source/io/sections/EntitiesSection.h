/***
 * hesperus: EntitiesSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITIESSECTION
#define H_HESP_ENTITIESSECTION

#include <source/level/entities/EntityManager.h>

namespace hesp {

struct EntitiesSection
{
	//#################### LOADING METHODS ####################
	static EntityManager_Ptr load(std::istream& is, const boost::filesystem::path& settingsDir);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const EntityManager_Ptr& entityManager);
};

}

#endif
