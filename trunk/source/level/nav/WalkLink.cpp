/***
 * hesperus: WalkLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "WalkLink.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
WalkLink::WalkLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
:	NavLink(sourcePoly, destPoly), m_edge(p1, p2)
{}

//#################### PUBLIC METHODS ####################
Vector3d WalkLink::dest_position() const
{
	return (m_edge.e1 + m_edge.e2) / 2;
}

NavLink_Ptr WalkLink::load(const std::string& line)
{
	// NYI
	throw 23;
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
