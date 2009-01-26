/***
 * hesperus: Level.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL
#define H_HESP_LEVEL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/bsp/BSPTree.h>
#include <source/level/entities/EntityManager.h>
#include <source/level/nav/NavDataset.h>
#include <source/level/onionbsp/OnionTree.h>
#include <source/level/portals/OnionPortal.h>
#include <source/level/portals/Portal.h>
#include <source/level/vis/VisTable.h>
#include "GeometryRenderer.h"

namespace hesp {

class Level
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	typedef std::vector<OnionPortal_Ptr> OnionPortalVector;
	typedef std::vector<Portal_Ptr> PortalVector;

	//#################### PRIVATE VARIABLES ####################
private:
	GeometryRenderer_Ptr m_geomRenderer;
	BSPTree_Ptr m_tree;
	PortalVector m_portals;
	LeafVisTable_Ptr m_leafVis;
	ColPolyVector m_onionPolygons;
	OnionTree_Ptr m_onionTree;
	OnionPortalVector m_onionPortals;
	// TODO: Navigation stuff
	EntityManager_Ptr m_entityManager;

	//#################### CONSTRUCTORS ####################
public:
	Level(const GeometryRenderer_Ptr& geomRenderer, const BSPTree_Ptr& tree,
		  const PortalVector& portals, const LeafVisTable_Ptr& leafVis,
		  const ColPolyVector& onionPolygons, const OnionTree_Ptr& onionTree,
		  const OnionPortalVector& onionPortals, const std::vector<NavDataset_Ptr>& navDatasets,
		  const EntityManager_Ptr& entityManager);

	//#################### PUBLIC METHODS ####################
public:
	const EntityManager_Ptr& entity_manager() const;
	void render() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Level> Level_Ptr;
typedef shared_ptr<const Level> Level_CPtr;

}

#endif
