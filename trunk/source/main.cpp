/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <source/exceptions/Exception.h>
#include <source/game/Game.h>
#if 0
#include <source/level/objects/CmpCollision.h>
#include <source/level/objects/CmpMeshMovement.h>
#include <source/level/objects/CmpOrientation.h>
#include <source/level/objects/CmpPhysics.h>
#include <source/level/objects/CmpPosition.h>
#include <source/level/objects/CmpRender.h>
#include <source/level/objects/CmpUserBipedYoke.h>
#include <source/level/objects/ObjectManager.h>
#endif
using namespace hesp;

int main(int argc, char* argv[])
try
{
#if 0
	std::vector<AABB3d> aabbs;
	ObjectManager om(aabbs);

	std::vector<int> aabbIndices; aabbIndices.push_back(0); aabbIndices.push_back(1);
	Vector3d look(1,0,0);
	std::string modelName = "Test-15";
	int pose = 0;
	Vector3d position(1,2,3);

	std::vector<IComponent_Ptr> components;
	components.push_back(IComponent_Ptr(new CmpCollision(aabbIndices, pose)));
	components.push_back(IComponent_Ptr(new CmpMeshMovement));
	components.push_back(IComponent_Ptr(new CmpOrientation(look)));
	components.push_back(IComponent_Ptr(new CmpPhysics));
	components.push_back(IComponent_Ptr(new CmpPosition(position)));
	components.push_back(IComponent_Ptr(new CmpRender(modelName)));
	components.push_back(IComponent_Ptr(new CmpUserBipedYoke));
	ObjectID id = om.create_object(components);

	ICmpPosition_Ptr p = om.get_component(id, p);
	om.set_component(id, ICmpPosition_Ptr(new CmpPosition(Vector3d(4,5,6))));
	p = om.get_component(id, p);
	ICmpDirectMovement_Ptr d = om.get_component(id, d);
	ICmpMeshMovement_Ptr m = om.get_component(id, m);
	std::cout << m->cur_nav_poly_index() << std::endl;
#endif

	Game game;
	game.run();

	return 0;
}
catch(Exception& e)
{
	std::cerr << e.cause() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << "Error: Something strange and unexpected happened" << std::endl;
	return EXIT_FAILURE;
}
