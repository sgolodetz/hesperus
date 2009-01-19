/***
 * hesperus: NavFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### SAVING METHODS ####################
void NavFileUtil::save(const std::string& filename, const std::vector<NavDataset_Ptr>& datasets)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << "Nav\n";
	os << "{\n";

	// Output the datasets sequentially.
	int datasetCount = static_cast<int>(datasets.size());
	for(int i=0; i<datasetCount; ++i)
	{
		// TODO: Replace this with code to save the entire dataset.
		FileSectionUtil::save_navmesh_section(os, datasets[i]->nav_mesh());
	}

	os << "}\n";
}

}
