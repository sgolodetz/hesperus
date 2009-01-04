/***
 * hcsg: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/level/csg/CSGUtil.h>
using namespace hesp;

int main(int argc, char *argv[])
{
	typedef Polygon<Vector3d,std::string> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	typedef PolyhedralBrush<Vector3d,std::string> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;

	std::vector<PolyBrush_Ptr> brushes;

	std::vector<Poly_Ptr> faces;
	faces.reserve(6);
	{
		// Front
		std::vector<Vector3d> verts;
		verts.push_back(Vector3d(0,0,0));
		verts.push_back(Vector3d(1,0,0));
		verts.push_back(Vector3d(1,0,1));
		verts.push_back(Vector3d(0,0,1));
		faces.push_back(Poly_Ptr(new Poly(verts, "")));
	}
	{
		// Right
		std::vector<Vector3d> verts;
		verts.push_back(Vector3d(1,0,0));
		verts.push_back(Vector3d(1,1,0));
		verts.push_back(Vector3d(1,1,1));
		verts.push_back(Vector3d(1,0,1));
		faces.push_back(Poly_Ptr(new Poly(verts, "")));
	}
	{
		// Back
		std::vector<Vector3d> verts;
		verts.push_back(Vector3d(1,1,0));
		verts.push_back(Vector3d(0,1,0));
		verts.push_back(Vector3d(0,1,1));
		verts.push_back(Vector3d(1,1,1));
		faces.push_back(Poly_Ptr(new Poly(verts, "")));
	}
	{
		// Left
		std::vector<Vector3d> verts;
		verts.push_back(Vector3d(0,1,0));
		verts.push_back(Vector3d(0,0,0));
		verts.push_back(Vector3d(0,0,1));
		verts.push_back(Vector3d(0,1,1));
		faces.push_back(Poly_Ptr(new Poly(verts, "")));
	}
	{
		// Top
		std::vector<Vector3d> verts;
		verts.push_back(Vector3d(0,0,1));
		verts.push_back(Vector3d(1,0,1));
		verts.push_back(Vector3d(1,1,1));
		verts.push_back(Vector3d(0,1,1));
		faces.push_back(Poly_Ptr(new Poly(verts, "")));
	}
	{
		// Bottom
		std::vector<Vector3d> verts;
		verts.push_back(Vector3d(0,1,0));
		verts.push_back(Vector3d(1,1,0));
		verts.push_back(Vector3d(1,0,0));
		verts.push_back(Vector3d(0,0,0));
		faces.push_back(Poly_Ptr(new Poly(verts, "")));
	}
	Vector3d minimum(0,0,0), maximum(1,1,1);
	AABB3d bounds(minimum, maximum);

	brushes.push_back(PolyBrush_Ptr(new PolyBrush(bounds, faces)));

	CSGUtil<Vector3d,std::string>::union_all(brushes);
	return 0;
}
