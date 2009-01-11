/***
 * hesperus: TreeFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_TREEFILEUTIL
#define H_HESP_IO_TREEFILEUTIL

namespace hesp {

struct TreeFileUtil
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons, const BSPTree_Ptr& tree);
};

}

#include "TreeFileUtil.tpp"

#endif
