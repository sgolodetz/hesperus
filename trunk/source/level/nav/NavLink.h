/***
 * hesperus: NavLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVLINK
#define H_HESP_LEVEL_NAV_NAVLINK

#include <ostream>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/LineSegment.h>

namespace hesp {

class NavLink
{
	//#################### PROTECTED VARIABLES ####################
protected:
	int m_sourcePoly, m_destPoly;
	LineSegment3d m_sourceSegment;

	//#################### CONSTRUCTORS ####################
public:
	NavLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	m_sourcePoly(sourcePoly), m_destPoly(destPoly), m_sourceSegment(p1,p2)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~NavLink() {}

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual std::string link_name() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	void output(std::ostream& os) const
	{
		os << link_name() << ' ' << m_sourcePoly << ' ' << m_destPoly << ' ' << m_sourceSegment;
	}

	// TODO: <TraversalFunctor> traverse(Vector3d sourcePosition)
};

class StepDownLink : public NavLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepDownLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	NavLink(sourcePoly, destPoly, p1, p2)
	{}

	//#################### PUBLIC METHODS ####################
public:
	std::string link_name() const
	{
		return "StepDown";
	}
};

class StepUpLink : public NavLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepUpLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	NavLink(sourcePoly, destPoly, p1, p2)
	{}

	//#################### PUBLIC METHODS ####################
public:
	std::string link_name() const
	{
		return "StepUp";
	}
};

class WalkLink : public NavLink
{
	//#################### CONSTRUCTORS ####################
public:
	WalkLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	NavLink(sourcePoly, destPoly, p1, p2)
	{}

	//#################### PUBLIC METHODS ####################
public:
	std::string link_name() const
	{
		return "Walk";
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavLink> NavLink_Ptr;
typedef shared_ptr<const NavLink> NavLink_CPtr;

}

#endif
