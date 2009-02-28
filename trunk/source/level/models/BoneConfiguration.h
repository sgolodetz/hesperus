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

	// TODO
};

}

#endif
