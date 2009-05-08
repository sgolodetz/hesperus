/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <source/exceptions/Exception.h>
#include <source/game/Game.h>
#if 0
#include <source/level/objects/CmpMeshMovement.h>
#include <source/level/objects/CmpPosition.h>
#include <source/level/objects/MsgObjectDestroyed.h>
#include <source/level/objects/ObjectManager.h>
#endif
using namespace hesp;

int main(int argc, char* argv[])
try
{
#if 0
	ObjectManager om;
	std::vector<IComponent_Ptr> components;
	components.push_back(IComponent_Ptr(new CmpPosition));
	components.push_back(IComponent_Ptr(new CmpMeshMovement));
	ObjectID id = om.create_object(components);
	ICmpPosition_Ptr p = om.get_component(id, p);
	om.set_component(id, ICmpPosition_Ptr(new CmpPosition));
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
