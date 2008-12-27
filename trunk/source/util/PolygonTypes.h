/***
 * hesperus: PolygonTypes.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_UTIL_POLYGONTYPES
#define H_HESP_UTIL_POLYGONTYPES

#include <string>

#include <source/math/geom/Polygon.h>
#include <source/math/vectors/TexturedVector3d.h>
#include <source/math/vectors/TexturedLitVector3d.h>

namespace hesp {

//#################### CLASSES ####################
struct TLPolygonInfo
{
	// TODO
};

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d, bool> CollisionPolygon;
typedef Polygon<TexturedVector3d, std::string> TexturedPolygon;
typedef Polygon<TexturedLitVector3d, TLPolygonInfo> TexturedLitPolygon;

}

#endif
