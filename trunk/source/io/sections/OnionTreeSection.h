/***
 * hesperus: OnionTreeSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONTREESECTION
#define H_HESP_ONIONTREESECTION

#include <source/level/trees/OnionTree.h>

namespace hesp {

struct OnionTreeSection
{
	//#################### LOADING METHODS ####################
	static OnionTree_Ptr load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const OnionTree_Ptr& tree);
};

}

#endif
