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
	NormalPhysicsObject objectA("G", "A", 1.0, PM_CHARACTER, Vector3d(1,1,1));	// (0,0,0) - (2,2,2)
	NormalPhysicsObject objectB("G", "B", 1.0, PM_CHARACTER, Vector3d(4,1,1));	// (3,0,0) - (5,2,2)
	objectA.set_position(Vector3d(2,1,1));	// (1,0,0) - (3,2,2)
	objectB.set_position(Vector3d(3,1,1));	// (2,0,0) - (4,2,2)

	// Try the collision detector.
	NarrowPhaseCollisionDetector cd(boundsManager);
	boost::optional<Contact> contact = cd.object_vs_object(objectA, objectB);
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
