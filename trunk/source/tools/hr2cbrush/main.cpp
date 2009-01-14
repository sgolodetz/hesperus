/***
 * hr2cbrush: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/io/BrushesFileUtil.h>
#include <source/level/brushes/PolyhedralBrush.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

//#################### TYPEDEFS ####################
typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;
typedef PolyhedralBrush<TexturedPolygon> TexPolyBrush;
typedef shared_ptr<TexPolyBrush> TexPolyBrush_Ptr;
typedef std::vector<TexPolyBrush_Ptr> TexPolyBrushVector;

typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
typedef PolyhedralBrush<CollisionPolygon> ColPolyBrush;
typedef shared_ptr<ColPolyBrush> ColPolyBrush_Ptr;
typedef std::vector<ColPolyBrush_Ptr> ColPolyBrushVector;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hr2cbrush <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

ColPolyAuxData make_aux_data(const Vector3d& faceNormal)
{
	// TODO: CPAuxData will eventually store more interesting things (see its definition).

	const double MAX_ANGLE_TO_VERTICAL = 45 * PI/180;	// i.e. 45 degrees
	double angleToVertical = acos(faceNormal.dot(Vector3d(0,0,1)));
	bool walkable = fabs(angleToVertical) <= MAX_ANGLE_TO_VERTICAL;

	return ColPolyAuxData(walkable);
}

ColPolyBrush_Ptr convert_brush(const TexPolyBrush_Ptr& texBrush)
{
	AABB3d bounds = texBrush->bounds();
	const TexPolyVector& texFaces = texBrush->faces();

	int faceCount = static_cast<int>(texFaces.size());
	ColPolyVector colFaces(faceCount);
	for(int i=0; i<faceCount; ++i)
	{
		int vertCount = texFaces[i]->vertex_count();
		std::vector<Vector3d> colVertices;
		colVertices.reserve(vertCount);
		for(int j=0; j<vertCount; ++j)
		{
			// Note: There is an implicit conversion from TexturedVector3d -> Vector3d taking place here.
			colVertices.push_back(texFaces[i]->vertex(j));
		}
		CollisionPolygon::AuxData colAuxData = make_aux_data(texFaces[i]->normal());
		colFaces[i].reset(new CollisionPolygon(colVertices, colAuxData));
	}

	return ColPolyBrush_Ptr(new ColPolyBrush(bounds, colFaces));
}

void run_converter(const std::string& inputFilename, const std::string& outputFilename)
{
	// Read in the rendering brushes.
	TexPolyBrushVector inputBrushes = BrushesFileUtil::load<TexturedPolygon>(inputFilename);

	// Convert each rendering brush into a collision brush.
	int brushCount = static_cast<int>(inputBrushes.size());
	ColPolyBrushVector outputBrushes(brushCount);
	for(int i=0; i<brushCount; ++i)
	{
		outputBrushes[i] = convert_brush(inputBrushes[i]);
	}

	// Write the collision brushes to disk.
	BrushesFileUtil::save(outputFilename, outputBrushes);
}

int main(int argc, char *argv[])
try
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_converter(args[1], args[2]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
