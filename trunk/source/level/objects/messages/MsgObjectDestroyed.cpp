/***
 * hesperus: MsgObjectDestroyed.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MsgObjectDestroyed.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
MsgObjectDestroyed::MsgObjectDestroyed(const ObjectID& id)
:	m_id(id)
{}

//#################### PUBLIC METHODS ####################
void MsgObjectDestroyed::dispatch(MessageHandlerBase *handler) const
{
	dynamic_dispatch(handler, this);
}

const ObjectID& MsgObjectDestroyed::object_id() const
{
	return m_id;
}

}
