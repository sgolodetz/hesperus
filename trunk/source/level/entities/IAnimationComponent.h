/***
 * hesperus: IAnimationComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IANIMATIONCOMPONENT
#define H_HESP_IANIMATIONCOMPONENT

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/models/AnimationController.h>

namespace hesp {

class IAnimationComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IAnimationComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const AnimationController_Ptr& anim_controller() const = 0;
	virtual const std::string& model_name() const = 0;
	virtual void save(std::ostream& os) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IAnimationComponent> IAnimationComponent_Ptr;

}

#endif
