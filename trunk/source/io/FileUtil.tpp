/***
 * hesperus: FileUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>

#include <boost/lexical_cast.hpp>

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename Vert, typename AuxData>
std::vector<shared_ptr<PolyhedralBrush<Vert,AuxData> > > FileUtil::load_brushes_file(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	typedef PolyhedralBrush<Vert,AuxData> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;
	typedef std::vector<PolyBrush_Ptr> PolyBrushVector;

	PolyBrushVector brushes;
	PolyBrush_Ptr brush;
	while(brush = load_polyhedral_brush<Vert,AuxData>(is))
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

	load_polygons_section(is, polygons);
	load_separator(is);
	tree = load_tree_section(is);
	load_separator(is);
	lightmapPrefix = load_lightmap_prefix_section(is);
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

	load_polygons_section(is, polygons);
	load_separator(is);
	tree = load_tree_section(is);
}

//#################### PRIVATE METHODS ####################
/**
Reads a polygon count, n, followed by an array of n polygons from the specified std::istream.

@param is			The std::istream
@param polygons		Used to return the polygons to the caller
*/
template <typename Poly>
void FileUtil::load_polygons_section(std::istream& is, std::vector<shared_ptr<Poly> >& polygons)
{
	std::string line;
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read polygon count");
	try
	{
		int polyCount = boost::lexical_cast<int,std::string>(line);
		load_polygons(is, polygons, polyCount);
	}
	catch(boost::bad_lexical_cast&) { throw Exception("The polygon count is not an integer"); }
}

template <typename Vert, typename AuxData>
shared_ptr<PolyhedralBrush<Vert,AuxData> > FileUtil::load_polyhedral_brush(std::istream& is)
{
	typedef PolyhedralBrush<Vert,AuxData> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;

	std::string line;

	if(!std::getline(is,line)) return PolyBrush_Ptr();
	if(line != "{") throw Exception("Expected {");

	// Read bounds.
	if(!std::getline(is,line)) throw Exception("Unexpected EOF whilst trying to read bounds");
	AABB3d bounds = read_aabb<Vector3d>(line);

	// Read faces.
	typedef hesp::Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	std::vector<Poly_Ptr> faces;
	load_polygons_section(is, faces);

	if(!std::getline(is,line)) throw Exception("Unexpected EOF whilst trying to read }");
	if(line != "}") throw Exception("Expected }");

	return PolyBrush_Ptr(new PolyBrush(bounds, faces));
}

}
