/***
 * hesperus: FileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_FILEUTIL
#define H_HESP_IO_FILEUTIL

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/bsp/BSPTree.h>
#include <source/level/lighting/Light.h>
#include <source/level/portals/Portal.h>
#include <source/level/vis/VisTable.h>

namespace hesp {

#if 0
std::vector<Light> load_lights_file(const std::string& filename);

template <typename Poly>
void load_lit_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree, std::string& lightmapPrefix);

template <typename Poly>
void load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

template <typename Poly>
void load_tree_file(std::istream& is, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

LeafVisTable_Ptr load_vis_file(const std::string& filename);
#endif

class FileUtil
{
	//#################### PUBLIC METHODS ####################
public:
	static std::vector<Light> load_lights_file(const std::string& filename);

	template <typename Poly>
	static void load_lit_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree, std::string& lightmapPrefix);

	static void load_portals_file(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals);

	template <typename Poly>
	static void load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

	static LeafVisTable_Ptr load_vis_file(const std::string& filename);

	//#################### PRIVATE METHODS ####################
private:
	static std::string load_lightmap_prefix_section(std::istream& is);

	static std::vector<Light> load_lights_section(std::istream& is);

	template <typename Poly>
	static void load_polygons_section(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);

	static void load_separator(std::istream& is);

	static BSPTree_Ptr load_tree_section(std::istream& is);

	static LeafVisTable_Ptr load_vis_section(std::istream& is);
};

}

#include "FileUtil.tpp"

#endif
