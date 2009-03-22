/***
 * hesperus: AnimationComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AnimationComponent.h"

#include <source/io/FieldIO.h>
#include <source/io/ModelFilesUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AnimationComponent::AnimationComponent(std::istream& is)
{
	m_modelName = FieldIO::read_field(is, "GameModel");
}

//#################### PUBLIC METHODS ####################
const Model_Ptr& AnimationComponent::model() const
{
	if(!m_model)
	{
		// FIXME: Load the appropriate models here once they've been created.
		m_model = ModelFilesUtil::load_model("Test-15");
	}
	return m_model;
}

void AnimationComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "GameModel", m_modelName);
}

}
