/***
 * hesperus: StepLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "StepLink.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
StepLink::StepLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
:	NavLink(sourcePoly, destPoly), m_sourceEdge(s1,s2), m_destEdge(d1,d2)
{}

StepLink::StepLink(int sourcePoly, int destPoly, const LineSegment3d& sourceEdge, const LineSegment3d& destEdge)
:	NavLink(sourcePoly, destPoly), m_sourceEdge(sourceEdge), m_destEdge(destEdge)
{}

//#################### PUBLIC METHODS ####################
Vector3d StepLink::dest_position() const
{
	return (m_destEdge.e1 + m_destEdge.e2) / 2;
}

Vector3d_Ptr StepLink::hit_test(const Vector3d& s, const Vector3d& d) const
{
	// NYI
	return Vector3d_Ptr();
}

void StepLink::output(std::ostream& os) const
{
	os << link_name() << ' ' << m_sourcePoly << ' ' << m_destPoly << ' ' << m_sourceEdge << ' ' << m_destEdge;
}

Vector3d StepLink::source_position() const
{
	return (m_sourceEdge.e1 + m_sourceEdge.e2) / 2;
}

}
