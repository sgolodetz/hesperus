/***
 * hesperus: Container.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#define Container_HEADER	template <typename Layout>
#define Container_THIS		Container<Layout>

namespace hesp {

//#################### PUBLIC METHODS ####################
Container_HEADER
void Container_THIS::fit(const Extents& extents)
{
	Component::fit(extents);

	m_components = m_layout.fit(extents);
	for(std::vector<LaidOutComponent>::iterator it=m_components.begin(), iend=m_components.end(); it!=iend; ++it)
	{
		it->component->fit(it->extents);
	}
}

Container_HEADER
Layout& Container_THIS::layout()
{
	return m_layout;
}

Container_HEADER
void Container_THIS::render() const
{
	for(std::vector<LaidOutComponent>::const_iterator it=m_components.begin(), iend=m_components.end(); it!=iend; ++it)
	{
		it->component->render();
	}
}

}

#undef Container_THIS
#undef Container_HEADER
