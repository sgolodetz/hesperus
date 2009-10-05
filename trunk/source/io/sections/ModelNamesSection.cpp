/***
 * hesperus: ModelNamesSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelNamesSection.h"

#include <source/io/util/LineIO.h>
#include <source/level/models/ModelManager.h>

namespace hesp {

//#################### LOADING METHODS ####################
ModelManager_Ptr ModelNamesSection::load(std::istream& is)
{
	ModelManager_Ptr modelManager(new ModelManager);

	LineIO::read_checked_line(is, "ModelNames");
	LineIO::read_checked_line(is, "{");

	std::string line;
	for(;;)
	{
		LineIO::read_trimmed_line(is, line, "model name");
		if(line == "}") break;
		if(line.length() == 0) continue;

		modelManager->register_model(line);
	}

	return modelManager;
}

//#################### SAVING METHODS ####################
void ModelNamesSection::save(std::ostream& os, const ModelManager_CPtr& modelManager)
{
	os << "ModelNames\n";
	os << "{\n";

	std::set<std::string> modelNames = modelManager->model_names();
	for(std::set<std::string>::const_iterator it=modelNames.begin(), iend=modelNames.end(); it!=iend; ++it)
	{
		os << '\t' << *it << '\n';
	}

	os << "}\n";
}

}
