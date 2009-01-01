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

namespace hesp {

template <typename Poly>
void load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

}

#include "FileUtil.tpp"

#endif
