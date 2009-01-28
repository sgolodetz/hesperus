/***
 * hesperus: IYokeComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IYOKECOMPONENT
#define H_HESP_IYOKECOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class IYokeComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IYokeComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void set_yoke(const Yoke_Ptr& yoke) = 0;
	virtual const Yoke_Ptr& yoke() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IYokeComponent> IYokeComponent_Ptr;

}

#endif
