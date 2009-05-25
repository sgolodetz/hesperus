/***
 * hesperus: ObjectManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTMANAGER
#define H_HESP_OBJECTMANAGER

#include <map>
#include <string>
#include <vector>

#include <boost/function.hpp>

#include <source/math/geom/AABB.h>
#include "IComponent.h"
#include "IDAllocator.h"
#include "Message.h"
#include "ObjectID.h"

namespace hesp {

class ObjectManager
{
	//#################### TYPEDEFS ####################
public:
	typedef boost::function<bool (const ObjectID&,const ObjectManager*)> GroupPredicate;
private:
	typedef std::map<std::string,IComponent_Ptr> Object;

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<AABB3d> m_aabbs;
	std::map<std::string,std::map<std::string,std::string> > m_componentPropertyTypes;
	std::map<std::string,GroupPredicate> m_groupPredicates;
	IDAllocator m_idAllocator;
	std::map<ObjectID,Object> m_objects;

	//#################### CONSTRUCTORS ####################
public:
	ObjectManager(const std::vector<AABB3d>& aabbs, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<AABB3d>& aabbs() const;
#if 0
	void broadcast_delayed_message(message)
#endif
	void broadcast_immediate_message(const Message_CPtr& msg);
	const std::map<std::string,std::map<std::string,std::string> >& component_property_types() const;
	void consolidate_object_ids();
	ObjectID create_object(const std::vector<IComponent_Ptr>& components);
	void destroy_object(const ObjectID& id);
	template <typename T> shared_ptr<T> get_component(const ObjectID& id, const shared_ptr<T>& = shared_ptr<T>());
	template <typename T> shared_ptr<const T> get_component(const ObjectID& id, const shared_ptr<const T>& = shared_ptr<const T>()) const;
	std::vector<IComponent_Ptr> get_components(const ObjectID& id);
	std::vector<ObjectID> group(const std::string& name) const;
	int object_count() const;
#if 0
	void post_delayed_message(const ObjectID& target, message)
#endif
	void post_immediate_message(const ObjectID& target, const Message_CPtr& msg);
	void register_group(const std::string& name, const GroupPredicate& pred);
	template <typename T> void set_component(const ObjectID& id, const shared_ptr<T>& component);
	ObjectID viewer() const;

	//#################### PRIVATE METHODS ####################
private:
	static bool is_activatable(const ObjectID& objectID, const ObjectManager *objectManager);
	static bool is_animatable(const ObjectID& objectID, const ObjectManager *objectManager);
	static bool is_renderable(const ObjectID& objectID, const ObjectManager *objectManager);
	static bool is_simulable(const ObjectID& objectID, const ObjectManager *objectManager);
	static bool is_yokeable(const ObjectID& objectID, const ObjectManager *objectManager);
	void post_message_to_object(Object& target, const Message_CPtr& msg);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ObjectManager> ObjectManager_Ptr;
typedef shared_ptr<const ObjectManager> ObjectManager_CPtr;

}

#include "ObjectManager.tpp"

#endif
