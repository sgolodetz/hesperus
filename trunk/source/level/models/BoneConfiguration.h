/***
 * hesperus: BoneConfiguration.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BONECONFIGURATION
#define H_HESP_BONECONFIGURATION

#include <map>
#include <vector>

#include "Bone.h"

namespace hesp {

class BoneConfiguration
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Bone_Ptr> m_bones;
	std::map<std::string,int> m_boneLookup;

	//#################### CONSTRUCTORS ####################
public:
	BoneConfiguration(const std::vector<Bone_Ptr>& bones);

	//#################### PUBLIC METHODS ####################
public:
	Bone_Ptr bones(int i);
	Bone_CPtr bones(int i) const;
	Bone_Ptr bones(const std::string& name);
	Bone_CPtr bones(const std::string& name) const;

	//#################### PRIVATE METHODS ####################
private:
	Bone_Ptr find_bone(const std::string& name) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<BoneConfiguration> BoneConfiguration_Ptr;
typedef shared_ptr<const BoneConfiguration> BoneConfiguration_CPtr;

}

#endif
