/***
 * hesperus: TreeSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TREESECTION
#define H_HESP_TREESECTION

#include <source/level/trees/BSPTree.h>

namespace hesp {

struct TreeSection
{
	//#################### LOADING METHODS ####################
	static BSPTree_Ptr load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const BSPTree_CPtr& tree);
};

}

#endif
