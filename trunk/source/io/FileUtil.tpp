/***
 * hesperus: FileUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>

#include <boost/lexical_cast.hpp>

#include <source/math/geom/GeomUtil.h>

namespace hesp {

/**
Loads the polygons, tree and lightmap prefix from the specified lit tree file.

@param filename			The name of the lit tree file
@param polygons			Used to return the polygons to the caller
@param tree				Used to return the tree to the caller
@param lightmapPrefix	Used to return the lightmap prefix to the caller
*/
template <typename Poly>
void load_lit_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree, std::string& lightmapPrefix)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The lit tree file could not be read");
	load_tree_file(is, polygons, tree);

	std::string line;
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read separator after tree section");
	if(line != "***") throw Exception("Bad separator between the tree and lightmap prefix sections");

	if(!std::getline(is, lightmapPrefix)) throw Exception("Unexpected EOF whilst trying to read lightmap prefix");
}

/**
Loads the polygons and tree from the specified tree file.

@param filename		The name of the tree file
@param polygons		Used to return the polygons to the caller
@param tree			Used to return the tree to the caller
*/
template <typename Poly>
void load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The tree file could not be read");
	load_tree_file(is, polygons, tree);
}

/**
Loads the polygons and tree from the specified std::istream.

@param is			The std::istream
@param polygons		Used to return the polygons to the caller
@param tree			Used to return the tree to the caller
*/
template <typename Poly>
void load_tree_file(std::istream& is, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree)
{
	// Read the polygons.
	std::string line;
	try
	{
		std::getline(is, line);
		int polyCount = boost::lexical_cast<int,std::string>(line);
		load_polygons(is, polygons, polyCount);
	}
	catch(boost::bad_lexical_cast&) { throw Exception("The polygon count is not an integer"); }

	// Read the separator.
	std::getline(is, line);
	if(line != "***") throw Exception("Bad separator between the polygons and BSP sections");

	// Read the BSP tree.
	tree = BSPTree::load_postorder_text(is);
}

}
