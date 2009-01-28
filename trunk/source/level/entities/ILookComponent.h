/***
 * hesperus: ILookComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ILOOKCOMPONENT
#define H_HESP_ILOOKCOMPONENT

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

namespace hesp {

class ILookComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ILookComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const Vector3d& look() const = 0;
	virtual void save(std::ostream& os) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ILookComponent> ILookComponent_Ptr;

}

#endif
