/***
 * hesperus: MinimusGotoPositionYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINIMUSGOTOPOSITIONYOKE
#define H_HESP_MINIMUSGOTOPOSITIONYOKE

#include <list>

#include <source/level/entities/Entity.h>
#include <source/level/entities/Yoke.h>

namespace hesp {

/**
This class represents a goto position yoke for the Minimus bot.
*/
class MinimusGotoPositionYoke : public Yoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	Entity_Ptr m_biped;
	std::vector<NavLink_Ptr> m_links;
	shared_ptr<std::list<int> > m_path;

	//#################### CONSTRUCTORS ####################
public:
	MinimusGotoPositionYoke(const Entity_Ptr& biped);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<EntityCommand_Ptr> generate_commands(UserInput& input, const std::vector<NavDataset_Ptr>& navDatasets);
};

}

#endif
