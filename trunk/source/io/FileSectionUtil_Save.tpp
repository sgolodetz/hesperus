/***
 * hesperus: FileSectionUtil_Save.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### SAVING METHODS ####################
/**
Saves a polygon section to the specified std::ostream.

@param os			The std::ostream
@param sectionName	The name to give the section in the file (e.g. Polygons, Portals, etc.)
@param polygons		The array of polygons
*/
template <typename Poly>
void FileSectionUtil::save_polygons_section(std::ostream& os, const std::string& sectionName, const std::vector<shared_ptr<Poly> >& polygons)
{
	os << sectionName << '\n';
	os << "{\n";
	write_polygons(os, polygons, true);
	os << "}\n";
}

//#################### SAVING SUPPORT METHODS ####################
/**
Writes a sequence of polygons to a stream, one per line.

@param os			The stream to which to write
@param polygons		The polygons to write to the stream
@param writeCount	Whether the polygon count should be output to the stream first
*/
template <typename Vert, typename AuxData>
void FileSectionUtil::write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	if(writeCount) os << polygons.size() << '\n';
	for(PolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		const Poly& curPoly = **it;
		int vertCount = curPoly.vertex_count();
		os << vertCount << ' ';
		for(int j=0; j<vertCount; ++j)
		{
			os << curPoly.vertex(j) << ' ';
		}
		os << curPoly.auxiliary_data() << '\n';
	}
}

/**
Writes a polyhedral brush to a std::ostream.

@param os		The std::ostream
@param brush	The polyhedral brush
*/
template <typename Poly>
void FileSectionUtil::write_polyhedral_brush(std::ostream& os, const PolyhedralBrush<Poly>& brush)
{
	os << "{\n";
	os << brush.bounds().minimum() << ' ' << brush.bounds().maximum() << '\n';
	write_polygons(os, brush.faces(), true);
	os << "}\n";
}

}
