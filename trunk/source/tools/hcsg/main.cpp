/***
 * hcsg: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <source/exceptions/Exception.h>
#include <source/level/csg/CSGUtil.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hcsg {-r|-c} <input brushes> <output geometry>" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_csg(const std::string& inputFilename, const std::string& outputFilename)
{
	typedef typename Poly::Vert Vert;
	typedef typename Poly::AuxData AuxData;

	// Read in the brushes.
	typedef PolyhedralBrush<Vert,AuxData> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;
	typedef std::vector<PolyBrush_Ptr> PolyBrushVector;
	PolyBrushVector brushes = FileUtil::load_brushes_file<Vert,AuxData>(inputFilename);

	// TODO
}

int main(int argc, char *argv[])
try
{
	if(argc != 4) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	if(args[1] == "-r") run_csg<TexturedPolygon>(args[2], args[3]);
	else if(args[1] == "-c") run_csg<CollisionPolygon>(args[2], args[3]);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }

#if 0
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
#endif
