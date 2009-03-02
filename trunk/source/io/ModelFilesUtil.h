/***
 * hesperus: ModelFilesUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELFILESUTIL
#define H_HESP_MODELFILESUTIL

#include <source/level/models/Skeleton.h>

namespace hesp {

class ModelFilesUtil
{
	//#################### LOADING METHODS ####################
public:
	// TODO

	//#################### LOADING SUPPORT METHODS ####################
#if 0	// TEMPORARY: To enable easy development testing
private:
#endif
	static Skeleton_Ptr load_skeleton(const std::string& filename);
};

}

#endif
