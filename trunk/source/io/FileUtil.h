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
#include <source/level/onionbsp/OnionTree.h>
#include <source/level/portals/Portal.h>
#include <source/util/PolygonTypes.h>

namespace hesp {

class FileUtil
{
	//#################### LOADING METHODS ####################
public:
	template <typename Poly> static std::vector<shared_ptr<PolyhedralBrush<Poly> > > load_brushes_file(const std::string& filename);
#if 0
	static Level_Ptr load_level_file(const std::string& filename);
#endif
	template <typename Poly> static void load_onion_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, OnionTree_Ptr& tree);
	template <typename Poly> static void load_polygons_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons);
	static void load_portals_file(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals);
	template <typename Poly> static void load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

	//#################### SAVING METHODS ####################
public:
	template <typename Poly> static void save_brushes_file(const std::string& filename, const std::vector<shared_ptr<PolyhedralBrush<Poly> > >& brushes);
#if 0
	static void save_level_file(const std::string& filename, const std::vector<TexturedPolygon_Ptr>& polygons,
								const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
								const LeafVisTable_Ptr& leafVis);
	static void save_level_file(const std::string& filename, const std::vector<TexturedLitPolygon_Ptr>& polygons,
								const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
								const LeafVisTable_Ptr& leafVis, const std::vector<Image24_Ptr>& lightmaps);
#endif

	//#################### LOADING SUPPORT METHODS ####################
private:
#if 0
	static Level_Ptr load_lit_level_file(std::istream& is);
#endif
	static void load_separator(std::istream& is);
#if 0
	static Level_Ptr load_unlit_level_file(std::istream& is);
#endif
};

}

#include "FileUtil.tpp"

#endif
