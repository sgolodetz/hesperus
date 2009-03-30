/***
 * asx: ASXModule.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXModule.h"

//#################### CONSTRUCTORS ####################
ASXModule::ASXModule(asIScriptModule *module)
:	m_module(module)
{}

//#################### PUBLIC METHODS ####################
bool ASXModule::build()
{
	int result = m_module->Build();
	if(result < 0) return false;
	else return true;
}
