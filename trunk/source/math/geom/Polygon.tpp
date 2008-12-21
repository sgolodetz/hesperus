/***
 * hesperus: Polygon.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#define Polygon_HEADER	template <typename Vertex, typename AuxiliaryData>
#define Polygon_THIS	Polygon<Vertex,AuxiliaryData>

namespace hesp {

//#################### CONSTRUCTORS ####################
Polygon_HEADER
Polygon_THIS::Polygon(const std::vector<Vertex>& vertices, const AuxiliaryData& auxData)
:	m_vertices(vertices), m_auxData(auxData)
{}

//#################### PUBLIC METHODS ####################
/**
Returns the auxiliary data stored with the polygon.

@return	As stated
*/
Polygon_HEADER
const AuxiliaryData& Polygon_THIS::auxiliary_data() const
{
	return m_auxData;
}

Polygon_HEADER
Plane::CP Polygon_THIS::classify_against(const Plane& plane) const
{
	// NYI
	throw 23;
}

/**
Splits the polygon across the specified plane.

@param plane	The plane across which to split the polygon
@return			A SplitResults object containing the results of the split
*/
Polygon_HEADER
typename Polygon_THIS::SplitResults Polygon_THIS::split(const Plane& plane) const
{
	// NYI
	throw 23;
}

/**
Returns the i'th vertex of the polygon.

@return	As stated
*/
Polygon_HEADER
Vertex& Polygon_THIS::vertex(int i)
{
	return m_vertices[i];
}

/**
Returns the i'th vertex of the polygon.

@return	As stated
*/
Polygon_HEADER
const Vertex& Polygon_THIS::vertex(int i) const
{
	return m_vertices[i];
}

}

#undef Polygon_THIS
#undef Polygon_HEADER
