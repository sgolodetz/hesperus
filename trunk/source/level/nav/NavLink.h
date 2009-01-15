/***
 * hesperus: NavLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVLINK
#define H_HESP_LEVEL_NAV_NAVLINK

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

namespace hesp {

class NavLink
{
	//#################### PROTECTED VARIABLES ####################
protected:
	int m_mapIndex;
	int m_destPoly;
	Vector3d m_p1, m_p2;

	//#################### CONSTRUCTORS ####################
public:
	NavLink(int mapIndex, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	m_mapIndex(mapIndex), m_destPoly(destPoly), m_p1(p1), m_p2(p2)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~NavLink() {}

	//#################### PUBLIC METHODS ####################
public:
	// TODO: Vector3d traverse(Vector3d oldPosition)
};

class StepDownLink : public NavLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepDownLink(int mapIndex, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	NavLink(mapIndex, destPoly, p1, p2)
	{}
};

class StepUpLink : public NavLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepUpLink(int mapIndex, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	NavLink(mapIndex, destPoly, p1, p2)
	{}
};

class WalkLink : public NavLink
{
	//#################### CONSTRUCTORS ####################
public:
	WalkLink(int mapIndex, int destPoly, const Vector3d& p1, const Vector3d& p2)
	:	NavLink(mapIndex, destPoly, p1, p2)
	{}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavLink> NavLink_Ptr;
typedef shared_ptr<const NavLink> NavLink_CPtr;

}

#endif
