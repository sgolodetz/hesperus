/***
 * hesperus: EntitiesSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITIESSECTION
#define H_HESP_ENTITIESSECTION

#include <boost/filesystem/operations.hpp>

#include <ASXEngine.h>

#include <source/level/entities/EntityManager.h>

namespace hesp {

class EntitiesSection
{
	//#################### LOADING METHODS ####################
public:
	static EntityManager_Ptr load(std::istream& is, const boost::filesystem::path& baseDir);

	//#################### SAVING METHODS ####################
public:
	static void save(std::ostream& os, const EntityManager_Ptr& entityManager);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static Entity_Ptr load_entity(std::istream& is, const ASXEngine_Ptr& aiEngine, const boost::filesystem::path& baseDir);
	static void load_entity_properties(std::istream& is, Properties& properties);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static void save_entity(std::ostream& os, const Entity_Ptr& entity);
};

}

#endif
