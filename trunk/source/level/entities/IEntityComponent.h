/***
 * hesperus: IEntityComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IENTITYCOMPONENT
#define H_HESP_IENTITYCOMPONENT

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class IEntityComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IEntityComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void save(std::ostream& os) const = 0;
};

}

#endif
