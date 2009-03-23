/***
 * hesperus: AnimationComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AnimationComponent.h"

#include <source/io/ModelFiles.h>
#include <source/io/util/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AnimationComponent::AnimationComponent(std::istream& is)
:	m_animController(new AnimationController)
{
	m_modelName = FieldIO::read_field(is, "GameModel");
}

//#################### PUBLIC METHODS ####################
const AnimationController_Ptr& AnimationComponent::anim_controller() const
{
	return m_animController;
}

const std::string& AnimationComponent::model_name() const
{
#if 0
	return m_modelName;
#else
	// FIXME: Load the appropriate models here once they've been created.
	static std::string s = "Test-15";
	return s;
#endif
}

void AnimationComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "GameModel", m_modelName);
}

}
