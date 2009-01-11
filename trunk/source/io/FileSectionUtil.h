/***
 * hesperus: FileSectionUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_FILESECTIONUTIL
#define H_HESP_IO_FILESECTIONUTIL

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/brushes/PolyhedralBrush.h>
#include <source/level/bsp/BSPTree.h>
#include <source/level/lighting/Light.h>
#include <source/level/onionbsp/OnionTree.h>
#include <source/level/portals/Portal.h>
#include <source/level/vis/VisTable.h>

namespace hesp {

class FileSectionUtil
{
	//#################### LOADING METHODS ####################
public:
	static std::string load_lightmap_prefix_section(std::istream& is);
	static std::vector<Light> load_lights_section(std::istream& is);
	static OnionTree_Ptr load_onion_tree_section(std::istream& is);
	template <typename Poly> static void load_polygons_section(std::istream& is, const std::string& sectionName, std::vector<shared_ptr<Poly> >& polygons);
	template <typename Poly> static shared_ptr<PolyhedralBrush<Poly> > load_polyhedral_brush_section(std::istream& is);
	static BSPTree_Ptr load_tree_section(std::istream& is);
	static LeafVisTable_Ptr load_vis_section(std::istream& is);

	//#################### SAVING METHODS ####################
public:
	static void save_lightmap_prefix_section(std::ostream& os, const std::string& lightmapPrefix);
	//static void save_lights_section(std::ostream& os, const std::vector<Light>& lights);
	static void save_onion_tree_section(std::ostream& os, const OnionTree_Ptr& tree);
	template <typename Poly> static void save_polygons_section(std::ostream& os, const std::string& sectionName, const std::vector<shared_ptr<Poly> >& polygons);
	template <typename Poly> static void save_polyhedral_brush_section(std::ostream& os, const shared_ptr<PolyhedralBrush<Poly> >& brush);
	static void save_tree_section(std::ostream& os, const BSPTree_Ptr& tree);
	static void save_vis_section(std::ostream& os, const LeafVisTable_Ptr& leafVis);

	//#################### LOADING SUPPORT METHODS ####################
public:
	template <typename Poly> static void load_counted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);
	template <typename Vert, typename AuxData> static shared_ptr<Polygon<Vert,AuxData> > load_polygon(const std::string& line, const std::string& n = "");
	template <typename Vert, typename AuxData> static void load_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead = INT_MAX);
private:
	static void read_line(std::istream& is, std::string& line, const std::string& description);
	static void read_checked_line(std::istream& is, std::string& line, const std::string& expected);

	//#################### SAVING SUPPORT METHODS ####################
public:
	template <typename Vert, typename AuxData> static void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount);
};

}

#include "FileSectionUtil_Load.tpp"
#include "FileSectionUtil_Save.tpp"

#endif
