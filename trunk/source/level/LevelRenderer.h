/***
 * hesperus: LevelRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LEVELRENDERER
#define H_HESP_LEVEL_LEVELRENDERER

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class LevelRenderer
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~LevelRenderer() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render(const std::vector<int>& polyIndices) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<LevelRenderer> LevelRenderer_Ptr;
typedef shared_ptr<const LevelRenderer> LevelRenderer_CPtr;

}

#endif
