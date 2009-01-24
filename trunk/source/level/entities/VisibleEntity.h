/***
 * hesperus: VisibleEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VISIBLEENTITY
#define H_HESP_VISIBLEENTITY

#include <string>

namespace hesp {

/**
This class represents a potentially visible entity (i.e. this entity needs an animated model).
*/
class VisibleEntity
{
	//#################### PROTECTED VARIABLES ####################
protected:
	std::string m_modelFilename;

	//#################### CONSTRUCTORS ####################
public:
	VisibleEntity(const std::string& modelFilename)
	:	m_modelFilename(modelFilename)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~VisibleEntity() {}
};

}

#endif
