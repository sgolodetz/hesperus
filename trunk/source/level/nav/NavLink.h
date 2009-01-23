/***
 * hesperus: NavLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVLINK
#define H_HESP_NAVLINK

#include <ostream>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

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

//#################### TYPEDEFS ####################
typedef shared_ptr<NavLink> NavLink_Ptr;
typedef shared_ptr<const NavLink> NavLink_CPtr;

}

#endif
