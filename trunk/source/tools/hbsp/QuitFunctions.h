/***
 * hbsp: QuitFunctions.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HBSP_QUITFUNCTIONS
#define H_HBSP_QUITFUNCTIONS

#include <iostream>
#include <string>

namespace hesp {

void quit_with_error(const std::string& error);
void quit_with_usage();

}

#endif
