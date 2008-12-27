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
typedef shared_ptr<CollisionPolygon> CollisionPolygon_Ptr;
typedef shared_ptr<const CollisionPolygon> CollisionPolygon_CPtr;

typedef Polygon<TexturedVector3d, std::string> TexturedPolygon;
typedef shared_ptr<TexturedPolygon> TexturedPolygon_Ptr;
typedef shared_ptr<const TexturedPolygon> TexturedPolygon_CPtr;

typedef Polygon<TexturedLitVector3d, TLPolygonInfo> TexturedLitPolygon;
typedef shared_ptr<TexturedLitPolygon> TexturedLitPolygon_Ptr;
typedef shared_ptr<const TexturedLitPolygon> TexturedLitPolygon_CPtr;

}

#endif
