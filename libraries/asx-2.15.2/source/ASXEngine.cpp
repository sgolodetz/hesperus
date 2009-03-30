/***
 * asx: ASXEngine.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXEngine.h"

#include <fstream>
#include <vector>

#include <scriptstring.h>

#include "ASXException.h"

//#################### CONSTRUCTORS ####################
ASXEngine::ASXEngine()
{
	m_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

	if(!m_engine)
	{
		throw ASXException("Could not create scripting engine");
	}

	m_engine->SetMessageCallback(asMETHOD(ASXEngine, message_callback), this, asCALL_THISCALL);

	RegisterScriptString(m_engine);
}

//#################### DESTRUCTOR ####################
ASXEngine::~ASXEngine()
{
	m_engine->Release();
}

//#################### PUBLIC METHODS ####################
void ASXEngine::clear_messages()
{
	std::swap(m_messages, std::vector<ASXMessage>());
}

ASXModule_Ptr ASXEngine::get_module(const std::string& moduleName) const
{
	asIScriptModule *module = m_engine->GetModule(moduleName.c_str(), asGM_ONLY_IF_EXISTS);
	if(module) return ASXModule_Ptr(new ASXModule(module));
	else throw ASXException("Unknown module: " + moduleName);
}

void ASXEngine::load_script(const std::string& filename, const std::string& moduleName)
{
	// Load in the file.
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw ASXException("Could not open " + filename + " for reading");

	is.seekg(0, std::ios::end);
	int len = is.tellg();
	if(len == 0) throw ASXException("Could not add script section from empty file " + filename);

	std::vector<char> buffer(len);
	is.seekg(0, std::ios::beg);
	is.read(&buffer[0], len);

	// Add the loaded script to the module.
	asIScriptModule *module = m_engine->GetModule(moduleName.c_str(), asGM_CREATE_IF_NOT_EXISTS);
	if(module->AddScriptSection(filename.c_str(), &buffer[0], len) < 0)
	{
		throw ASXException("Could not add script section from file " + filename);
	}
}

const std::vector<ASXMessage>& ASXEngine::messages() const
{
	return m_messages;
}

void ASXEngine::output_messages(std::ostream& os) const
{
	for(size_t i=0, size=m_messages.size(); i<size; ++i)
	{
		m_messages[i].output(os);
	}
}

//#################### PRIVATE METHODS ####################
void ASXEngine::message_callback(const asSMessageInfo *msg)
{
	std::string type;
	switch(msg->type)
	{
		case asMSGTYPE_INFORMATION:
		{
			type = "INFO";
			break;
		}
		case asMSGTYPE_WARNING:
		{
			type = "WARN";
			break;
		}
		default:
		{
			type = "ERR";
			break;
		}
	}
	m_messages.push_back(ASXMessage(type, msg->section, msg->row, msg->col, msg->message));
}
