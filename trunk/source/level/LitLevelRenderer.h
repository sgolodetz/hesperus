/***
 * hesperus: LitLevelRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LITLEVELRENDERER
#define H_HESP_LEVEL_LITLEVELRENDERER

#include "LevelRenderer.h"

namespace hesp {

class LitLevelRenderer : public LevelRenderer
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
