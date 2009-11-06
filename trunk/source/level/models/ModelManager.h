/***
 * hesperus: ModelManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELMANAGER
#define H_HESP_MODELMANAGER

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/util/ResourceManager.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Model> Model_Ptr;

class ModelManager : private ResourceManager<Model_Ptr>
{
	//#################### PUBLIC METHODS ####################
public:
	using ResourceManager<Model_Ptr>::load_all;
	const Model_Ptr& model(const std::string& modelName);
	std::set<std::string> model_names() const;
	void register_model(const std::string& modelName);

	//#################### PRIVATE METHODS ####################
private:
	Model_Ptr load_resource(const std::string& modelName) const;
	std::string resource_type() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ModelManager> ModelManager_Ptr;
typedef shared_ptr<const ModelManager> ModelManager_CPtr;

}

#endif
