/***
 * hesperus: ObjectManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTMANAGER
#define H_HESP_OBJECTMANAGER

#include <map>
#include <string>
#include <vector>

#include <source/math/geom/AABB.h>
#include "IComponent.h"
#include "IDAllocator.h"
#include "Message.h"
#include "ObjectID.h"

namespace hesp {

class ObjectManager
{
	//#################### TYPEDEFS ####################
private:
	typedef std::map<std::string,IComponent_Ptr> Object;

	//#################### PRIVATE VARIABLES ####################
private:
	IDAllocator m_idAllocator;
	std::map<ObjectID,Object> m_objects;

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<AABB3d>& aabbs() const;
#if 0
	void broadcast_delayed_message(message)
#endif
	void broadcast_immediate_message(const Message_CPtr& msg);
	ObjectID create_object(const std::vector<IComponent_Ptr>& components);
	void destroy_object(const ObjectID& id);
	template <typename T> shared_ptr<T> get_component(const ObjectID& id, const shared_ptr<T>& = shared_ptr<T>());
#if 0
	void post_delayed_message(const ObjectID& target, message)
#endif
	void post_immediate_message(const ObjectID& target, const Message_CPtr& msg);
	template <typename T> void set_component(const ObjectID& id, const shared_ptr<T>& component);

	//#################### PRIVATE METHODS ####################
private:
	void post_message_to_object(Object& target, const Message_CPtr& msg);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ObjectManager> ObjectManager_Ptr;
typedef shared_ptr<const ObjectManager> ObjectManager_CPtr;

}

#include "ObjectManager.tpp"

#endif
