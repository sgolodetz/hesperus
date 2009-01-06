/***
 * hesperus: BrushExpander.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BrushExpander.h"

namespace hesp {

//#################### NESTED CLASSES ####################
struct BrushPlane
{
	Plane plane;
	CollisionPolygon::AuxData auxData;
};

//#################### PUBLIC METHODS ####################
BrushExpander::ColPolyBrush_Ptr BrushExpander::expand_brush(const ColPolyBrush_Ptr& brush)
{
	// NYI
	throw 23;
}

//#################### PRIVATE METHODS ####################
std::set<BrushExpander::BrushPlane> BrushExpander::determine_brush_planes(const ColPolyBrush_Ptr& brush)
{
	// NYI
	throw 23;
}

}
