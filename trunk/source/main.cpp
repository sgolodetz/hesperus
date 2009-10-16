/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <source/exceptions/Exception.h>
#include <source/game/Game.h>
#include <source/io/util/DirectoryFinder.h>
using namespace hesp;

//#define CD_TEST
#ifdef CD_TEST
#include <source/level/collisions/AABBBounds.h>
#include <source/level/collisions/BoundsManager.h>
#include <source/level/physics/NarrowPhaseCollisionDetector.h>
#include <source/level/physics/NormalPhysicsObject.h>
#endif

//#define PS_TEST
#ifdef PS_TEST
#include <source/level/collisions/AABBBounds.h>
#include <source/level/collisions/BoundsManager.h>
#include <source/level/physics/BasicContactResolver.h>
#include <source/level/physics/ForceGenerator.h>
#include <source/level/physics/NormalPhysicsObject.h>
#include <source/level/physics/PhysicsSystem.h>
#endif

namespace hesp {

boost::filesystem::path determine_base_directory()
{
	return determine_base_directory_from_game();
}

}

int main(int argc, char* argv[])
try
{
#ifdef CD_TEST
	// Set up the bounds.
	std::vector<Bounds_CPtr> bounds;
	std::map<std::string,int> boundsLookup;
	std::map<std::string,BoundsManager::BoundsGroup> boundsGroups;
	bounds.push_back(Bounds_CPtr(new AABBBounds(Vector3d(1,1,1))));
	boundsLookup["A"] = 0;
	boundsGroups["G"]["A"] = "A";
	bounds.push_back(Bounds_CPtr(new AABBBounds(Vector3d(1,1,1))));
	boundsLookup["B"] = 1;
	boundsGroups["G"]["B"] = "B";
	BoundsManager_Ptr boundsManager(new BoundsManager(bounds, boundsLookup, boundsGroups));

	// Set up the physics objects.
	/*NormalPhysicsObject objectA("G", "A", 1.0, PM_CHARACTER, Vector3d(1,1,1));	// (0,0,0) - (2,2,2)
	NormalPhysicsObject objectB("G", "B", 1.0, PM_CHARACTER, Vector3d(4,1,1));	// (3,0,0) - (5,2,2)
	objectA.cache_previous_position();
	objectA.set_position(Vector3d(2,1,1));	// (1,0,0) - (3,2,2)
	objectB.cache_previous_position();
	objectB.set_position(Vector3d(3,1,1));	// (2,0,0) - (4,2,2)*/

	/*NormalPhysicsObject objectA("G", "A", 1.0, PM_CHARACTER, Vector3d(1,1,1));	// (0,0,0) - (2,2,2)
	NormalPhysicsObject objectB("G", "B", 1.0, PM_CHARACTER, Vector3d(4,2,1));	// (3,1,0) - (5,3,2)
	objectA.cache_previous_position();
	objectA.set_position(Vector3d(2,1,1));	// (1,0,0) - (3,2,2)
	objectB.cache_previous_position();
	objectB.set_position(Vector3d(3,2,1));	// (2,1,0) - (4,3,2)*/

	/*NormalPhysicsObject objectA("G", "A", 1.0, PM_CHARACTER, Vector3d(0,0,0));	// (-1,-1,-1) - (1,1,1)
	NormalPhysicsObject objectB("G", "B", 1.0, PM_CHARACTER, Vector3d(2,2,2));	// (1,1,1) - (3,3,3)
	objectB.cache_previous_position();
	objectB.set_position(Vector3d(1,1,1));	// (0,0,0) - (2,2,2)*/

	NormalPhysicsObject objectA("G", "A", 1.0, PM_CHARACTER, Vector3d(0,0,0));
	NormalPhysicsObject objectB("G", "B", 1.0, PM_CHARACTER, Vector3d(0.6,0.8,0));

	/*NormalPhysicsObject objectA("G", "A", 1.0, PM_CHARACTER, Vector3d(0,0,0));
	NormalPhysicsObject objectB("G", "B", 1.0, PM_CHARACTER, Vector3d(0,0,0));*/

	// Try the collision detector.
	NarrowPhaseCollisionDetector cd(boundsManager);
	boost::optional<Contact> contact = cd.object_vs_object(objectA, objectB);
	if(contact)
	{
		const Contact& c = *contact;
		int x;
		x = 23;
	}
#endif

#ifdef PS_TEST
	struct FixedForceGenerator : public ForceGenerator
	{
		Vector3d m_force;
		explicit FixedForceGenerator(const Vector3d& force) : m_force(force) {}
		void update_force(PhysicsObject& object, int milliseconds) const { object.apply_force(m_force); }
	};

	// Set up the bounds.
	std::vector<Bounds_CPtr> bounds;
	std::map<std::string,int> boundsLookup;
	std::map<std::string,BoundsManager::BoundsGroup> boundsGroups;
	bounds.push_back(Bounds_CPtr(new AABBBounds(Vector3d(1,1,1))));
	boundsLookup["A"] = 0;
	boundsGroups["G"]["A"] = "A";
	bounds.push_back(Bounds_CPtr(new AABBBounds(Vector3d(1,1,1))));
	boundsLookup["B"] = 1;
	boundsGroups["G"]["B"] = "B";
	BoundsManager_Ptr boundsManager(new BoundsManager(bounds, boundsLookup, boundsGroups));

	OnionTree_CPtr tree;
	PhysicsSystem ps(boundsManager, tree);
	ps.set_contact_resolver(PM_CHARACTER, PM_CHARACTER, ContactResolver_CPtr(new BasicContactResolver(0.1)));

	PhysicsObject_Ptr objectA(new NormalPhysicsObject("G", "A", 1.0, PM_CHARACTER, Vector3d(1,1,1)));
	PhysicsObject_Ptr objectB(new NormalPhysicsObject("G", "B", 1.0, PM_CHARACTER, Vector3d(4,1,1)));
	PhysicsObjectHandle handleA = ps.register_object(objectA);
	PhysicsObjectHandle handleB = ps.register_object(objectB);

	ps.set_force_generator(handleA, "fA", ForceGenerator_CPtr(new FixedForceGenerator(Vector3d(2,0,0))));
	ps.set_force_generator(handleB, "fB", ForceGenerator_CPtr(new FixedForceGenerator(Vector3d(-2,0,0))));

	ps.update(1000);
#endif

	Game game;
	game.run();

	return 0;
}
catch(Exception& e)
{
	std::cerr << "Error: " << e.cause() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << "Error: Something strange and unexpected happened" << std::endl;
	return EXIT_FAILURE;
}
