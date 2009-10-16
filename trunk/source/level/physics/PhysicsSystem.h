/***
 * hesperus: PhysicsSystem.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PHYSICSSYSTEM
#define H_HESP_PHYSICSSYSTEM

#include <map>
#include <vector>

#include <boost/weak_ptr.hpp>
using boost::weak_ptr;

#include <source/util/IDAllocator.h>
#include "BroadPhaseCollisionDetector.h"
#include "ContactResolverRegistry.h"
#include "ForceGeneratorRegistry.h"
#include "NarrowPhaseCollisionDetector.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
typedef shared_ptr<const class Contact> Contact_CPtr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;
typedef shared_ptr<class PhysicsObject> PhysicsObject_Ptr;

//#################### TYPEDEFS ####################
typedef shared_ptr<int> PhysicsObjectHandle;

class PhysicsSystem
{
	//#################### NESTED CLASSES ####################
private:
	struct ContactPred
	{
		bool operator()(const Contact_CPtr& lhs, const Contact_CPtr& rhs) const
		{
			return lhs->time() < rhs->time();
		}
	};

	struct ObjectData
	{
		weak_ptr<int> m_wid;
		PhysicsObject_Ptr m_object;

		ObjectData(const weak_ptr<int>& wid, const PhysicsObject_Ptr& object)
		:	m_wid(wid), m_object(object)
		{}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	BroadPhaseCollisionDetector m_broadDetector;
	ContactResolverRegistry m_contactResolverRegistry;
	ForceGeneratorRegistry m_forceGeneratorRegistry;
	IDAllocator m_idAllocator;
	NarrowPhaseCollisionDetector m_narrowDetector;
	std::map<int,ObjectData> m_objects;
	OnionTree_CPtr m_tree;

	//#################### CONSTRUCTORS ####################
public:
	explicit PhysicsSystem(const BoundsManager_CPtr& boundsManager, const OnionTree_CPtr& tree);

	//#################### PUBLIC METHODS ####################
public:
	PhysicsObjectHandle register_object(const PhysicsObject_Ptr& object);
	void remove_contact_resolver(PhysicsMaterial material1, PhysicsMaterial material2);
	void remove_force_generator(const PhysicsObjectHandle& handle, const std::string& forceName);
	void set_contact_resolver(PhysicsMaterial material1, PhysicsMaterial material2, const ContactResolver_CPtr& resolver);
	void set_force_generator(const PhysicsObjectHandle& handle, const std::string& forceName, const ForceGenerator_CPtr& generator);
	void update(int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	void apply_hard_constraints(std::vector<Contact_CPtr>& contacts);
	std::vector<std::vector<Contact_CPtr> > batch_contacts(const std::vector<Contact_CPtr>& contacts);
	void check_objects();
	void detect_contacts(std::vector<Contact_CPtr>& contacts);
	void resolve_contacts(const std::vector<Contact_CPtr>& contacts);
	void simulate_objects(int milliseconds);
};

}

#endif
