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
	static EntityManager_Ptr load(std::istream& is, const std::vector<AABB3d>& aabbs, const std::map<std::string,std::string>& propertyTypes, const boost::filesystem::path& baseDir);

	//#################### SAVING METHODS ####################
public:
	static void save(std::ostream& os, const EntityManager_Ptr& entityManager);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static std::string intarray_to_string(const std::vector<int>& arr);
	static Entity_Ptr load_entity(std::istream& is, const ASXEngine_Ptr& aiEngine, const std::map<std::string,std::string>& propertyTypes, const boost::filesystem::path& baseDir);
	static void load_entity_properties(std::istream& is, Properties& properties, const std::map<std::string,std::string>& propertyTypes);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static void save_entity(std::ostream& os, const Entity_Ptr& entity, const std::map<std::string,std::string>& propertyTypes);
	static std::vector<int> string_to_intarray(const std::string& s);
};

}

#endif
