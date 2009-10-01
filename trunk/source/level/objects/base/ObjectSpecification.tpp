/***
 * hesperus: ObjectSpecification.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename T>
void ObjectSpecification::set_component_property(const std::string& componentName, const std::string& propertyName, const T& value)
{
	ComponentMap::iterator it = m_components.find(componentName);
	if(it != m_components.end())
	{
		Properties& properties = it->second;
		properties.set(propertyName, value);
	}
	else throw Exception("The object specification doesn't contain a component named: " + componentName);
}

}
