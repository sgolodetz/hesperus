/***
 * hesperus: InputBinding.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "InputBinding.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
InputBinding::InputBinding(const std::map<Action,Inputter_CPtr>& inputters)
:	m_inputters(inputters)
{}

//#################### PUBLIC OPERATORS ####################
Inputter_CPtr InputBinding::operator()(Action action) const
{
	std::map<Action,Inputter_CPtr>::const_iterator it = m_inputters.find(action);
	if(it != m_inputters.end()) return it->second;
	else return Inputter_CPtr();
}

//#################### PUBLIC METHODS ####################
bool InputBinding::down(Action action, const InputState& input) const
{
	Inputter_CPtr inputter = (*this)(action);
	return inputter && inputter->down(input);
}

}
