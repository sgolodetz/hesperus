/***
 * hesperus: TreeFileUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/exceptions/Exception.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads the polygons and BSP tree from the specified tree file.

@param filename		The name of the tree file
@param polygons		Used to return the polygons to the caller
@param tree			Used to return the tree to the caller
*/
template <typename Poly>
void TreeFileUtil::load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	tree = FileSectionUtil::load_tree_section(is);
}

//#################### SAVING METHODS ####################
/**
Saves the polygons and BSP tree to the specified tree file.

@param filename		The name of the tree file
@param polygons		The polygons
@param tree			The tree
*/
template <typename Poly>
void TreeFileUtil::save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons, const BSPTree_Ptr& tree)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	FileSectionUtil::save_tree_section(os, tree);
}

}
