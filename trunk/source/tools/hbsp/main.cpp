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
#include "QuitFunctions.h"
#include "RenderingVector3d.h"
#include "VertBuilder.h"
using namespace hesp;

//#################### ENUMERATIONS ####################
enum GeomType
{
	COLLISION,
	RENDERING
};

//#################### CLASSES ####################
struct PolyIndex
{
	int index;
	bool splitCandidate;	// is the plane of the referenced polygon a split candidate?

	PolyIndex(int index_, bool splitCandidate_)
	:	index(index_), splitCandidate(splitCandidate_)
	{}
};

//#################### FUNCTIONS ####################
template <typename Vert, typename AuxData>
BSPNode_Ptr build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight,
						  PlaneClassifier relativeToParent = CP_BACK)
{
	Plane_Ptr splitter = choose_split_plane(polyIndices, polygons, weight);

	if(splitter.get() == NULL)
	{
		if(relativeToParent == CP_BACK)
		{
			return BSPLeaf::make_solid_leaf();
		}
		else
		{
			std::vector<int> indicesOnly;
			for(size_t i=0, size=polyIndices.size(); i<size; ++i) indicesOnly.push_back(polyIndices[i].index);
			return BSPLeaf::make_empty_leaf(indicesOnly);
		}
	}

	std::vector<PolyIndex> backPolys, frontPolys;

	for(std::vector<PolyIndex>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		int curIndex = it->index;
		const Polygon<Vert,AuxData>& curPoly = *polygons[curIndex];
		switch(classify_polygon_against_plane(curPoly, *splitter))
		{
			case CP_BACK:
			{
				backPolys.push_back(*it);
				break;
			}
			case CP_COPLANAR:
			{
				if(splitter->normal().dot(curPoly.normal()) > 0) frontPolys.push_back(PolyIndex(curIndex,false));
				else backPolys.push_back(PolyIndex(curIndex,false));
				break;
			}
			case CP_FRONT:
			{
				frontPolys.push_back(*it);
				break;
			}
			case CP_STRADDLE:
			{
				SplitResults<Vert,AuxData> sr = split_polygon(curPoly, *splitter);
				polygons[curIndex] = sr.back;
				int k = static_cast<int>(polygons.size());
				polygons.push_back(sr.front);
				backPolys.push_back(PolyIndex(curIndex,it->splitCandidate));
				frontPolys.push_back(PolyIndex(k,it->splitCandidate));
				break;
			}
		}
	}

	BSPNode_Ptr left = build_subtree(frontPolys, polygons, weight, CP_FRONT);
	BSPNode_Ptr right = build_subtree(backPolys, polygons, weight, CP_BACK);

	BSPNode_Ptr subtreeRoot(new BSPBranch(splitter, left, right));
	left->set_parent(subtreeRoot.get());
	right->set_parent(subtreeRoot.get());
	return subtreeRoot;
}

template <typename Vert, typename AuxData>
BSPNode_Ptr build_tree(std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight)
{
	int polyCount = static_cast<int>(polygons.size());
	std::vector<PolyIndex> polyIndices;
	polyIndices.reserve(polyCount);
	for(int i=0; i<polyCount; ++i) polyIndices.push_back(PolyIndex(i, true));
	BSPNode_Ptr root = build_subtree(polyIndices, polygons, weight);

	return root;
}

template <typename Vert, typename AuxData>
Plane_Ptr choose_split_plane(const std::vector<PolyIndex>& polyIndices, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight)
{
	Plane_Ptr bestPlane;
	double bestMetric = INT_MAX;

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		if(!polyIndices[i].splitCandidate) continue;

		Plane plane = make_plane(*polygons[polyIndices[i].index]);
		int balance = 0, splits = 0;

		for(int j=0; j<indexCount; ++j)
		{
			if(j == i) continue;

			switch(classify_polygon_against_plane(*polygons[polyIndices[j].index], plane))
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
