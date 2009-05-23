/***
 * hesperus: ICmpCollectible.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPCOLLECTIBLE
#define H_HESP_ICMPCOLLECTIBLE

#include <source/level/objects/base/IComponent.h>

namespace hesp {

class ICmpCollectible : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	// TODO

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Collectible"; }
	static std::string static_group_type()	{ return "Collectible"; }

	std::string own_type() const			{ return "Collectible"; }
	static std::string static_own_type()	{ return "Collectible"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpCollectible> ICmpCollectible_Ptr;
typedef shared_ptr<const ICmpCollectible> ICmpCollectible_CPtr;

}

#endif
