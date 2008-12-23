/***
 * hportal: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <iostream>

#include <source/bsp/PortalGenerator.h>
#include <source/math/geom/GeomUtil.h>
#include <source/math/geom/Polygon.h>
#include <source/math/vectors/RenderingVector3d.h>
using namespace hesp;

int main(int argc, char *argv[])
{
	typedef Polygon<RenderingVector3d,std::string> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	std::vector<Poly_Ptr> polygons;
	load_polygons(std::cin, polygons, 1);

	shared_ptr<std::vector<Portal_Ptr> > portals = PortalGenerator::generate_portals(BSPTree_Ptr(), polygons);

	return 0;
}
