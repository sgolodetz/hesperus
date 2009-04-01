/***
 * asx: ASXContext.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXContext.h"

struct ASXContextReleaser
{
	void operator()(asIScriptContext *context)
	{
		context->Release();
	}
};

//#################### CONSTRUCTORS ####################
ASXContext::ASXContext(asIScriptContext *context, int funcID)
:	m_context(context, ASXContextReleaser()), m_funcID(funcID)
{}

//#################### PUBLIC OPERATORS ####################
asIScriptContext *ASXContext::operator->() const
{
	return m_context.get();
}

//#################### PUBLIC METHODS ####################
int ASXContext::execute()
{
	return m_context->Execute();
}

int ASXContext::prepare()
{
	return m_context->Prepare(m_funcID);
}
