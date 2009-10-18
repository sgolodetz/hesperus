/***
 * hesperus: ContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONTACTRESOLVER
#define H_HESP_CONTACTRESOLVER

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Contact;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class ContactResolver
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ContactResolver() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void resolve_contact(const Contact& contact, const OnionTree_CPtr& tree) const = 0;
};

}

#endif
