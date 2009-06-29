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
	std::string m_usableGroup;
	std::vector<std::string> m_hotspots;
	std::string m_scriptName;

	//#################### CONSTRUCTORS ####################
public:
	CmpUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& scriptName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<std::string>& hotspots() const;
	std::pair<std::string,Properties> save() const;
	std::string usable_group() const;
	void use();
};

}

#endif
