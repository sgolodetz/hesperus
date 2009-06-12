/***
 * hesperus: ObjectsSection.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/io/util/PropFormatter.h>
#include <source/io/util/PropReader.h>

namespace hesp {

//#################### LOADING SUPPORT METHODS ####################
template <typename T>
void ObjectsSection::load_property(Properties& properties, const std::string& name, const std::string& value)
{
	properties.set(name, read_property<T>(value));
}

//#################### SAVING SUPPORT METHODS ####################
template <typename T>
void ObjectsSection::save_property(std::ostream& os, const Properties& properties, const std::string& name)
{
	FieldIO::write_typed_field(os, name, format_property(properties.get<T>(name)));
}

}
