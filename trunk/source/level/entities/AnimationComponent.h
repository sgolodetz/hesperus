/***
 * hesperus: AnimationComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ANIMATIONCOMPONENT
#define H_HESP_ANIMATIONCOMPONENT

#include <string>

#include "IAnimationComponent.h"

namespace hesp {

class AnimationComponent : public IAnimationComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	AnimationController_Ptr m_animController;
	std::string m_modelName;

	//#################### CONSTRUCTORS ####################
public:
	AnimationComponent(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	const AnimationController_Ptr& anim_controller() const;
	const std::string& model_name() const;
	void save(std::ostream& os) const;
};

}

#endif
