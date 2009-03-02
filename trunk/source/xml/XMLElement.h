/***
 * hesperus: XMLElement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLELEMENT
#define H_HESP_XMLELEMENT

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class XMLElement> XMLElement_Ptr;
typedef shared_ptr<const class XMLElement> XMLElement_CPtr;

class XMLElement
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name;
	std::map<std::string,std::string> m_attributes;
	std::multimap<std::string,XMLElement_Ptr> m_children;

	//#################### CONSTRUCTORS ####################
public:
	XMLElement(const std::string& name);

	//#################### PUBLIC METHODS ####################
public:
	void add_child(const XMLElement_Ptr& child);
	const std::string& name() const;
	void set_attribute(const std::string& name, const std::string& value);
};

}

#endif
