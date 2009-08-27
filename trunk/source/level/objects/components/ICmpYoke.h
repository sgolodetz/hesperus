/***
 * hesperus: ICmpYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPYOKE
#define H_HESP_ICMPYOKE

#include <source/level/objects/base/IYoke.h>
#include <source/level/objects/base/ObjectComponent.h>

namespace hesp {

class ICmpYoke : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Yoke"; }
	static std::string static_group_type()	{ return "Yoke"; }

	std::string own_type() const			{ return "Yoke"; }
	static std::string static_own_type()	{ return "Yoke"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpYoke> ICmpYoke_Ptr;
typedef shared_ptr<const ICmpYoke> ICmpYoke_CPtr;

}

#endif
