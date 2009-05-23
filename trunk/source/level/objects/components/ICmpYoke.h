/***
 * hesperus: ICmpYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPYOKE
#define H_HESP_ICMPYOKE

#include <source/level/objects/base/IComponent.h>
#include <source/level/objects/base/IYoke.h>

namespace hesp {

class ICmpYoke : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets) = 0;

	//#################### PUBLIC METHODS ####################
public:
	static std::string static_type()	{ return "Yoke"; }
	std::string type() const			{ return "Yoke"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpYoke> ICmpYoke_Ptr;
typedef shared_ptr<const ICmpYoke> ICmpYoke_CPtr;

}

#endif