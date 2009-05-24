/***
 * hesperus: ICmpModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPMODELRENDER
#define H_HESP_ICMPMODELRENDER

#include <source/level/models/AnimationController.h>
#include <source/level/models/ModelManager.h>
#include "ICmpRender.h"

namespace hesp {

class ICmpModelRender : public ICmpRender
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual AnimationController_Ptr anim_controller() const = 0;
	virtual void set_model_manager(const ModelManager_Ptr& modelManager) = 0;
	virtual void set_skeleton() = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Render"; }
	static std::string static_group_type()	{ return "Render"; }

	std::string own_type() const			{ return "ModelRender"; }
	static std::string static_own_type()	{ return "ModelRender"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpModelRender> ICmpModelRender_Ptr;
typedef shared_ptr<const ICmpModelRender> ICmpModelRender_CPtr;

}

#endif