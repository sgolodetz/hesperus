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
	std::string m_attachPoint;	// the bone to which the object should be attached for rendering purposes

	//#################### CONSTRUCTORS ####################
public:
	CmpOwnable(const std::string& attachPoint, const ObjectID& owner = ObjectID());

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const std::string& attach_point() const;
	void clear_owner();
	const ObjectID& owner() const;
	std::pair<std::string,Properties> save() const;
	void set_owner(const ObjectID& owner);
};

}

#endif
