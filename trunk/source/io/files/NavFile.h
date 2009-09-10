/***
 * hesperus: NavFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVFILE
#define H_HESP_NAVFILE

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NavDataset> NavDataset_Ptr;

struct NavFile
{
	//#################### LOADING METHODS ####################
	static std::vector<NavDataset_Ptr> load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const std::vector<NavDataset_Ptr>& datasets);
};

}

#endif
