/***
 * hesperus: VisibilityComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VISIBILITYCOMPONENT
#define H_HESP_VISIBILITYCOMPONENT

#include <string>

#include "IVisibilityComponent.h"

namespace hesp {

class VisibilityComponent : public IVisibilityComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_modelFilename;

	//#################### CONSTRUCTORS ####################
public:
	VisibilityComponent(const std::string& modelFilename)
	:	m_modelFilename(modelFilename)
	{}
};

}

#endif
