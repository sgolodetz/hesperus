/***
 * hesperus: StepLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_STEPLINK
#define H_HESP_STEPLINK

#include <source/math/geom/LineSegment.h>
#include "NavLink.h"

namespace hesp {

class StepLink : public NavLink
{
	//#################### PROTECTED VARIABLES ####################
protected:
	LineSegment3d m_sourceEdge, m_destEdge;

	//#################### CONSTRUCTORS ####################
public:
	StepLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2);

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual std::string link_name() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	Vector3d dest_position() const;
	void output(std::ostream& os) const;
	Vector3d source_position() const;
};

}

#endif
