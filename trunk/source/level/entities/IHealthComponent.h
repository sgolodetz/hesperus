/***
 * hesperus: IHealthComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IHEALTHCOMPONENT
#define H_HESP_IHEALTHCOMPONENT

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class IHealthComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IHealthComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int health() const = 0;
	virtual void save(std::ostream& os) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IHealthComponent> IHealthComponent_Ptr;

}

#endif
