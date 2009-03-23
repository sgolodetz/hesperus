/***
 * hesperus: ModelManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELMANAGER
#define H_HESP_MODELMANAGER

#include <map>
#include <string>

#include "Model.h"

namespace hesp {

class ModelManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,Model_Ptr> m_models;

	//#################### PUBLIC METHODS ####################
public:
	const Model_Ptr& model(const std::string& modelName) const;
	void load_all();
	void register_model(const std::string& modelName);
};

}

#endif
