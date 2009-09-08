/***
 * hesperus: ConfiguredBone.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONFIGUREDBONE
#define H_HESP_CONFIGUREDBONE

#include <source/math/matrices/RBTMatrix.h>

namespace hesp {

class ConfiguredBone
{
	//#################### PRIVATE VARIABLES ####################
private:
	RBTMatrix_CPtr m_absoluteMatrix;

	//#################### CONSTRUCTORS ####################
public:
	explicit ConfiguredBone(const RBTMatrix_CPtr& absoluteMatrix);

	//#################### PUBLIC METHODS ####################
public:
	const RBTMatrix_CPtr& absolute_matrix() const;
	Vector3d orientation() const;
	Vector3d position() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ConfiguredBone> ConfiguredBone_Ptr;
typedef shared_ptr<const ConfiguredBone> ConfiguredBone_CPtr;

}

#endif
