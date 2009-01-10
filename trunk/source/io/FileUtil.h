/***
 * hesperus: FileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_FILEUTIL
#define H_HESP_IO_FILEUTIL

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/images/Image.h>
#include <source/level/Level.h>
#include <source/level/brushes/PolyhedralBrush.h>
#include <source/level/bsp/BSPTree.h>
#include <source/level/lighting/Light.h>
#include <source/level/onionbsp/OnionTree.h>
#include <source/level/portals/Portal.h>
#include <source/level/vis/VisTable.h>
#include <source/util/PolygonTypes.h>

namespace hesp {

class FileUtil
{
	//#################### PUBLIC METHODS ####################
public:
	template <typename Poly>
	static std::vector<shared_ptr<PolyhedralBrush<Poly> > > load_brushes_file(const std::string& filename);

	static Level_Ptr load_level_file(const std::string& filename);

	static std::vector<Light> load_lights_file(const std::string& filename);

	template <typename Poly>
	static void load_lit_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree, std::string& lightmapPrefix);

	template <typename Poly>
	static void load_onion_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, OnionTree_Ptr& tree);

	template <typename Poly>
	static void load_polygons_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons);

	static void load_portals_file(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals);

	template <typename Poly>
	static void load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

	static LeafVisTable_Ptr load_vis_file(const std::string& filename);

	template <typename Poly>
	static void save_brushes_file(const std::string& filename, const std::vector<shared_ptr<PolyhedralBrush<Poly> > >& brushes);

	static void save_level_file(const std::string& filename, const std::vector<TexturedPolygon_Ptr>& polygons,
								const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
								const LeafVisTable_Ptr& leafVis);

	static void save_level_file(const std::string& filename, const std::vector<TexturedLitPolygon_Ptr>& polygons,
								const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
								const LeafVisTable_Ptr& leafVis, const std::vector<Image24_Ptr>& lightmaps);

	static void save_vis_section(std::ostream& os, const LeafVisTable_Ptr& leafVis);

	//#################### PRIVATE METHODS ####################
private:
	static std::vector<Light> load_lights_section(std::istream& is);

	static Level_Ptr load_lit_level_file(std::istream& is);

	static OnionTree_Ptr load_onion_tree_section(std::istream& is);

	template <typename Poly>
	static void load_polygons_section(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);

	template <typename Poly>
	static shared_ptr<PolyhedralBrush<Poly> > load_polyhedral_brush(std::istream& is);

	static void load_separator(std::istream& is);

	static BSPTree_Ptr load_tree_section(std::istream& is);

	static Level_Ptr load_unlit_level_file(std::istream& is);

	static LeafVisTable_Ptr load_vis_section(std::istream& is);
};

}

#include "FileUtil.tpp"

#endif
