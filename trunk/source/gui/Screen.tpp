/***
 * hesperus: Screen.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/gl.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename Layout>
void Screen<Layout>::render() const
{
	const int& x1 = m_extents->left(), y1 = m_extents->top(), x2 = m_extents->right(), y2 = m_extents->bottom();
	glViewport(x1, y1, x2-x1, y2-y1);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Container::render();

	SDL_GL_SwapBuffers();
}

}
