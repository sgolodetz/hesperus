/***
 * hesperus: CmpCollectible.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPCOLLECTIBLE
#define H_HESP_CMPCOLLECTIBLE

#include "ICmpCollectible.h"

namespace hesp {

class CmpCollectible : public ICmpCollectible
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_scriptName;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpCollectible(const std::string& scriptName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	std::pair<std::string,Properties> save() const;
};

}

#endif
