/***
 * hesperus: YokeUserBiped.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_YOKEUSERBIPED
#define H_HESP_YOKEUSERBIPED

#include "IYoke.h"

namespace hesp {

/**
This class represents a yoke that allows the user to control a biped.
Bipeds can walk, run, jump, crouch, etc.
*/
class YokeUserBiped : public IYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	ObjectManager *m_objectManager;

	//#################### CONSTRUCTORS ####################
public:
	YokeUserBiped(const ObjectID& objectID, ObjectManager *objectManager);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
};

}

#endif
