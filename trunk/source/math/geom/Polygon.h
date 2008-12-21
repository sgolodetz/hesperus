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
template <typename Vert, typename AuxData>
class Polygon
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Vert> m_vertices;
	AuxData m_auxData;

	//#################### CONSTRUCTORS ####################
public:
	Polygon(const std::vector<Vert>& vertices, const AuxData& auxData);

	//#################### PUBLIC METHODS ####################
public:
	const AuxData& auxiliary_data() const;
	Vert& vertex(int i);
	const Vert& vertex(int i) const;
	int vertex_count() const;
};

}

#include "Polygon.tpp"

#endif
