/***
 * hesperus: StepDownLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "StepDownLink.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
StepDownLink::StepDownLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
:	StepLink(sourcePoly, destPoly, s1, s2, d1, d2)
{}

//#################### PUBLIC METHODS ####################
std::string StepDownLink::link_name() const
{
	return "StepDown";
}

NavLink_Ptr StepDownLink::load(const std::string& line)
{
	// NYI
	throw 23;
}

}
