/***
 * hesperus: UnlitLevelRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UnlitLevelRenderer.h"

#include <source/colours/Colour3d.h>
#include <source/ogl/WrappedGL.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
UnlitLevelRenderer::UnlitLevelRenderer(const std::vector<TexturedPolygon_Ptr>& polygons)
:	m_polygons(polygons)
{}

//#################### PUBLIC METHODS ####################
void UnlitLevelRenderer::render(const std::vector<int>& polyIndices) const
{
	// TEMPORARY: This should be replaced with render_proper() once the proper version is ready.
	render_simple(polyIndices);
}

//#################### PRIVATE METHODS ####################
void UnlitLevelRenderer::render_simple(const std::vector<int>& polyIndices) const
{
	glPushAttrib(GL_POLYGON_BIT);

	Colour3d colours[] = {Colour3d(1,0,0), Colour3d(0,1,0), Colour3d(0,0,1)};

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		TexturedPolygon_Ptr poly = m_polygons[polyIndices[i]];
		int vertCount = poly->vertex_count();
		glBegin(GL_POLYGON);
			for(int j=0; j<vertCount; ++j)
			{
				int k = (i+j)%3;
				glColor3d(colours[k].r, colours[k].g, colours[k].b);

				const TexturedVector3d& v = poly->vertex(j);
				glVertex3d(v.x, v.y, v.z);
			}
		glEnd();
	}

	glPopAttrib();
}

}
