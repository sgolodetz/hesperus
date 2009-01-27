/***
 * hesperus: Guard.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GUARD
#define H_HESP_GUARD

#include "BipedEntity.h"
#include "MortalEntity.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Guard> Guard_Ptr;
typedef shared_ptr<const class Guard> Guard_CPtr;

/**
This class represents a guard (a basic enemy for testing purposes).
*/
class Guard
:	public BipedEntity,
	public MortalEntity
{
	//#################### CONSTRUCTORS ####################
public:
	Guard(const std::vector<int>& aabbIndices,
		  const std::string& modelFilename,
		  int health,
		  const Vector3d& look,
		  int pose,
		  const Vector3d& position);

	//#################### PUBLIC METHODS ####################
public:
	Guard *as_guard();
	static Guard_Ptr load(std::istream& is);
	void save(std::ostream& os) const;
};

}

#endif
