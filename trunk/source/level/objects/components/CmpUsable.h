/***
 * hesperus: CmpUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPUSABLE
#define H_HESP_CMPUSABLE

#include "ICmpUsable.h"

namespace hesp {

class CmpUsable : public ICmpUsable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_scriptName;
	std::string m_usableGroup;

	//#################### CONSTRUCTORS ####################
public:
	CmpUsable(const std::string& usableGroup, const std::string& scriptName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	std::pair<std::string,Properties> save() const;
	std::string usable_group() const;
	void use();
};

}

#endif
