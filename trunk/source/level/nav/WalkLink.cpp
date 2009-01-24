/***
 * hesperus: WalkLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "WalkLink.h"

#include <sstream>

namespace hesp {

//#################### CONSTRUCTORS ####################
WalkLink::WalkLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
:	NavLink(sourcePoly, destPoly), m_edge(p1, p2)
{}

WalkLink::WalkLink(int sourcePoly, int destPoly, const LineSegment3d& edge)
:	NavLink(sourcePoly, destPoly), m_edge(edge)
{}

//#################### PUBLIC METHODS ####################
Vector3d WalkLink::dest_position() const
{
	return (m_edge.e1 + m_edge.e2) / 2;
}

NavLink_Ptr WalkLink::load(const std::string& data)
{
	std::stringstream ss;
	ss << data;

	std::ios_base::fmtflags oldFlags = ss.flags();
	ss.setf(std::ios_base::skipws);

	int sourcePoly, destPoly;
	LineSegment3d edge;

	ss >> sourcePoly >> destPoly >> edge;

	ss.flags(oldFlags);

	return NavLink_Ptr(new WalkLink(sourcePoly, destPoly, edge));
}

void WalkLink::output(std::ostream& os) const
{
	os << "Walk " << m_sourcePoly << ' ' << m_destPoly << ' ' << m_edge;
}

Vector3d WalkLink::source_position() const
{
	return (m_edge.e1 + m_edge.e2) / 2;
}

}
