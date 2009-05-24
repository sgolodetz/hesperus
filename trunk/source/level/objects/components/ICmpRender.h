/***
 * hesperus: ICmpRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPRENDER
#define H_HESP_ICMPRENDER

#include <source/level/models/AnimationController.h>
#include <source/level/models/ModelManager.h>
#include <source/level/objects/base/IComponent.h>

namespace hesp {

class ICmpRender : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual AnimationController_Ptr anim_controller() const = 0;
	virtual void render() const = 0;
	virtual void set_model_manager(const ModelManager_Ptr& modelManager) = 0;
	virtual void set_skeleton() = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Render"; }
	static std::string static_group_type()	{ return "Render"; }

	std::string own_type() const			{ return "Render"; }
	static std::string static_own_type()	{ return "Render"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpRender> ICmpRender_Ptr;
typedef shared_ptr<const ICmpRender> ICmpRender_CPtr;

}

#endif
