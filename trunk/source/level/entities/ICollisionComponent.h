/***
 * hesperus: ICollisionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICOLLISIONCOMPONENT
#define H_HESP_ICOLLISIONCOMPONENT

#include <vector>

namespace hesp {

class ICollisionComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ICollisionComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::vector<int>& aabb_indices() const = 0;
	virtual int pose() const = 0;
};

}

#endif
