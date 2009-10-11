/***
 * hesperus: ContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONTACTRESOLVER
#define H_HESP_CONTACTRESOLVER

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Contact;

class ContactResolver
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ContactResolver() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void resolve_contact(const Contact& contact, double restitutionCoefficient) const = 0;
};

}

#endif
