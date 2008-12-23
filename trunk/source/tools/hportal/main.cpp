/***
 * hportal: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <iostream>
#include <list>

#include <source/bsp/PortalGenerator.h>
#include <source/math/geom/GeomUtil.h>
#include <source/math/geom/Polygon.h>
#include <source/math/vectors/RenderingVector3d.h>
using namespace hesp;

int main(int argc, char *argv[])
{
#if 0
	typedef Polygon<RenderingVector3d,std::string> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	std::vector<Poly_Ptr> polygons;
	load_polygons(std::cin, polygons, 1);

	shared_ptr<std::list<Portal_Ptr> > portals = PortalGenerator::generate_portals(BSPTree_Ptr(), polygons);
#endif

	typedef PortalGenerator::PlaneRepPred PlaneRepPred;
	RepresentativeTree<Plane, PlaneRepPred> repTree(PlaneRepPred(0.5 * PI / 180, 0.001));
	repTree.insert(Plane(Vector3d(1,0,0),0).to_undirected_form());
	repTree.insert(Plane(Vector3d(-1,0,0),0).to_undirected_form());
	repTree.insert(Plane(Vector3d(-2,0,-2),0).to_undirected_form());
	repTree.insert(Plane(Vector3d(1,0,1),0).to_undirected_form());
	repTree.insert(Plane(Vector3d(1,0,-1),0).to_undirected_form());
	repTree.insert(Plane(Vector3d(1,0,-1.01),0).to_undirected_form());

	shared_ptr<std::list<Plane> > planes = repTree.representatives();
	if(planes)
	{
		std::copy(planes->begin(), planes->end(), std::ostream_iterator<Plane>(std::cout, "\n"));
	}

	return 0;
}
