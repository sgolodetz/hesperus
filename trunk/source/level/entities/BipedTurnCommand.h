/***
 * hesperus: BipedTurnCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BIPEDTURNCOMMAND
#define H_HESP_BIPEDTURNCOMMAND

#include "Entity.h"
#include "EntityCommand.h"

namespace hesp {

class BipedTurnCommand : public EntityCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	Entity_Ptr m_biped;
	int m_mouseMotionX, m_mouseMotionY;

	//#################### CONSTRUCTORS ####################
public:
	BipedTurnCommand(const Entity_Ptr& biped, int mouseMotionX, int mouseMotionY);

	//#################### PUBLIC METHODS ####################
public:
	void execute(int milliseconds);
};

}

#endif
