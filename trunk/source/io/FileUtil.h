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
#include <source/level/vis/VisTable.h>

namespace hesp {

std::vector<Light> load_lights_file(const std::string& filename);

template <typename Poly>
void load_lit_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree, std::string& lightmapPrefix);

template <typename Poly>
void load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

template <typename Poly>
void load_tree_file(std::istream& is, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

LeafVisTable_Ptr load_vis_file(const std::string& filename);

}

#include "FileUtil.tpp"

#endif
