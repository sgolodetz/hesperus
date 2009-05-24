/***
 * hesperus: CmpScriptedActivatable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPSCRIPTEDACTIVATABLE
#define H_HESP_CMPSCRIPTEDACTIVATABLE

#include "ICmpActivatable.h"

namespace hesp {

class CmpScriptedActivatable : public ICmpActivatable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_scriptName;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpScriptedActivatable(const std::string& scriptName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void activated_by(const ObjectID& activator);
	void check_dependencies() const;
	std::pair<std::string,Properties> save() const;
};

}

#endif
