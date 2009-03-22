/***
 * hesperus: LitTreeFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LITTREEFILEUTIL
#define H_HESP_LITTREEFILEUTIL

#include <source/level/bsp/BSPTree.h>

namespace hesp {

struct LitTreeFileUtil
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons,
											  BSPTree_Ptr& tree, std::string& lightmapPrefix);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons,
											  const BSPTree_Ptr& tree, const std::string& lightmapPrefix);
};

}

#include "LitTreeFileUtil.tpp"

#endif
