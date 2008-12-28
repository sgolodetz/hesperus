/***
 * hesperus: Lightmap.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LIGHTING_LIGHTMAP
#define H_HESP_LEVEL_LIGHTING_LIGHTMAP

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class Lightmap
{
	// TODO
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Lightmap> Lightmap_Ptr;
typedef shared_ptr<const Lightmap> Lightmap_CPtr;

}

#endif
