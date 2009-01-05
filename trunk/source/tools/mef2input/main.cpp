/***
 * mef2input: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/level/csg/PolyhedralBrush.h>
#include <source/math/geom/AABB.h>
#include <source/math/geom/GeomUtil.h>
#include <source/util/PolygonTypes.h>
#include "TexturePlane.h"
using namespace hesp;

//#################### CLASSES ####################
struct MEFAuxData
{
	std::string texture;
	TexturePlane_Ptr texturePlane;
};

TexturePlane_Ptr read_texture_plane(std::istream& is)
{
	std::string dummy;
	double offsetU, offsetV, scaleU, scaleV, angleDegrees;
	is >> dummy >> offsetU >> offsetV >> scaleU >> scaleV >> angleDegrees >> dummy;
	return TexturePlane_Ptr(new TexturePlane(offsetU, offsetV, scaleU, scaleV, angleDegrees));
}

std::istream& operator>>(std::istream& is, MEFAuxData& rhs)
{
	is >> std::skipws;
	is >> rhs.texture;
	rhs.texturePlane = read_texture_plane(is);
	is >> std::noskipws;
	return is;
}

//#################### TYPEDEFS ####################
typedef hesp::Polygon<Vector3d,MEFAuxData> MEFPolygon;
typedef shared_ptr<MEFPolygon> MEFPolygon_Ptr;

typedef PolyhedralBrush<TexturedPolygon> TexPolyhedralBrush;
typedef shared_ptr<TexPolyhedralBrush> TexPolyhedralBrush_Ptr;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: mef2input <input MEF> <output brushes> <output entities> <output lights>" << std::endl;
	exit(EXIT_FAILURE);
}

void read_line(std::istream& is, std::string& line, const std::string& attemptedAction)
{
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to " + attemptedAction);
}

void skip_section(std::istream& is)
{
	std::string line;

	int bracketCount = 0;
	do
	{
		if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to skip section");
		if(line == "{") ++bracketCount;
		if(line == "}") --bracketCount;
	} while(bracketCount > 0);
}

void read_polyhedral_brush(std::istream& is, std::vector<TexPolyhedralBrush_Ptr>& brushes)
{
	const double SCALE = 1.0/32;	// we want a 32-unit grid square to correspond to 1 metre in the world

	std::string line;

	read_line(is, line, "read PolyhedralBrush");
	if(line != "{") throw Exception("PolyhedralBrush: Expected {");

	read_line(is, line, "read bounds");
	if(line.substr(0,6) != "Bounds" || line.length() < 8) throw Exception("PolyhedralBrush: Expected Bounds");

	// Parse bounds.
	line = line.substr(7);
	AABB3d bounds = read_aabb<Vector3d>(line, SCALE);

	// Read polygon count.
	read_line(is, line, "read polygon count");
	if(line.substr(0,9) != "PolyCount" || line.length() < 11) throw Exception("PolyhedralBrush: Expected PolyCount");

	int polyCount;
	try							{ polyCount = lexical_cast<int,std::string>(line.substr(10)); }
	catch(bad_lexical_cast&)	{ throw Exception("PolyhedralBrush: Polygon count is not an integer"); }

	// Read polygons.
	std::vector<TexturedPolygon_Ptr> faces;
	for(int i=0; i<polyCount; ++i)
	{
		read_line(is, line, "read polygon");
		if(line.substr(0,7) != "Polygon" || line.length() < 9) throw Exception("PolyhedralBrush: Expected Polygon");

		// Parse polygon.
		MEFPolygon_Ptr poly = load_polygon<Vector3d,MEFAuxData>(line.substr(8));

		// Convert polygon to hesperus form.
		std::vector<TexturedVector3d> newVertices;
		TexturePlane_Ptr& texturePlane = poly->auxiliary_data().texturePlane;
		texturePlane->determine_axis_vectors(poly->normal());
		int vertCount = poly->vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			Vector3d oldVert = poly->vertex(j);
			TexCoords texCoords = texturePlane->calculate_coordinates(oldVert);
			oldVert *= SCALE;
			newVertices.push_back(TexturedVector3d(oldVert.x, oldVert.y, oldVert.z, texCoords.u, texCoords.v));
		}
		faces.push_back(TexturedPolygon_Ptr(new TexturedPolygon(newVertices, poly->auxiliary_data().texture)));
	}
	brushes.push_back(TexPolyhedralBrush_Ptr(new TexPolyhedralBrush(bounds, faces)));

	read_line(is, line, "read PolyhedralBrush");
	if(line != "}") throw Exception("PolyhedralBrush: Expected }");
}

void read_architecture_brush_composite(std::istream& is, std::vector<TexPolyhedralBrush_Ptr>& brushes)
{
	std::string line;
	read_line(is, line, "read ArchitectureBrushComposite");
	if(line != "{") throw Exception("ArchitectureBrushComposite: Expected {");

	for(;;)
	{
		read_line(is, line, "read ArchitectureBrushComposite");
		if(line == "}") break;

		if(line == "ArchitectureBrushComposite") read_architecture_brush_composite(is, brushes);
		else if(line == "PolyhedralBrush") read_polyhedral_brush(is, brushes);
		else
		{
			std::cout << "Warning: Don't know how to read a " << line << " subsection of ArchitectureBrushComposite" << std::endl;
			skip_section(is);
		}
	}
}

void write_brushes(const std::string& filename, const std::vector<TexPolyhedralBrush_Ptr>& brushes)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	int brushCount = static_cast<int>(brushes.size());
	for(int i=0; i<brushCount; ++i)
	{
		os << *brushes[i];
	}
}

void run_converter(const std::string& inputFilename, const std::string& brushesFilename, const std::string& entitiesFilename, const std::string& lightsFilename)
{
	std::vector<TexPolyhedralBrush_Ptr> brushes;

	// Read in the MEF file.
	std::ifstream is(inputFilename.c_str());
	if(is.fail()) throw Exception("Could not open " + inputFilename + " for reading");

	std::string line;

	read_line(is, line, "read MEF ID");
	if(line != "MEF 2") throw Exception("Bad MEF ID or unexpected file version");

	read_line(is, line, "read Textures");
	if(line != "Textures") throw Exception("Textures section is missing");
	skip_section(is);

	while(std::getline(is, line))
	{
		if(line == "ArchitectureBrushComposite") read_architecture_brush_composite(is, brushes);
		else if(line == "PolyhedralBrush") read_polyhedral_brush(is, brushes);
		else
		{
			std::cout << "Warning: Don't know how to read a " << line << " section" << std::endl;
			skip_section(is);
		}
	}

	// Write the brushes to disk.
	write_brushes(brushesFilename, brushes);

	// Write the entities to disk.
	// TODO

	// Write the lights to disk.
	// TODO
}

int main(int argc, char *argv[])
try
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_converter(args[1], args[2], args[3], args[4]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
