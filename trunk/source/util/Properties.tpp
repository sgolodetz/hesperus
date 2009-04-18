/***
 * hesperus: Properties.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename T>
shared_ptr<T> Properties::get(const std::string& name) const
{
	if(!has(name)) return shared_ptr<T>();

	try
	{
		return boost::any_cast<shared_ptr<T> >(m_properties.find(name)->second);
	}
	catch(boost::bad_any_cast&)
	{
		throw Exception("The property " + name + " does not have the specified type");
	}
}

template <typename T>
void Properties::set(const std::string& name, const T& value)
{
	m_properties[name] = shared_ptr<T>(new T(value));
}

template <typename T>
void Properties::set(const std::string& name, const shared_ptr<T>& value)
{
	m_properties[name] = value;
}

}
