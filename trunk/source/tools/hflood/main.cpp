/***
 * hflood: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <source/io/EntitiesFileUtil.h>
#include <source/io/GeometryFileUtil.h>
#include <source/io/PortalsFileUtil.h>
#include <source/io/TreeFileUtil.h>
#include <source/level/bsp/BSPTree.h>
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
	std::cout << "Usage: hflood {-r|-c} <input tree> <input portals> <input entities> <output geometry>" << std::endl;
	exit(EXIT_FAILURE);
}

Vector3d load_player_pos(const std::string& entitiesFilename, const bf::path& settingsDir)
{
#if 1
	// TODO: This will need to be changed when we change the entities file format.
	std::ifstream is(entitiesFilename.c_str());
	if(is.fail()) throw Exception("Could not open " + entitiesFilename + " for reading");

	std::string line;
	if(!std::getline(is,line)) throw Exception("Unexpected EOF whilst trying to read player position");

	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;

	tokenizer tok(line.begin(), line.end(), sep(" "));
	std::vector<std::string> tokens(tok.begin(), tok.end());
	if(tokens.size() != 5) throw Exception("Invalid player position specification");

	std::vector<std::string> components(&tokens[1], &tokens[4]);

	return Vector3d(components);
#else
	EntityManager_Ptr entityManager = EntitiesFileUtil::load(entitiesFilename, settingsDir);

	// NYI
	throw 23;
#endif
}

void flood_from(int leaf, const std::map<int,std::vector<Portal_Ptr> >& portalsFromLeaf, std::set<int>& validLeaves)
{
	validLeaves.insert(leaf);

	std::map<int,std::vector<Portal_Ptr> >::const_iterator it = portalsFromLeaf.find(leaf);
	if(it != portalsFromLeaf.end())
	{
		const std::vector<Portal_Ptr>& outPortals = it->second;
		int outPortalCount = static_cast<int>(outPortals.size());
		for(int j=0; j<outPortalCount; ++j)
		{
			Portal_Ptr outPortal = outPortals[j];
			int toLeaf = outPortal->auxiliary_data().toLeaf;

			// If the destination of this portal is already marked as valid, don't recurse.
			if(validLeaves.find(toLeaf) != validLeaves.end()) continue;

			// Otherwise, recursively flood from the leaf on the other side of this portal.
			flood_from(toLeaf, portalsFromLeaf, validLeaves);
		}
	}
}

template <typename Poly>
void run_flood(const std::string& treeFilename, const std::string& portalsFilename, const std::string& entitiesFilename,
			   const std::string& outputFilename, const bf::path& settingsDir)
{
	// Load the polygons and tree.
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;
	PolyVector polygons;
	BSPTree_Ptr tree;
	TreeFileUtil::load(treeFilename, polygons, tree);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	PortalsFileUtil::load(portalsFilename, emptyLeafCount, portals);

	// Load the player position.
	Vector3d playerPos = load_player_pos(entitiesFilename, settingsDir);

	// Build the "portals from leaf" data structure.
	std::map<int,std::vector<Portal_Ptr> > portalsFromLeaf;
	for(std::vector<Portal_Ptr>::const_iterator it=portals.begin(), iend=portals.end(); it!=iend; ++it)
	{
		int fromLeaf = (*it)->auxiliary_data().fromLeaf;
		portalsFromLeaf[fromLeaf].push_back(*it);
	}

	// Flood out from the player leaf to figure out which leaves are valid.
	int playerLeaf = tree->find_leaf_index(playerPos);

	if(tree->leaf(playerLeaf)->is_solid()) throw Exception("The player start position is in a wall!");

	std::set<int> validLeaves;
	flood_from(playerLeaf, portalsFromLeaf, validLeaves);

	// Copy all the polygons from them to an array.
	PolyVector validPolygons;
	validPolygons.reserve(polygons.size());
	for(std::set<int>::const_iterator it=validLeaves.begin(), iend=validLeaves.end(); it!=iend; ++it)
	{
		const BSPLeaf *leaf = tree->leaf(*it);
		const std::vector<int>& polyIndices = leaf->polygon_indices();
		for(std::vector<int>::const_iterator jt=polyIndices.begin(), jend=polyIndices.end(); jt!=jend; ++jt)
		{
			validPolygons.push_back(polygons[*jt]);
		}
	}

	// Write the polygons to the output file.
	GeometryFileUtil::save(outputFilename, validPolygons);
}

int main(int argc, char *argv[])
try
{
	if(argc != 6) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	// Work out the path to the settings directory.
	bf::path settingsDir = args[0];
	if(!settingsDir.is_complete()) settingsDir = bf::initial_path() / args[0];
	settingsDir = settingsDir.branch_path();	// -> hesperus/bin/tools/
	settingsDir = settingsDir.branch_path();	// -> hesperus/bin/
	settingsDir /= "resources/settings/";		// -> hesperus/bin/resources/settings

	if(args[1] == "-r") run_flood<TexturedPolygon>(args[2], args[3], args[4], args[5], settingsDir);
	else if(args[1] == "-c") run_flood<CollisionPolygon>(args[2], args[3], args[4], args[5], settingsDir);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
