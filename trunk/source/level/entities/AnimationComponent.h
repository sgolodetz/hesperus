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
	mutable Model_Ptr m_model;
	std::string m_modelName;

	//#################### CONSTRUCTORS ####################
public:
	AnimationComponent(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	const Model_Ptr& model() const;
	void save(std::ostream& os) const;
};

}

#endif
