/***
 * hbsp: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/bsp/BSPCompiler.h>
#include <source/math/geom/GeomUtil.h>
#include <source/math/geom/Polygon.h>
#include <source/math/vectors/RenderingVector3d.h>
#include <source/math/vectors/Vector3.h>
#include <source/math/vectors/VecUtil.h>
#include "QuitFunctions.h"
using namespace hesp;

//#################### ENUMERATIONS ####################
enum GeomType
{
	COLLISION,
	RENDERING
};

//#################### FUNCTIONS ####################
template <typename Vert, typename AuxData>
void load_polygons(const std::string& inputFilename, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	std::ifstream fs(inputFilename.c_str());
	if(fs.fail()) quit_with_error("Input file does not exist");

	std::string line;
	int n = 1;
	while(std::getline(fs, line))
	{
		boost::trim(line);
		if(line != "")
		{
			// Read the vertex count.
			std::string::size_type L = line.find(' ');
			if(L == std::string::npos) quit_with_error("Bad input on line " + lexical_cast<std::string,int>(n));
			std::string vertCountString = line.substr(0,L);
			int vertCount;
			try							{ vertCount = lexical_cast<int,std::string>(vertCountString); }
			catch(bad_lexical_cast&)	{ quit_with_error("Bad vertex count on line " + lexical_cast<std::string,int>(n)); }

			// Read the auxiliary data.
			std::string::size_type R = line.find_last_of(") ");
			if(R == std::string::npos || R+1 >= line.length()) quit_with_error("Bad input on line " + lexical_cast<std::string,int>(n));
			std::string auxDataString = line.substr(R+1);
			boost::trim(auxDataString);
			AuxData auxData;
			try							{ auxData = lexical_cast<AuxData,std::string>(auxDataString); }
			catch(bad_lexical_cast&)	{ quit_with_error("Bad auxiliary data on line " + lexical_cast<std::string,int>(n)); }

			// Read the vertices.
			std::string verticesString = line.substr(L+1, R-L-1);
			std::vector<Vert> vertices;

			typedef boost::char_separator<char> sep;
			typedef boost::tokenizer<sep> tokenizer;
			tokenizer tok(verticesString.begin(), verticesString.end(), sep(" "));
			std::vector<std::string> tokens(tok.begin(), tok.end());
			int tokensPerVert = static_cast<int>(tokens.size()) / vertCount;
			if(tokensPerVert < 3) quit_with_error("Bad vertex data on line " + lexical_cast<std::string,int>(n));

			for(int i=0; i<vertCount; ++i)
			{
				int offset = i*tokensPerVert;
				if(tokens[offset] != "(" || tokens[offset+tokensPerVert-1] != ")") quit_with_error("Bad vertex data on line " + lexical_cast<std::string,int>(n));

				std::vector<std::string> components;
				std::copy(&tokens[offset+1], &tokens[offset+tokensPerVert-1], std::back_inserter(components));
				vertices.push_back(VecUtil<Vert>::build_vector(components));
			}

			// Add the completed polygon to the list.
			polygons.push_back(Poly_Ptr(new Poly(vertices, auxData)));
		}

		++n;
	}
}

template <typename Vert, typename AuxData>
void run_compiler(const std::string& inputFilename, const std::string& outputFilename, const double weight)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Load the input polygons from disk.
	PolyVector polygons;
	load_polygons(inputFilename, polygons);

	// Build the BSP tree.
	BSPTree_Ptr tree = BSPCompiler::build_tree(polygons, weight);

	// Save the polygons and the BSP tree to the output file.
	std::ofstream fs(outputFilename.c_str());
	if(fs.fail()) quit_with_error("Couldn't open output file for writing");
	write_polygons(fs, polygons);
	fs << "***\n";
	tree->output_postorder_text(fs);
}

template <typename Vert, typename AuxData>
void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	os << polygons.size() << '\n';
	for(PolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		const Poly& curPoly = **it;
		int vertCount = curPoly.vertex_count();
		os << vertCount << ' ';
		for(int j=0; j<vertCount; ++j)
		{
			VecUtil<Vert>::output_vector(os, curPoly.vertex(j));
			os << ' ';
		}
		os << curPoly.auxiliary_data() << '\n';
	}
}

int main(int argc, char *argv[])
{
	if(argc != 4 && argc != 5) quit_with_usage();

	const std::vector<std::string> args(argv, argv+argc);

	GeomType geomType;
	if(args[1] == "-r") geomType = RENDERING;
	else if(args[1] == "-c") geomType = COLLISION;
	else quit_with_usage();

	std::string inputFilename = args[2];
	std::string outputFilename = args[3];

	double weight = 4;
	if(argc == 5)
	{
		if(args[4].substr(0,2) != "-w") quit_with_usage();

		try							{ weight = lexical_cast<double,std::string>(args[4].substr(2)); }
		catch(bad_lexical_cast&)	{ quit_with_usage(); }
	}

	switch(geomType)
	{
		case COLLISION:
		{
			run_compiler<Vector3d,bool>(inputFilename, outputFilename, weight);
			break;
		}
		case RENDERING:
		{
			run_compiler<RenderingVector3d,std::string>(inputFilename, outputFilename, weight);
			break;
		}
	}

	return 0;
}
