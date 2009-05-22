/***
 * hesperus: ICmpRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPRENDER
#define H_HESP_ICMPRENDER

#include <source/level/models/AnimationController.h>
#include <source/level/objects/base/IComponent.h>

namespace hesp {

class ICmpRender : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual AnimationController_Ptr anim_controller() const = 0;
	virtual const std::string& model_name() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	static std::string static_type()	{ return "Render"; }
	std::string type() const			{ return "Render"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpRender> ICmpRender_Ptr;
typedef shared_ptr<const ICmpRender> ICmpRender_CPtr;

}

#endif
