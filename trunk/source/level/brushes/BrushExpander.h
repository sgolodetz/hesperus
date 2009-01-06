/***
 * hesperus: BrushExpander.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_BRUSHES_BRUSHEXPANDER
#define H_HESP_LEVEL_BRUSHES_BRUSHEXPANDER

#include <set>

#include <source/math/geom/AABB.h>
#include <source/math/geom/Plane.h>
#include <source/math/geom/UniquePlanePred.h>
#include <source/util/PolygonTypes.h>
#include "PolyhedralBrush.h"

namespace hesp {

class BrushExpander
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<CPAuxData> CPAuxData_Ptr;

	//#################### NESTED CLASSES ####################
private:
	struct BrushPlane
	{
		//#################### PUBLIC VARIABLES ####################
		Plane plane;
		CPAuxData_Ptr auxData;

		//#################### CONSTRUCTORS ####################
		explicit BrushPlane(const Plane& plane_, const CPAuxData_Ptr auxData_ = CPAuxData_Ptr())
		:	plane(plane_), auxData(auxData_)
		{}

		//#################### PUBLIC OPERATORS ####################
		bool operator<(const BrushPlane& rhs) const
		{
			const double angleTolerance = 0.5 * PI / 180;	// convert 0.5 degrees to radians
			const double distTolerance = 0.001;
			return UniquePlanePred(angleTolerance, distTolerance)(plane, rhs.plane);
		}
	};

	//#################### TYPEDEFS ####################
private:
	typedef PolyhedralBrush<CollisionPolygon> ColPolyBrush;
	typedef shared_ptr<ColPolyBrush> ColPolyBrush_Ptr;

	typedef std::set<BrushPlane> BrushPlaneSet;
	typedef shared_ptr<BrushPlaneSet> BrushPlaneSet_Ptr;

	//#################### PUBLIC METHODS ####################
public:
	static ColPolyBrush_Ptr expand_brush(const ColPolyBrush_Ptr& brush, const AABB3d& aabb);

	//#################### PRIVATE METHODS ####################
private:
	static BrushPlaneSet_Ptr determine_brush_planes(const ColPolyBrush_Ptr& brush);
	static BrushPlaneSet_Ptr expand_brush_planes(const BrushPlaneSet_Ptr& brushPlanes, const AABB3d& aabb);
};

}

#endif
