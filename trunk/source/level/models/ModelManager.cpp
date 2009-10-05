/***
 * hesperus: ModelManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelManager.h"

#include <source/exceptions/Exception.h>
#include <source/io/files/ModelFiles.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads all the registered models.
*/
void ModelManager::load_all()
{
	for(std::map<std::string,Model_Ptr>::iterator it=m_models.begin(), iend=m_models.end(); it!=iend; ++it)
	{
		const std::string& name = it->first;
		Model_Ptr& model = it->second;

		if(!model)
		{
			model = ModelFiles::load_model(name);
		}
	}
}

/**
Returns the model with the specified name, if any.

@param modelName	The name of the model
@return				The model, if it exists
@throw Exception	If the model doesn't exist
*/
const Model_Ptr& ModelManager::model(const std::string& modelName)
{
	std::map<std::string,Model_Ptr>::const_iterator it = m_models.find(modelName);
	if(it != m_models.end()) return it->second;
	else throw Exception("This model manager does not contain a model named " + modelName);
}

/**
Returns the names of the registered models.

@return	The registered model names
*/
std::set<std::string> ModelManager::model_names() const
{
	std::set<std::string> ret;
	for(std::map<std::string,Model_Ptr>::const_iterator it=m_models.begin(), iend=m_models.end(); it!=iend; ++it)
	{
		ret.insert(it->first);
	}
	return ret;
}

/**
Registers a model for subsequent loading.

@param modelName	The name of the model
*/
void ModelManager::register_model(const std::string& modelName)
{
	m_models.insert(std::make_pair(modelName, Model_Ptr()));
}

}
