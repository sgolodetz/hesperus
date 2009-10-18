/***
 * hesperus: BasicContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BASICCONTACTRESOLVER
#define H_HESP_BASICCONTACTRESOLVER

#include "ContactResolver.h"

namespace hesp {

class BasicContactResolver : public ContactResolver
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_restitutionCoefficient;

	//#################### CONSTRUCTORS ####################
public:
	explicit BasicContactResolver(double restitutionCoefficient);

	//#################### PUBLIC METHODS ####################
public:
	void resolve_contact(const Contact& contact, const OnionTree_CPtr& tree) const;

	//#################### PRIVATE METHODS ####################
private:
	void resolve_object_object(const Contact& contact, const OnionTree_CPtr& tree) const;
	void resolve_object_world(const Contact& contact) const;
};

}

#endif
