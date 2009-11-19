/***
 * hesperus: InputBinding.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INPUTBINDING
#define H_HESP_INPUTBINDING

#include <map>

#include "Inputter.h"

namespace hesp {

class InputBinding
{
	//#################### ENUMERATIONS ####################
public:
	enum Action
	{
		ACT_CROUCH,
		ACT_JUMP,
		ACT_MOVE_BACKWARD,
		ACT_MOVE_FORWARD,
		ACT_STRAFE_LEFT,
		ACT_STRAFE_RIGHT,
		ACT_USE_ITEM,
		ACT_WALK,
	};

	//#################### PRIVATE VARIABLES ####################
private:
	std::map<Action,Inputter_CPtr> m_inputters;

	//#################### CONSTRUCTORS ####################
public:
	explicit InputBinding(const std::map<Action,Inputter_CPtr>& inputters);

	//#################### PUBLIC OPERATORS ####################
public:
	Inputter_CPtr operator()(Action action) const;

	//#################### PUBLIC METHODS ####################
public:
	bool down(Action action, const InputState& input) const;
};

}

#endif
