/***
 * hesperus: AnimationComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ANIMATIONCOMPONENT
#define H_HESP_ANIMATIONCOMPONENT

#include <string>

#include "IAnimationComponent.h"

namespace hesp {

class AnimationComponent : public IAnimationComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_modelFilename;

	//#################### CONSTRUCTORS ####################
public:
	AnimationComponent(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	void save(std::ostream& os) const;
};

}

#endif
