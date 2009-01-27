/***
 * hesperus: BipedWalkCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BIPEDWALKCOMMAND
#define H_HESP_BIPEDWALKCOMMAND

#include <source/math/vectors/Vector3.h>
#include "BipedEntity.h"

namespace hesp {

class BipedWalkCommand : public EntityCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	BipedEntity *m_biped;
	Vector3d m_dir;

	//#################### CONSTRUCTORS ####################
public:
	BipedWalkCommand(BipedEntity *biped, const Vector3d& dir);

	//#################### PUBLIC METHODS ####################
public:
	void execute(int milliseconds);
};

}

#endif
