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
#if 0
#include <source/io/files/DefinitionsFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/level/objects/ObjectsFile.h>
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
#if 0
	boost::filesystem::path baseDir = determine_base_directory_from_game();

	std::string def = (determine_settings_directory(baseDir) / "testex-def.xml").file_string();
	std::vector<AABB3d> aabbs;
	std::map<std::string,std::map<std::string,std::string> > componentPropertyTypes;
	DefinitionsFile::load_ex(def, aabbs, componentPropertyTypes);

	for(std::map<std::string,std::map<std::string,std::string> >::const_iterator it=componentPropertyTypes.begin(), iend=componentPropertyTypes.end(); it!=iend; ++it)
	{
		std::cout << it->first << '\n';
		for(std::map<std::string,std::string>::const_iterator jt=it->second.begin(), jend=it->second.end(); jt!=jend; ++jt)
		{
			std::cout << "    " << jt->first << ':' << jt->second << '\n';
		}
	}

	std::string obs = (baseDir / "resources/levels/tricky/tricky.obs").file_string();
	ObjectManager_Ptr objectManager = ObjectsFile::load(obs, aabbs, componentPropertyTypes, baseDir);
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
