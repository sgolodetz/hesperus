/***
 * hesperus: MinimusGotoPositionYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINIMUSGOTOPOSITIONYOKE
#define H_HESP_MINIMUSGOTOPOSITIONYOKE

#include <source/level/objects/base/IYoke.h>

namespace hesp {

/**
This class represents a goto position yoke for the Minimus bot.
*/
class MinimusGotoPositionYoke : public IYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	ObjectManager *m_objectManager;

	Vector3d m_dest;
	std::vector<NavLink_Ptr> m_links;
	shared_ptr<std::list<int> > m_path;

	//#################### CONSTRUCTORS ####################
public:
	MinimusGotoPositionYoke(const ObjectID& objectID, ObjectManager *objectManager, const Vector3d& dest);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
};

}

#endif
