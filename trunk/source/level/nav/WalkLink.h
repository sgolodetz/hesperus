/***
 * hesperus: WalkLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_WALKLINK
#define H_HESP_WALKLINK

#include <source/math/geom/LineSegment.h>
#include "NavLink.h"

namespace hesp {

class WalkLink : public NavLink
{
	//#################### PRIVATE VARIABLES ####################
private:
	LineSegment3d m_edge;

	//#################### CONSTRUCTORS ####################
public:
	WalkLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2);
	WalkLink(int sourcePoly, int destPoly, const LineSegment3d& edge);

	//#################### PUBLIC METHODS ####################
public:
	Vector3d dest_position() const;
	static NavLink_Ptr load(const std::string& data);
	void output(std::ostream& os) const;
	Vector3d source_position() const;
};

}

#endif
