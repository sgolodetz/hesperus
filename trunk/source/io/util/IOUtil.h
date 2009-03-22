/***
 * hesperus: IOUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SECTIONUTIL
#define H_HESP_SECTIONUTIL

#include <source/level/brushes/PolyhedralBrush.h>
#include <source/math/geom/Polygon.h>

namespace hesp {

struct IOUtil
{
	//#################### LOADING SUPPORT METHODS ####################
	template <typename Poly> static void load_counted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);
	template <typename Vert, typename AuxData> static shared_ptr<Polygon<Vert,AuxData> > load_polygon(const std::string& line, const std::string& n = "");
	template <typename Vert, typename AuxData> static void load_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead = INT_MAX);
	template <typename Poly> static shared_ptr<PolyhedralBrush<Poly> > load_polyhedral_brush(std::istream& is);
	template <typename Poly> static void load_uncounted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);

	//#################### SAVING SUPPORT METHODS ####################
	template <typename Vert, typename AuxData> static void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount);
	template <typename Poly> static void write_polyhedral_brush(std::ostream& os, const PolyhedralBrush<Poly>& brush);
};

}

#include "IOUtil.tpp"

#endif
