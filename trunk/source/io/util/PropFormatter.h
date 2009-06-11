/***
 * hesperus: PropFormatter.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PROPFORMATTER
#define H_HESP_PROPFORMATTER

#include <map>
#include <string>
#include <vector>

namespace hesp {

//#################### MAIN TEMPLATE ####################
template <typename T> struct PropFormatter
{
	static std::string format(const T& input);
};

//#################### SPECIALIZATIONS ####################
template <> struct PropFormatter<std::string>
{
	static std::string format(const std::string& input);
};

template <typename K, typename V> struct PropFormatter<std::map<K,V> >
{
	static std::string format(const std::map<K,V>& input);
};

template <typename T> struct PropFormatter<std::vector<T> >
{
	static std::string format(const std::vector<T>& input);
};

//#################### HELPER FUNCTIONS ####################
template <typename T> std::string format_property(const T& input);

}

#include "PropFormatter.tpp"

#endif
