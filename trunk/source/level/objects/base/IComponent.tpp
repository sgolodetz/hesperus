/***
 * hesperus: IComponent.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/exceptions/Exception.h>

namespace hesp {

template <typename C>
void IComponent::check_dependency() const
{
	if(!m_objectManager->get_component<C>(m_objectID))
	{
		throw Exception(own_type() + " (" + m_objectID.to_string() + "): Requires " + C::static_own_type() + " component");
	}
}

}
