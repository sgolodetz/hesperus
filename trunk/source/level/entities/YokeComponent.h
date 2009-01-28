/***
 * hesperus: YokeComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_YOKECOMPONENT
#define H_HESP_YOKECOMPONENT

#include "IYokeComponent.h"
#include "Yoke.h"

namespace hesp {

class YokeComponent : public IYokeComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Yoke_Ptr m_yoke;

	//#################### PUBLIC METHODS ####################
public:
	void set_yoke(const Yoke_Ptr& yoke)
	{
		m_yoke = yoke;
	}

	const Yoke_Ptr& yoke() const
	{
		return yoke;
	}
};

}

#endif
