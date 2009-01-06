/***
 * hesperus: BrushExpander.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_BRUSHES_BRUSHEXPANDER
#define H_HESP_LEVEL_BRUSHES_BRUSHEXPANDER

#include <set>

#include <source/math/geom/Plane.h>
#include <source/util/PolygonTypes.h>
#include "PolyhedralBrush.h"

namespace hesp {

class BrushExpander
{
	//#################### NESTED CLASSES ####################
private:
	struct BrushPlane
	{
		Plane plane;
		shared_ptr<CPAuxData> auxData;
	};

	//#################### TYPEDEFS ####################
private:
	typedef PolyhedralBrush<CollisionPolygon> ColPolyBrush;
	typedef shared_ptr<ColPolyBrush> ColPolyBrush_Ptr;

	typedef std::set<BrushPlane> BrushPlaneSet;
	typedef shared_ptr<BrushPlaneSet> BrushPlaneSet_Ptr;

	//#################### PUBLIC METHODS ####################
public:
	static ColPolyBrush_Ptr expand_brush(const ColPolyBrush_Ptr& brush);

	//#################### PRIVATE METHODS ####################
private:
	static BrushPlaneSet_Ptr determine_brush_planes(const ColPolyBrush_Ptr& brush);
};

}

#endif
