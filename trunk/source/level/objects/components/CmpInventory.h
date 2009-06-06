/***
 * hesperus: CmpInventory.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPINVENTORY
#define H_HESP_CMPINVENTORY

#include <set>

#include "ICmpInventory.h"

namespace hesp {

class CmpInventory : public ICmpInventory
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::set<ObjectID> m_objects;
	std::map<std::string,int> m_consumables;				// the amounts of various types of consumable (e.g. pistol bullets) currently being held

	bool m_groupsDirty;										// do the usable groups need updating?
	std::map<std::string,std::set<ObjectID> > m_groups;		// which usables of each group (e.g. pistols) are currently being held

	//#################### CONSTRUCTORS ####################
public:
	CmpInventory(const std::set<ObjectID>& objects, const std::map<std::string,int>& consumables);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void add_consumables(const std::string& type, int amount);
	void add_object(const ObjectID& objectID);
	void destroy_consumables(const std::string& type, int amount);
	void remove_object(const ObjectID& objectID);
	std::pair<std::string,Properties> save() const;

	//#################### PRIVATE METHODS ####################
private:
	void update_groups();
};

}

#endif
