/***
 * hesperus: ModelNamesSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELNAMESSECTION
#define H_HESP_MODELNAMESSECTION

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class ModelManager> ModelManager_Ptr;
typedef shared_ptr<const class ModelManager> ModelManager_CPtr;

struct ModelNamesSection
{
	//#################### LOADING METHODS ####################
	static ModelManager_Ptr load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const ModelManager_CPtr& modelManager);
};

}

#endif
