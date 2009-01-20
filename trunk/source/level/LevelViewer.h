/***
 * hesperus: LevelViewer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVELVIEWER
#define H_HESP_LEVELVIEWER

#include <source/gui/Component.h>
#include "Level.h"

namespace hesp {

class LevelViewer : public Component
{
	//#################### PRIVATE VARIABLES ####################
private:
	Level_Ptr m_level;

	//#################### CONSTRUCTORS ####################
public:
	LevelViewer(const Level_Ptr& level);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
};

}

#endif
