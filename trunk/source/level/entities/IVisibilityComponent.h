/***
 * hesperus: IVisibilityComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IVISIBILITYCOMPONENT
#define H_HESP_IVISIBILITYCOMPONENT

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class IVisibilityComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IVisibilityComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void save(std::ostream& os) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IVisibilityComponent> IVisibilityComponent_Ptr;

}

#endif
