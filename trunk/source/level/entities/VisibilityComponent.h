/***
 * hesperus: VisibilityComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VISIBILITYCOMPONENT
#define H_HESP_VISIBILITYCOMPONENT

#include <string>

#include <source/io/FieldIO.h>
#include "IVisibilityComponent.h"

namespace hesp {

class VisibilityComponent : public IVisibilityComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_modelFilename;

	//#################### CONSTRUCTORS ####################
public:
	VisibilityComponent(std::istream& is)
	{
		m_modelFilename = FieldIO::read_field(is, "GameModel");
	}

	//#################### PUBLIC METHODS ####################
public:
	void save(std::ostream& os) const
	{
		FieldIO::write_typed_field(os, "GameModel", m_modelFilename);
	}
};

}

#endif
