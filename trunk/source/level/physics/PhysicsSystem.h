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
#include "NarrowPhaseCollisionDetector.h"
#include "ForceGeneratorRegistry.h"

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
