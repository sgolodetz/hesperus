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

#include <source/math/geom/Polygon.h>
#include <source/math/vectors/Vector3.h>
using namespace hesp;

//#################### ENUMERATIONS ####################
enum GeomType
{
	COLLISION,
	RENDERING
};

//#################### FORWARD DECLARATIONS ####################
void quit_with_error(const std::string& error);
void quit_with_usage();

//#################### CLASSES ####################
struct RenderingVector3d
{
	double x, y, z, u, v;

	RenderingVector3d(double x_, double y_, double z_, double u_, double v_)
	:	x(x_), y(y_), z(z_), u(u_), v(v_)
	{}
};

template <typename Vert> struct VertBuilder;

template <> struct VertBuilder<Vector3d>
{
	static Vector3d build_vertex(const std::vector<std::string>& components)
	{
		if(components.size() != 3) quit_with_error("Incorrect number of vertex components");

		try
		{
			double x = lexical_cast<double,std::string>(components[0]);
			double y = lexical_cast<double,std::string>(components[1]);
			double z = lexical_cast<double,std::string>(components[2]);
			return Vector3d(x,y,z);
		}
		catch(bad_lexical_cast&)
		{
			quit_with_error("One of the vertex components is not a number");

			// This will never happen.
			throw 23;
		}
	}
};

template <> struct VertBuilder<RenderingVector3d>
{
	static RenderingVector3d build_vertex(const std::vector<std::string>& components)
	{
		if(components.size() != 5) quit_with_error("Incorrect number of vertex components");

		try
		{
			double x = lexical_cast<double,std::string>(components[0]);
			double y = lexical_cast<double,std::string>(components[1]);
			double z = lexical_cast<double,std::string>(components[2]);
			double u = lexical_cast<double,std::string>(components[3]);
			double v = lexical_cast<double,std::string>(components[4]);
			return RenderingVector3d(x,y,z,u,v);
		}
		catch(bad_lexical_cast&)
		{
			quit_with_error("One of the vertex components is not a number");

			// This will never happen.
			throw 23;
		}
	}
};

//#################### FUNCTIONS ####################
template <typename Vert, typename AuxData>
void load_polygons(const std::string& inputFilename, std::vector<Polygon<Vert,AuxData> >& polygons)
{
	typedef Polygon<Vert,AuxData> Poly;

	std::ifstream fs(inputFilename.c_str());

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
				vertices.push_back(VertBuilder<Vert>::build_vertex(components));
			}

			// Add the completed polygon to the list.
			polygons.push_back(Poly(vertices, auxData));
		}

		++n;
	}
}

void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hbsp {-r|-c} <input filename> <output filename> [-w<number>]" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Vert, typename AuxData>
void run_compiler(const std::string& inputFilename, const std::string& outputFilename, const double weight)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef std::vector<Poly> PolyVector;

	PolyVector polygons;
	load_polygons(inputFilename, polygons);

	// TODO
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
