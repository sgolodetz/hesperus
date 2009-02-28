/***
 * hesperus: BoneConfiguration.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BoneConfiguration.h"

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
Bone_Ptr BoneConfiguration::bones(int i)
{
	return m_bones[i];
}

Bone_CPtr BoneConfiguration::bones(int i) const
{
	return m_bones[i];
}

Bone_Ptr BoneConfiguration::bones(const std::string& name)
{
	return find_bone(name);
}

Bone_CPtr BoneConfiguration::bones(const std::string& name) const
{
	return find_bone(name);
}

//#################### PRIVATE METHODS ####################
Bone_Ptr BoneConfiguration::find_bone(const std::string& name) const
{
	std::map<std::string,int>::const_iterator it = m_boneLookup.find(name);
	if(it != m_boneLookup.end()) return m_bones[it->second];
	else throw Exception("Bone " + name + " does not exist");
}

}
