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
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/datastructures/PriorityQueue.h>
#include <source/util/IDAllocator.h>
#include "ListenerTable.h"
#include "ObjectID.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
typedef shared_ptr<class IObjectComponent> IObjectComponent_Ptr;
typedef shared_ptr<const class Message> Message_CPtr;

class ObjectManager
{
	//#################### TYPEDEFS ####################
public:
	typedef boost::function<bool (const ObjectID&,const ObjectManager*)> GroupPredicate;
private:
	typedef PriorityQueue<ObjectID,int,bool,std::greater<int> > DestructionQueue;
	typedef std::map<std::string,IObjectComponent_Ptr> Object;

	//#################### PRIVATE VARIABLES ####################
private:
	BoundsManager_CPtr m_boundsManager;
	std::map<std::string,std::map<std::string,std::string> > m_componentPropertyTypes;
	std::map<std::string,GroupPredicate> m_groupPredicates;
	IDAllocator m_idAllocator;
	std::map<ObjectID,Object> m_objects;

	DestructionQueue m_destructionQueue;
	ListenerTable m_listenerTable;

	//#################### CONSTRUCTORS ####################
public:
	ObjectManager(const BoundsManager_CPtr& boundsManager, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);

	//#################### PUBLIC METHODS ####################
public:
	void add_listener(IObjectComponent *listener, const ObjectID& id);
	const BoundsManager_CPtr& bounds_manager() const;
	void broadcast_message(const Message_CPtr& msg);
	const std::map<std::string,std::map<std::string,std::string> >& component_property_types() const;
	void consolidate_object_ids();
	ObjectID create_object(const std::vector<IObjectComponent_Ptr>& components);
	void flush_destruction_queue();
	template <typename T> shared_ptr<T> get_component(const ObjectID& id, const shared_ptr<T>& = shared_ptr<T>());
	template <typename T> shared_ptr<const T> get_component(const ObjectID& id, const shared_ptr<const T>& = shared_ptr<const T>()) const;
	std::vector<IObjectComponent_Ptr> get_components(const ObjectID& id);
	std::vector<ObjectID> group(const std::string& name) const;
	int object_count() const;
	ObjectID player() const;
	void post_message(const ObjectID& target, const Message_CPtr& msg);
	void queue_child_for_destruction(const ObjectID& child, const ObjectID& parent);
	void queue_for_destruction(const ObjectID& id);
	void register_group(const std::string& name, const GroupPredicate& pred);
	void remove_listener(IObjectComponent *listener, const ObjectID& id);
#if 0
	template <typename T> void set_component(const ObjectID& id, const shared_ptr<T>& component);
#endif

	//#################### PRIVATE METHODS ####################
private:
	void destroy_object(const ObjectID& id);
	template <typename T> shared_ptr<T> get_component(const ObjectID& id, const std::string& group);
	template <typename T> shared_ptr<const T> get_component(const ObjectID& id, const std::string& group) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ObjectManager> ObjectManager_Ptr;
typedef shared_ptr<const ObjectManager> ObjectManager_CPtr;

}

#include "ObjectManager.tpp"

#endif
