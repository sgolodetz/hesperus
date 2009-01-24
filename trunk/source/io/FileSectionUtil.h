/***
 * hesperus: FileSectionUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FILESECTIONUTIL
#define H_HESP_FILESECTIONUTIL

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/images/Image.h>
#include <source/level/brushes/PolyhedralBrush.h>
#include <source/level/bsp/BSPTree.h>
#include <source/level/entities/EntityManager.h>
#include <source/level/lighting/Light.h>
#include <source/level/nav/NavDataset.h>
#include <source/level/onionbsp/OnionTree.h>
#include <source/level/portals/Portal.h>
#include <source/level/vis/VisTable.h>

namespace hesp {

class FileSectionUtil
{
	//#################### FRIENDS ####################
	// We want to only allow access to the specific classes which should be doing file I/O.
	// This helps discourage random file I/O elsewhere in the project.
	friend struct BrushesFileUtil;
	friend struct EntDefFileUtil;
	friend struct EntitiesFileUtil;
	friend class EntityManager;
	friend struct GeometryFileUtil;
	friend class LevelFileUtil;
	friend struct LightsFileUtil;
	friend struct LitTreeFileUtil;
	friend struct NavFileUtil;
	friend struct OnionPortalsFileUtil;
	friend struct OnionTreeFileUtil;
	friend struct PortalsFileUtil;
	friend struct TreeFileUtil;
	friend struct VisFileUtil;

	//#################### LOADING METHODS ####################
private:
	static std::vector<AABB3d> load_aabbs_section(std::istream& is);
	static std::string load_lightmap_prefix_section(std::istream& is);
	static std::vector<Image24_Ptr> load_lightmaps_section(std::istream& is);
	static std::vector<Light> load_lights_section(std::istream& is);
	static std::vector<NavDataset_Ptr> load_nav_section(std::istream& is);
	static OnionTree_Ptr load_onion_tree_section(std::istream& is);
	template <typename Poly> static void load_polygons_section(std::istream& is, const std::string& sectionName, std::vector<shared_ptr<Poly> >& polygons);
	static BSPTree_Ptr load_tree_section(std::istream& is);
	static LeafVisTable_Ptr load_vis_section(std::istream& is);

	//#################### SAVING METHODS ####################
private:
	static void save_lightmap_prefix_section(std::ostream& os, const std::string& lightmapPrefix);
	static void save_lightmaps_section(std::ostream& os, const std::vector<Image24_Ptr>& lightmaps);
	static void save_nav_section(std::ostream& os, const std::vector<NavDataset_Ptr>& datasets);
	static void save_onion_tree_section(std::ostream& os, const OnionTree_Ptr& tree);
	template <typename Poly> static void save_polygons_section(std::ostream& os, const std::string& sectionName, const std::vector<shared_ptr<Poly> >& polygons);
	static void save_tree_section(std::ostream& os, const BSPTree_Ptr& tree);
	static void save_vis_section(std::ostream& os, const LeafVisTable_Ptr& leafVis);

	//#################### LOADING SUPPORT METHODS ####################
private:
	template <typename Poly> static void load_counted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);
public:
	template <typename Vert, typename AuxData> static shared_ptr<Polygon<Vert,AuxData> > load_polygon(const std::string& line, const std::string& n = "");
private:
	template <typename Poly> static shared_ptr<PolyhedralBrush<Poly> > load_polyhedral_brush(std::istream& is);
	template <typename Poly> static void load_uncounted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);
	template <typename Vert, typename AuxData> static void load_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead = INT_MAX);
	static AdjacencyList_Ptr read_adjacency_list(std::istream& is);
	static void read_checked_line(std::istream& is, const std::string& expected);
	static void read_line(std::istream& is, std::string& line, const std::string& description);
	static NavMesh_Ptr read_navmesh(std::istream& is);
	static PathTable_Ptr read_path_table(std::istream& is);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static void write_adjacency_list(std::ostream& os, const AdjacencyList_Ptr& adjList);
	static void write_navmesh(std::ostream& os, const NavMesh_Ptr& mesh);
	static void write_path_table(std::ostream& os, const PathTable_Ptr& pathTable);
	template <typename Vert, typename AuxData> static void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount);
	template <typename Poly> static void write_polyhedral_brush(std::ostream& os, const PolyhedralBrush<Poly>& brush);
};

}

#include "FileSectionUtil_Load.tpp"
#include "FileSectionUtil_Save.tpp"

#endif
