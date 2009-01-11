/***
 * hesperus: OnionTreeFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_ONIONTREEFILEUTIL
#define H_HESP_IO_ONIONTREEFILEUTIL

#include <source/level/onionbsp/OnionTree.h>

namespace hesp {

struct OnionTreeFileUtil
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, OnionTree_Ptr& tree);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons, const OnionTree_Ptr& tree);
};

}

#include "OnionTreeFileUtil.tpp"

#endif
