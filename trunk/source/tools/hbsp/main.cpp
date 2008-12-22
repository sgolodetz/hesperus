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

#include <source/bsp/BSPBranch.h>
#include <source/bsp/BSPLeaf.h>
#include <source/math/geom/GeomUtil.h>
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

	RenderingVector3d() : x(0), y(0), z(0), u(0), v(0) {}

	RenderingVector3d(double x_, double y_, double z_, double u_, double v_)
	:	x(x_), y(y_), z(z_), u(u_), v(v_)
	{}

	RenderingVector3d& operator+=(const RenderingVector3d& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		u += rhs.u;
		v += rhs.v;
		return *this;
	}

	RenderingVector3d& operator-=(const RenderingVector3d& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		u -= rhs.u;
		v -= rhs.v;
		return *this;
	}

	RenderingVector3d& operator*=(double factor)
	{
		x *= factor;
		y *= factor;
		z *= factor;
		u *= factor;
		v *= factor;
		return *this;
	}

	operator Vector3d() const
	{
		return Vector3d(x,y,z);
	}
};

RenderingVector3d operator+(const RenderingVector3d& lhs, const RenderingVector3d& rhs)
{
	RenderingVector3d copy(lhs);
	copy += rhs;
	return copy;
}

RenderingVector3d operator-(const RenderingVector3d& lhs, const RenderingVector3d& rhs)
{
	RenderingVector3d copy(lhs);
	copy -= rhs;
	return copy;
}

RenderingVector3d operator*(double factor, const RenderingVector3d& v)
{
	RenderingVector3d copy(v);
	copy *= factor;
	return copy;
}

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
BSPNode_Ptr build_subtree(const std::vector<int>& polyIndices, const std::vector<int>& extraIndices, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons,
						  const double weight)
{
	Plane_Ptr splitter = choose_split_plane(polyIndices, polygons, weight);

	std::vector<int> backPolys, frontPolys;
	std::vector<int> backExtra, frontExtra(extraIndices);

	for(std::vector<int>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		int curIndex = *it;
		const Polygon<Vert,AuxData>& curPoly = *polygons[curIndex];
		switch(classify_polygon_against_plane(curPoly, *splitter))
		{
			case CP_BACK:
			{
				backPolys.push_back(curIndex);
				break;
			}
			case CP_COPLANAR:
			{
				if(splitter->normal().dot(curPoly.normal()) > 0) frontExtra.push_back(curIndex);
				else backExtra.push_back(curIndex);
				break;
			}
			case CP_FRONT:
			{
				frontPolys.push_back(curIndex);
				break;
			}
			case CP_STRADDLE:
			{
				SplitResults<Vert,AuxData> sr = split_polygon(curPoly, *splitter);
				int k = static_cast<int>(polygons.size());
				polygons.push_back(sr.back);
				polygons.push_back(sr.front);
				backPolys.push_back(k);
				frontPolys.push_back(k+1);
				break;
			}
		}
	}

	BSPNode_Ptr left, right;

	if(frontPolys.empty()) left = BSPLeaf::make_empty_leaf(frontExtra);
	else left = build_subtree(frontPolys, frontExtra, polygons, weight);

	if(backPolys.empty()) right = BSPLeaf::make_solid_leaf();
	else right = build_subtree(backPolys, backExtra, polygons, weight);

	BSPNode_Ptr subtreeRoot(new BSPBranch(splitter, left, right));
	left->set_parent(subtreeRoot.get());
	right->set_parent(subtreeRoot.get());
	return subtreeRoot;
}

template <typename Vert, typename AuxData>
BSPNode_Ptr build_tree(std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight)
{
	int polyCount = static_cast<int>(polygons.size());
	std::vector<int> polyIndices(polyCount);
	for(int i=0; i<polyCount; ++i) polyIndices[i] = i;
	std::vector<int> extraIndices;
	BSPNode_Ptr root = build_subtree(polyIndices, extraIndices, polygons, weight);

	// TODO
	std::map<int,int> testIndexMap;
	root->map_polygon_indices(testIndexMap);

	return root;
}

template <typename Vert, typename AuxData>
Plane_Ptr choose_split_plane(const std::vector<int>& polyIndices, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight)
{
	Plane_Ptr bestPlane;
	double bestMetric = INT_MAX;

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		Plane plane = make_plane(*polygons[polyIndices[i]]);
		int balance = 0, splits = 0;

		for(int j=0; j<indexCount; ++j)
		{
			if(j == i) continue;

			switch(classify_polygon_against_plane(*polygons[polyIndices[j]], plane))
			{
				case CP_BACK:
					--balance;
					break;
				case CP_COPLANAR:
					break;
				case CP_FRONT:
					++balance;
					break;
				case CP_STRADDLE:
					++splits;
					break;
			}
		}

		double metric = weight*balance + splits;
		if(metric < bestMetric)
		{
			bestPlane = Plane_Ptr(new Plane(plane));
			bestMetric = metric;
		}
	}

	return bestPlane;
}

template <typename Vert, typename AuxData>
void load_polygons(const std::string& inputFilename, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

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
			polygons.push_back(Poly_Ptr(new Poly(vertices, auxData)));
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
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Load the input polygons from disk.
	PolyVector polygons;
	load_polygons(inputFilename, polygons);

	// Build the BSP tree.
	BSPNode_Ptr tree = build_tree(polygons, weight);

	// Save the polygons and the BSP tree to the output file.
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
