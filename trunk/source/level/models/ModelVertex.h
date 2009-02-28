/***
 * hesperus: ModelVertex.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELVERTEX
#define H_HESP_MODELVERTEX

#include <vector>

#include <source/math/vectors/Vector3.h>
#include "BoneWeight.h"

namespace hesp {

class ModelVertex
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position;
	Vector3d m_normal;
	std::vector<BoneWeight> m_boneWeights;

	// TODO
};

}

#endif
