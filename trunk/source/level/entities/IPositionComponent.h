/***
 * hesperus: IPositionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IPOSITIONCOMPONENT
#define H_HESP_IPOSITIONCOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

namespace hesp {

class IPositionComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IPositionComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const Vector3d& position() const = 0;
	virtual void set_position(const Vector3d& position) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IPositionComponent> IPositionComponent_Ptr;

}

#endif
