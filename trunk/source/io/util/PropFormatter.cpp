/***
 * hesperus: PropFormatter.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PropFormatter.h"

namespace hesp {

//#################### SPECIALIZATIONS ####################
std::string PropFormatter<std::string>::format(const std::string& input)
{
	return "\"" + input + "\"";
}

}