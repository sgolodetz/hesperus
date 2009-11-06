/***
 * hesperus: ResourceManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_RESOURCEMANAGER
#define H_HESP_RESOURCEMANAGER

#include <map>
#include <set>
#include <string>

namespace hesp {

template <typename Resource>
class ResourceManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,Resource> m_resources;

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual Resource load_resource(const std::string& resourceName) const = 0;
	virtual std::string resource_type() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	void load_all();
	void register_resource(const std::string& resourceName);
	const Resource& resource(const std::string& resourceName);
	std::set<std::string> resource_names() const;
};

}

#include "ResourceManager.tpp"

#endif
