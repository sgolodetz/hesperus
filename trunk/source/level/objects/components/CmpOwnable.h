/***
 * hesperus: CmpOwnable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPOWNABLE
#define H_HESP_CMPOWNABLE

#include "ICmpOwnable.h"

namespace hesp {

class CmpOwnable : public ICmpOwnable
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_owner;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpOwnable(const ObjectID& owner = ObjectID());

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void clear_owner();
	const ObjectID& owner() const;
	std::pair<std::string,Properties> save() const;
	void set_owner(const ObjectID& owner);
};

}

#endif
