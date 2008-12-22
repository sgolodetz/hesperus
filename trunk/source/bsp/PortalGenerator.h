/***
 * hesperus: PortalGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_PORTALGENERATOR
#define H_HESP_BSP_PORTALGENERATOR

#include <list>

#include <source/math/geom/Polygon.h>
#include "BSPTree.h"

namespace hesp {

struct LeafLink
{
	int fromLeaf;
	int toLeaf;
};

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,LeafLink> Portal;
typedef shared_ptr<Portal> Portal_Ptr;

class PortalGenerator
{
	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData>
	static std::vector<Portal_Ptr> generate_portals(const BSPTree_Ptr& tree, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons);

	//#################### PRIVATE METHODS ####################
private:
	static std::list<Portal_Ptr> clip_portal_to_subtree(const Portal_Ptr& portal, const BSPNode_Ptr& subtreeRoot, PlaneClassifier relativeToPortal = CP_STRADDLE);
	// TODO: std::set<Plane> find_unique_planes(polygons)
	static Portal_Ptr make_initial_portal(const Plane& plane);
};

}

#include "PortalGenerator.tpp"

#endif
