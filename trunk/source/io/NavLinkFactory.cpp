/***
 * hesperus: NavLinkFactory.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavLinkFactory.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavLinkFactory::NavLinkFactory()
{
	m_loaders["StepDown"] = &StepDownLink::load;
	m_loaders["StepUp"] = &StepUpLink::load;
	m_loaders["Walk"] = &WalkLink::load;
}

//#################### PUBLIC METHODS ####################
NavLink_Ptr NavLinkFactory::construct_navlink(const std::string& line) const
{
	// NYI
	throw 23;
}

}
