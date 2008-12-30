/***
 * hesperus: Lightmap.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LIGHTING_LIGHTMAP
#define H_HESP_LEVEL_LIGHTING_LIGHTMAP

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/colours/Colour3d.h>

namespace hesp {

class Lightmap
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::vector<Colour3d> > m_lumels;
	int m_rows, m_cols;

	//#################### CONSTRUCTORS ####################
public:
	Lightmap(int rows, int cols, const Colour3d& initial = Colour3d());

	//#################### PUBLIC OPERATORS ####################
public:
	Colour3d& operator()(int row, int col);
	const Colour3d& operator()(int row, int col) const;
	Lightmap& operator+=(const Lightmap& rhs);

	//#################### PUBLIC METHODS ####################
public:
	// TODO
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Lightmap> Lightmap_Ptr;
typedef shared_ptr<const Lightmap> Lightmap_CPtr;

}

#endif
