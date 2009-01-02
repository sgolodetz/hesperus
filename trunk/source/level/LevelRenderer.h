/***
 * hesperus: LevelRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LEVELRENDERER
#define H_HESP_LEVEL_LEVELRENDERER

namespace hesp {

class LevelRenderer
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~LevelRenderer() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render() const = 0;
};

}

#endif
