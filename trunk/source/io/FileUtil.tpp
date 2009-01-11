/***
 * hesperus: FileUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>

#include <boost/lexical_cast.hpp>

#include <source/math/geom/GeomUtil.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads an array of polyhedral brushes from the specified brushes file.

@param filename	The name of the brushes file
@return			The brushes
*/
template <typename Poly>
std::vector<shared_ptr<PolyhedralBrush<Poly> > > FileUtil::load_brushes_file(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	typedef PolyhedralBrush<Poly> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;
	typedef std::vector<PolyBrush_Ptr> PolyBrushVector;

	PolyBrushVector brushes;
	PolyBrush_Ptr brush;
	while(brush = load_polyhedral_brush<Poly>(is))
	{
		brushes.push_back(brush);
	}

	return brushes;
}

/**
Loads the polygons, tree and lightmap prefix from the specified lit tree file.

@param filename			The name of the lit tree file
@param polygons			Used to return the polygons to the caller
@param tree				Used to return the tree to the caller
@param lightmapPrefix	Used to return the lightmap prefix to the caller
*/
template <typename Poly>
void FileUtil::load_lit_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree, std::string& lightmapPrefix)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The lit tree file could not be read");

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	load_separator(is);
	tree = FileSectionUtil::load_tree_section(is);
	load_separator(is);
	lightmapPrefix = FileSectionUtil::load_lightmap_prefix_section(is);
}

/**
Loads the polygons and onion tree from the specified onion tree file.

@param filename		The name of the onion tree file
@param polygons		Used to return the polygons to the caller
@param tree			Used to return the onion tree to the caller
*/
template <typename Poly>
void FileUtil::load_onion_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, OnionTree_Ptr& tree)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The onion tree file could not be read");

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	load_separator(is);
	tree = FileSectionUtil::load_onion_tree_section(is);
}

/**
Loads an array of polygons from the specified file.

@param filename		The name of the file
@param polygons		Used to return the polygons to the caller
*/
template <typename Poly>
void FileUtil::load_polygons_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The polygons file could not be read");

	FileSectionUtil::load_uncounted_polygons(is, polygons);
}

/**
Loads the polygons and tree from the specified tree file.

@param filename		The name of the tree file
@param polygons		Used to return the polygons to the caller
@param tree			Used to return the tree to the caller
*/
template <typename Poly>
void FileUtil::load_tree_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The tree file could not be read");

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	load_separator(is);
	tree = FileSectionUtil::load_tree_section(is);
}

/**
Saves a brushes file to disk.

@param filename		The name of the file to which to save the brushes
@param brushes		The brushes to save
@throws Exception	If the output file could not be opened for writing
*/
template <typename Poly>
void FileUtil::save_brushes_file(const std::string& filename, const std::vector<shared_ptr<PolyhedralBrush<Poly> > >& brushes)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	int brushCount = static_cast<int>(brushes.size());
	for(int i=0; i<brushCount; ++i)
	{
		FileSectionUtil::write_polyhedral_brush(os, *brushes[i]);
	}
}

//#################### PRIVATE METHODS ####################
/**
Loads a polyhedral brush from a std::istream.

@param is			The std::istream
@return				The polyhedral brush, or NULL if EOF was encountered
@throws Exception	If anything else goes wrong whilst trying to read the polyhedral brush
*/
template <typename Poly>
shared_ptr<PolyhedralBrush<Poly> > FileUtil::load_polyhedral_brush(std::istream& is)
{
	typedef PolyhedralBrush<Poly> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;

	std::string line;

	if(!std::getline(is,line)) return PolyBrush_Ptr();
	if(line != "{") throw Exception("Expected {");

	// Read bounds.
	if(!std::getline(is,line)) throw Exception("Unexpected EOF whilst trying to read bounds");
	AABB3d bounds = read_aabb<Vector3d>(line);

	// Read faces.
	typedef shared_ptr<Poly> Poly_Ptr;
	std::vector<Poly_Ptr> faces;
	FileSectionUtil::load_counted_polygons(is, faces);

	if(!std::getline(is,line)) throw Exception("Unexpected EOF whilst trying to read }");
	if(line != "}") throw Exception("Expected }");

	return PolyBrush_Ptr(new PolyBrush(bounds, faces));
}

}
