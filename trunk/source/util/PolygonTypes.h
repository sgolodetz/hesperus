/***
 * hesperus: PolygonTypes.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_UTIL_POLYGONTYPES
#define H_HESP_UTIL_POLYGONTYPES

#include <iosfwd>
#include <string>

#include <source/math/geom/Polygon.h>
#include <source/math/vectors/TexturedVector3d.h>
#include <source/math/vectors/TexturedLitVector3d.h>

namespace hesp {

//#################### CLASSES ####################
/*
TODO

We will eventually want to store the following sorts of things as well:

- type of surface (for sound effects)
*/
struct CPAuxData
{
	bool walkable;

	CPAuxData()
	:	walkable(false)
	{}

	explicit CPAuxData(bool walkable_)
	:	walkable(walkable_)
	{}
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const CPAuxData& rhs);
std::istream& operator>>(std::istream& is, CPAuxData& rhs);

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d, CPAuxData> CollisionPolygon;
typedef shared_ptr<CollisionPolygon> CollisionPolygon_Ptr;
typedef shared_ptr<const CollisionPolygon> CollisionPolygon_CPtr;

typedef Polygon<TexturedVector3d, std::string> TexturedPolygon;
typedef shared_ptr<TexturedPolygon> TexturedPolygon_Ptr;
typedef shared_ptr<const TexturedPolygon> TexturedPolygon_CPtr;

typedef Polygon<TexturedLitVector3d, std::string> TexturedLitPolygon;
typedef shared_ptr<TexturedLitPolygon> TexturedLitPolygon_Ptr;
typedef shared_ptr<const TexturedLitPolygon> TexturedLitPolygon_CPtr;

}

#endif
