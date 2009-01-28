/***
 * hesperus: ICollisionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICOLLISIONCOMPONENT
#define H_HESP_ICOLLISIONCOMPONENT

#include <iosfwd>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

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
	virtual void save(std::ostream& os) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICollisionComponent> ICollisionComponent_Ptr;

}

#endif
