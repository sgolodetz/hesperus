/***
 * hesperus: ModelFilesUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELFILESUTIL
#define H_HESP_MODELFILESUTIL

#include <source/level/models/Model.h>
#include <source/xml/XMLParser.h>

namespace hesp {

class ModelFilesUtil
{
	//#################### LOADING METHODS ####################
public:
	static Model_Ptr load_model(const std::string& name);

	//#################### LOADING SUPPORT METHODS ####################
#if 0	// TEMPORARY: To enable easy development testing
private:
#endif
	static TexCoords extract_texcoords(const XMLElement_CPtr& elt);
	static Vector3d extract_vector3d(const XMLElement_CPtr& elt);
	static Mesh_Ptr load_mesh(const std::string& filename);
	static Skeleton_Ptr load_skeleton(const std::string& filename);
};

}

#endif
