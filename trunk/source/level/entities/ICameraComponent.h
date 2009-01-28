/***
 * hesperus: ICameraComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICAMERACOMPONENT
#define H_HESP_ICAMERACOMPONENT

#include <iosfwd>

#include <source/camera/Camera.h>

namespace hesp {

class ICameraComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ICameraComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual Camera& camera() = 0;
	virtual const Camera& camera() const = 0;
	virtual void save(std::ostream& os) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICameraComponent> ICameraComponent_Ptr;

}

#endif
