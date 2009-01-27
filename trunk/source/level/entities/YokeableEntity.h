/***
 * hesperus: YokeableEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_YOKEABLEENTITY
#define H_HESP_YOKEABLEENTITY

#include "Yoke.h"

namespace hesp {

/**
This class represents a yokeable entity (i.e. one which can be controlled by either player input or an AI).
*/
class YokeableEntity
{
	//#################### PRIVATE VARIABLES ####################
private:
	Yoke_Ptr m_yoke;

	//#################### DESTRUCTOR ####################
public:
	virtual ~YokeableEntity() {}

	//#################### PUBLIC METHODS ####################
public:
	void set_yoke(const Yoke_Ptr& yoke)
	{
		m_yoke = yoke;
	}

	const Yoke_Ptr& yoke() const
	{
		return m_yoke;
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<YokeableEntity> YokeableEntity_Ptr;

}

#endif
