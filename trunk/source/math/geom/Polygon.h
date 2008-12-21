/***
 * hesperus: Polygon.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_GEOM_POLYGON
#define H_HESP_MATH_GEOM_POLYGON

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "Plane.h"

namespace hesp {

/**
This class template allows users to create polygons with a vertex type and auxiliary data type
of their choosing. For instance, they can create textured polygons by specifying a vertex type
with (u,v) texture coordinates and an auxiliary data type referencing the texture itself.
*/
template <typename Vertex, typename AuxiliaryData>
class Polygon
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<Polygon> Polygon_Ptr;

	//#################### NESTED CLASSES ####################
public:
	struct SplitResults
	{
		Polygon_Ptr back;
		Polygon_Ptr front;

		SplitResults(const Polygon_Ptr& back_, const Polygon_Ptr& front) : back(back_), front(front_) {}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Vertex> m_vertices;
	AuxiliaryData m_auxData;

	//#################### CONSTRUCTORS ####################
public:
	Polygon(const std::vector<Vertex>& vertices, const AuxiliaryData& auxData);

	//#################### PUBLIC METHODS ####################
public:
	const AuxiliaryData& auxiliary_data() const;
	Plane::CP classify_against(const Plane& plane) const;
	SplitResults split(const Plane& plane) const;
	Vertex& vertex(int i);
	const Vertex& vertex(int i) const;
};

}

#include "Polygon.tpp"

#endif
