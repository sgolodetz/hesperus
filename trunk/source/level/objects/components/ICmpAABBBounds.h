/***
 * hesperus: ICmpAABBBounds.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPAABBBOUNDS
#define H_HESP_ICMPAABBBOUNDS

#include <vector>

#include <source/level/objects/base/IComponent.h>

namespace hesp {

class ICmpAABBBounds : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::vector<int>& aabb_indices() const = 0;
	virtual int cur_aabb_index() const = 0;
	virtual int pose() const = 0;
	virtual void set_pose(int pose) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Bounds"; }
	static std::string static_group_type()	{ return "Bounds"; }

	std::string own_type() const			{ return "AABBBounds"; }
	static std::string static_own_type()	{ return "AABBBounds"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpAABBBounds> ICmpAABBBounds_Ptr;
typedef shared_ptr<const ICmpAABBBounds> ICmpAABBBounds_CPtr;

}

#endif
