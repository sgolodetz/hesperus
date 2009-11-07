/***
 * hesperus: AbsorbProjectileContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ABSORBPROJECTILECONTACTRESOLVER
#define H_HESP_ABSORBPROJECTILECONTACTRESOLVER

#include <source/level/physics/ContactResolver.h>
#include <source/level/physics/PhysicsMaterial.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectManager;

class AbsorbProjectileContactResolver : public ContactResolver
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectManager *m_objectManager;
	PhysicsMaterial m_projectileMaterial;

	//#################### CONSTRUCTORS ####################
public:
	AbsorbProjectileContactResolver(ObjectManager *objectManager, PhysicsMaterial projectileMaterial);

	//#################### PRIVATE METHODS ####################
private:
	void resolve_object_object(const Contact& contact, const OnionTree_CPtr& tree) const;
	void resolve_object_world(const Contact& contact) const;
};

}

#endif
