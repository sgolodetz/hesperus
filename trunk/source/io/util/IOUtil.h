/***
 * hesperus: IOUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IOUTIL
#define H_HESP_IOUTIL

#include <source/level/brushes/PolyhedralBrush.h>
#include <source/math/geom/Polygon.h>

namespace hesp {

struct IOUtil
{
	//#################### READING METHODS ####################
	static void read_counted_planes(std::istream& is, std::vector<Plane>& planes);
	template <typename Poly> static void read_counted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);
	static Plane read_plane(const std::string& line, const std::string& n = "");
	template <typename Vert, typename AuxData> static shared_ptr<Polygon<Vert,AuxData> > read_polygon(const std::string& line, const std::string& n = "");
	static void read_planes(std::istream& is, std::vector<Plane>& planes);
	template <typename Vert, typename AuxData> static void read_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead);
	template <typename Poly> static shared_ptr<PolyhedralBrush<Poly> > read_polyhedral_brush(std::istream& is);
	static void read_uncounted_planes(std::istream& is, std::vector<Plane>& planes);
	template <typename Poly> static void read_uncounted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);

	//#################### WRITING METHODS ####################
	static void write_planes(std::ostream& os, const std::vector<Plane>& planes, bool writeCount);
	template <typename Vert, typename AuxData> static void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount);
	template <typename Poly> static void write_polyhedral_brush(std::ostream& os, const PolyhedralBrush<Poly>& brush);
};

}

#include "IOUtil.tpp"

#endif
