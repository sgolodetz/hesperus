/***
 * hesperus: EntityManager.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

//#################### PRIVATE METHODS ####################
template <typename T>
T EntityManager::read_typed_field(std::istream& is, const std::string& expectedFieldName)
{
	std::string fieldString = read_field(is, expectedFieldName);
	try
	{
		T field = lexical_cast<T,std::string>(fieldString);
		return field;
	}
	catch(bad_lexical_cast&)
	{
		throw Exception("The value for " + expectedFieldName + " was not of the right type");
	}
}

}
