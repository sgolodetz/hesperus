/***
 * hesperus: AnimationComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AnimationComponent.h"

#include <source/io/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AnimationComponent::AnimationComponent(std::istream& is)
{
	m_modelFilename = FieldIO::read_field(is, "GameModel");
}

//#################### PUBLIC METHODS ####################
void AnimationComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "GameModel", m_modelFilename);
}

}
