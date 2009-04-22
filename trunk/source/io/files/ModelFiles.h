/***
 * hesperus: ModelFiles.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELFILES
#define H_HESP_MODELFILES

#include <map>
#include <string>

#include <source/level/models/Model.h>
#include <source/xml/XMLParser.h>

namespace hesp {

class ModelFiles
{
	//#################### TYPEDEFS ####################
private:
	typedef std::pair<std::string,Material_Ptr> NamedMaterial;
	typedef shared_ptr<NamedMaterial> NamedMaterial_Ptr;

	//#################### LOADING METHODS ####################
public:
	static std::map<std::string,Material_Ptr> load_materials(const std::string& filename);
	static Mesh_Ptr load_mesh(const std::string& filename, const std::map<std::string,Material_Ptr>& materials);
	static Model_Ptr load_model(const std::string& name);
	static Skeleton_Ptr load_skeleton(const std::string& filename);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static TexCoords extract_texcoords(const XMLElement_CPtr& elt);
	static Vector3d extract_vector3d(const XMLElement_CPtr& elt);
	static NamedMaterial_Ptr read_material(std::istream& is);
	static Material_Ptr read_pass(std::istream& is);
	static Material_Ptr read_technique(std::istream& is);
};

}

#endif
