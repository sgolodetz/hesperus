/***
 * hesperus: ObjectID.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectID.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
ObjectID::ObjectID()
:	m_id(-1)
{}

ObjectID::ObjectID(int id)
:	m_id(id)
{}

//#################### PUBLIC METHODS ####################
int ObjectID::value() const
{
	return m_id;
}

//#################### GLOBAL OPERATORS ####################
bool operator<(const ObjectID& lhs, const ObjectID& rhs)
{
	return lhs.value() < rhs.value();
}

}
