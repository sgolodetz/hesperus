/***
 * hesperus: CmpItemActivatable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPITEMACTIVATABLE
#define H_HESP_CMPITEMACTIVATABLE

#include "ICmpActivatable.h"

namespace hesp {

class CmpItemActivatable : public ICmpActivatable
{
	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr load(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	void activated_by(const ObjectID& activator);
	std::pair<std::string,Properties> save() const;
};

}

#endif
