/***
 * hesperus: UnlitLevelRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_UNLITLEVELRENDERER
#define H_HESP_LEVEL_UNLITLEVELRENDERER

#include "LevelRenderer.h"

namespace hesp {

class UnlitLevelRenderer : public LevelRenderer
{
	//#################### PRIVATE VARIABLES ####################
private:
	// TODO

	//#################### CONSTRUCTORS ####################
public:
	// TODO

	//#################### PUBLIC METHODS ####################
public:
	virtual void render() const = 0;
};

}

#endif
