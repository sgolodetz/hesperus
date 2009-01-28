/***
 * hesperus: IHealthComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IHEALTHCOMPONENT
#define H_HESP_IHEALTHCOMPONENT

namespace hesp {

class IHealthComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IHealthComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int health() const = 0;
};

}

#endif
