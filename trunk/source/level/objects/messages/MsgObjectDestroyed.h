/***
 * hesperus: MsgObjectDestroyed.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MSGOBJECTDESTROYED
#define H_HESP_MSGOBJECTDESTROYED

#include <source/level/objects/base/Message.h>
#include <source/level/objects/base/ObjectID.h>

namespace hesp {

class MsgObjectDestroyed : public Message
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_id;

	//#################### CONSTRUCTORS ####################
public:
	explicit MsgObjectDestroyed(const ObjectID& id);

	//#################### PUBLIC METHODS ####################
public:
	void dispatch(MessageHandlerBase *handler) const;
	const ObjectID& object_id() const;
	std::set<ObjectID> referenced_objects() const;
};

}

#endif
