/***
 * hesperus: YokeMinimusGotoPosition.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "YokeMinimusGotoPosition.h"

#include <source/level/nav/GlobalPathfinder.h>
#include "CmdBipedMove.h"
#include "ICmpCollision.h"
#include "ICmpMeshMovement.h"
#include "ICmpPosition.h"
#include "MoveFunctions.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
YokeMinimusGotoPosition::YokeMinimusGotoPosition(const ObjectID& objectID, ObjectManager *objectManager, const Vector3d& dest)
:	m_objectID(objectID), m_objectManager(objectManager), m_dest(dest)
{}

	//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> YokeMinimusGotoPosition::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																		  const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	// Check to make sure the yoke's still active.
	if(m_state != YOKE_ACTIVE)
	{
		return std::vector<ObjectCommand_Ptr>();
	}

	ICmpCollision_Ptr cmpCollision = m_objectManager->get_component(m_objectID, cmpCollision);		assert(cmpCollision != NULL);
	ICmpMeshMovement_Ptr cmpMovement = m_objectManager->get_component(m_objectID, cmpMovement);		assert(cmpMovement != NULL);
	ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);			assert(cmpPosition != NULL);

	const Vector3d& source = cmpPosition->position();

	if(!m_path)
	{
		int mapIndex = cmpCollision->aabb_indices()[cmpCollision->pose()];
		NavMesh_Ptr navMesh = navDatasets[mapIndex]->nav_mesh();
		GlobalPathfinder pathfinder(navMesh, navDatasets[mapIndex]->adjacency_list(), navDatasets[mapIndex]->path_table());

		int suggestedSourcePoly = cmpMovement->cur_nav_poly_index();
		int sourcePoly = MoveFunctions::find_nav_polygon(source, suggestedSourcePoly, polygons, tree, navMesh);
		if(sourcePoly == -1)	{ m_state = YOKE_FAILED; return std::vector<ObjectCommand_Ptr>(); }
		int destPoly = MoveFunctions::find_nav_polygon(m_dest, -1, polygons, tree, navMesh);
		if(destPoly == -1)		{ m_state = YOKE_FAILED; return std::vector<ObjectCommand_Ptr>(); }

		// FIXME: It's wasteful to copy the array of links each time (even though it's an array of pointers).
		m_links = navMesh->links();

		m_path.reset(new std::list<int>);
		bool pathFound = pathfinder.find_path(source, sourcePoly, m_dest, destPoly, *m_path);
		if(!pathFound)			{ m_state = YOKE_FAILED; return std::vector<ObjectCommand_Ptr>(); }
	}

	// FIXME:	The way this yoke decides that it's traversed a link isn't sufficient.
	//			I've devised a better way of doing it, but it's not yet implemented.

	// Step 1:	If the path is non-empty, go to the next link that we haven't already passed.
	Vector3d dir;
	while(!m_path->empty())
	{
		const Vector3d& linkIn = m_links[m_path->front()]->source_position();
		const Vector3d& linkOut = m_links[m_path->front()]->dest_position();
		dir = linkIn - source;
		if(dir.length() >= 0.1 && source.distance(linkOut) >= 0.1)
		{
			dir.normalize();
			break;
		}
		else
		{
			m_path->pop_front();
		}
	}

	if(!m_path->empty())
	{
		std::vector<ObjectCommand_Ptr> commands;
		commands.push_back(ObjectCommand_Ptr(new CmdBipedMove(m_objectID, dir, cmpMovement->walk_speed())));
		return commands;
	}

	// Step 2:	If the path is empty, go straight to the actual destination.
	dir = m_dest - source;
	if(dir.length() >= 0.1)
	{
		dir.normalize();

		std::vector<ObjectCommand_Ptr> commands;
		commands.push_back(ObjectCommand_Ptr(new CmdBipedMove(m_objectID, dir, cmpMovement->walk_speed())));
		return commands;
	}
	else
	{
		// We've reached the destination.
		m_state = YOKE_SUCCEEDED;
		return std::vector<ObjectCommand_Ptr>();
	}
}

}
