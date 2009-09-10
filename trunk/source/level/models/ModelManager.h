/***
 * hesperus: ModelManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELMANAGER
#define H_HESP_MODELMANAGER

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Model> Model_Ptr;

class ModelManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,Model_Ptr> m_models;

	//#################### PUBLIC METHODS ####################
public:
	const Model_Ptr& model(const std::string& modelName);
	void load_all();
	void register_model(const std::string& modelName);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ModelManager> ModelManager_Ptr;
typedef shared_ptr<const ModelManager> ModelManager_CPtr;

}

#endif
