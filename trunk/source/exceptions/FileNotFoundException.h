/***
 * hesperus: FileNotFoundException.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Exception.h"

namespace hesp {

class FileNotFoundException : public Exception
{
	//#################### CONSTRUCTORS ####################
public:
	explicit FileNotFoundException(const std::string& filename, const std::string& message = "")
	:	Exception("File Not Found: " + filename + ". " + message)
	{}
};

}
