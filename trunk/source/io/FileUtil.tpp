/***
 * hesperus: FileUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

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

	// Read the polygons.
	std::string line;
	try
	{
		std::getline(is, line);
		int polyCount = lexical_cast<int,std::string>(line);
		load_polygons(is, polygons, polyCount);
	}
	catch(bad_lexical_cast&) { throw Exception("The polygon count is not an integer"); }

	// Read the separator.
	std::getline(is, line);
	if(line != "***") throw Exception("Bad separator between the polygons and BSP sections");

	// Read the BSP tree.
	tree = BSPTree::load_postorder_text(is);
}

}
