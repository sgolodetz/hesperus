/***
 * hesperus: CmpBipedAnimChooser.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPBIPEDANIMCHOOSER
#define H_HESP_CMPBIPEDANIMCHOOSER

#include "ICmpBipedAnimChooser.h"

namespace hesp {

class CmpBipedAnimChooser : public ICmpBipedAnimChooser
{
	//#################### PRIVATE VARIABLES ####################
private:
	bool m_runFlag;
	bool m_walkFlag;

	//#################### CONSTRUCTORS ####################
public:
	CmpBipedAnimChooser();

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	std::string choose_animation(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
	std::pair<std::string,Properties> save() const;
	void set_run_flag();
	void set_walk_flag();

	//#################### PRIVATE METHODS ####################
private:
	void reset_flags();
};

}

#endif
