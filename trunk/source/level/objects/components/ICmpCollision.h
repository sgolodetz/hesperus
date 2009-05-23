/***
 * hesperus: ICmpCollision.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPCOLLISION
#define H_HESP_ICMPCOLLISION

#include <vector>

#include <source/level/objects/base/IComponent.h>

namespace hesp {

class ICmpCollision : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::vector<int>& aabb_indices() const = 0;
	virtual int pose() const = 0;
	virtual void set_pose(int pose) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Collision"; }
	static std::string static_group_type()	{ return "Collision"; }

	std::string own_type() const			{ return "Collision"; }
	static std::string static_own_type()	{ return "Collision"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpCollision> ICmpCollision_Ptr;
typedef shared_ptr<const ICmpCollision> ICmpCollision_CPtr;

}

#endif
