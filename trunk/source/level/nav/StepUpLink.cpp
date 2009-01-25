/***
 * hesperus: StepUpLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "StepUpLink.h"

#include <sstream>

namespace hesp {

//#################### CONSTRUCTORS ####################
StepUpLink::StepUpLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
:	StepLink(sourcePoly, destPoly, s1, s2, d1, d2)
{}

StepUpLink::StepUpLink(int sourcePoly, int destPoly, const LineSegment3d& sourceEdge, const LineSegment3d& destEdge)
:	StepLink(sourcePoly, destPoly, sourceEdge, destEdge)
{}

//#################### PUBLIC METHODS ####################
std::string StepUpLink::link_name() const
{
	return "StepUp";
}

NavLink_Ptr StepUpLink::load(const std::string& data)
{
	std::stringstream ss;
	ss << data;

	std::ios_base::fmtflags oldFlags = ss.flags();
	ss.setf(std::ios_base::skipws);

	int sourcePoly, destPoly;
	LineSegment3d sourceEdge, destEdge;

	ss >> sourcePoly >> destPoly >> sourceEdge >> destEdge;

	ss.flags(oldFlags);

	return NavLink_Ptr(new StepUpLink(sourcePoly, destPoly, sourceEdge, destEdge));
}

}