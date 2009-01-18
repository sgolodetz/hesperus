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

	//#################### CONSTRUCTORS ####################
public:
	NavLink(int sourcePoly, int destPoly)
	:	m_sourcePoly(sourcePoly), m_destPoly(destPoly)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~NavLink() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual Vector3d dest_position() const = 0;
	virtual void output(std::ostream& os) const = 0;
	virtual Vector3d source_position() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	int dest_poly() const		{ return m_destPoly; }
	int source_poly() const		{ return m_sourcePoly; }

	// TODO: <TraversalFunctor> traverse(Vector3d sourcePosition)
};

class StepLink : public NavLink
{
	//#################### PROTECTED VARIABLES ####################
protected:
	LineSegment3d m_sourceEdge, m_destEdge;

	//#################### CONSTRUCTORS ####################
public:
	StepLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
	:	NavLink(sourcePoly, destPoly), m_sourceEdge(s1,s2), m_destEdge(d1,d2)
	{}

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual std::string link_name() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	Vector3d dest_position() const
	{
		return (m_destEdge.e1 + m_destEdge.e2) / 2;
	}

	void output(std::ostream& os) const
	{
		os << link_name() << ' ' << m_sourcePoly << ' ' << m_destPoly << ' ' << m_sourceEdge << ' ' << m_destEdge;
	}

	Vector3d source_position() const
	{
		return (m_sourceEdge.e1 + m_sourceEdge.e2) / 2;
	}
};

class StepDownLink : public StepLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepDownLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
	:	StepLink(sourcePoly, destPoly, s1, s2, d1, d2)
	{}

	//#################### PUBLIC METHODS ####################
public:
	std::string link_name() const
	{
		return "StepDown";
	}
};

class StepUpLink : public StepLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepUpLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
	:	StepLink(sourcePoly, destPoly, s1, s2, d1, d2)
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
	//#################### PRIVATE VARIABLES ####################
private:
	LineSegment3d m_edge;

	//#################### CONSTRUCTORS ####################
public:
	WalkLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	NavLink(sourcePoly, destPoly), m_edge(p1, p2)
	{}

	//#################### PUBLIC METHODS ####################
public:
	Vector3d dest_position() const
	{
		return (m_edge.e1 + m_edge.e2) / 2;
	}

	void output(std::ostream& os) const
	{
		os << "Walk " << m_sourcePoly << ' ' << m_destPoly << ' ' << m_edge;
	}

	Vector3d source_position() const
	{
		return (m_edge.e1 + m_edge.e2) / 2;
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavLink> NavLink_Ptr;
typedef shared_ptr<const NavLink> NavLink_CPtr;

}

#endif
