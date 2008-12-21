/***
 * hesperus: Polygon.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#define Polygon_HEADER	template <typename Vert, typename AuxData>
#define Polygon_THIS	Polygon<Vert,AuxData>

namespace hesp {

//#################### CONSTRUCTORS ####################
Polygon_HEADER
Polygon_THIS::Polygon(const std::vector<Vert>& vertices, const AuxData& auxData)
:	m_vertices(vertices), m_auxData(auxData)
{}

//#################### PUBLIC METHODS ####################
/**
Returns the auxiliary data stored with the polygon.

@return	As stated
*/
Polygon_HEADER
const AuxData& Polygon_THIS::auxiliary_data() const
{
	return m_auxData;
}

/**
Returns the i'th vertex of the polygon.

@return	As stated
*/
Polygon_HEADER
Vert& Polygon_THIS::vertex(int i)
{
	return m_vertices[i];
}

/**
Returns the i'th vertex of the polygon.

@return	As stated
*/
Polygon_HEADER
const Vert& Polygon_THIS::vertex(int i) const
{
	return m_vertices[i];
}

/**
Returns the number of vertices in the polygon.

@return	As stated
*/
Polygon_HEADER
int Polygon_THIS::vertex_count() const
{
	return static_cast<int>(m_vertices.size());
}

}

#undef Polygon_THIS
#undef Polygon_HEADER
