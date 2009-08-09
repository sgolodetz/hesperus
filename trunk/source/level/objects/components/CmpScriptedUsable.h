/***
 * hesperus: CmpScriptedUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPSCRIPTEDUSABLE
#define H_HESP_CMPSCRIPTEDUSABLE

#include "CmpUsable.h"

namespace hesp {

class CmpScriptedUsable : public CmpUsable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_scriptName;

	//#################### CONSTRUCTORS ####################
public:
	CmpScriptedUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& scriptName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	std::pair<std::string,Properties> save() const;
	void use();
};

}

#endif