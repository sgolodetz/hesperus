/***
 * hesperus: PathTable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_PATHTABLE
#define H_HESP_LEVEL_NAV_PATHTABLE

namespace hesp {

class PathTable
{
	//#################### PUBLIC METHODS ####################
public:
	float cost(int i, int j) const;
	int next_node(int i, int j) const;
	int size() const;
};

}

#endif
