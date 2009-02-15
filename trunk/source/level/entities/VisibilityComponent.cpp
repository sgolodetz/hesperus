/***
 * hesperus: VisibilityComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VisibilityComponent.h"

#include <source/io/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
VisibilityComponent::VisibilityComponent(std::istream& is)
{
	m_modelFilename = FieldIO::read_field(is, "GameModel");
}

//#################### PUBLIC METHODS ####################
void VisibilityComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "GameModel", m_modelFilename);
}

}
