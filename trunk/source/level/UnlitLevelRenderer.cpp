/***
 * hesperus: UnlitLevelRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UnlitLevelRenderer.h"

#include <set>

#include <source/ogl/WrappedGL.h>

#include <source/colours/Colour3d.h>
#include <source/images/BitmapLoader.h>
#include <source/textures/TextureFactory.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
UnlitLevelRenderer::UnlitLevelRenderer(const std::vector<TexturedPolygon_Ptr>& polygons)
:	m_polygons(polygons)
{
	// Determine the set of unique texture names.
	std::set<std::string> textureNames;
	int polyCount = static_cast<int>(polygons.size());
	for(int i=0; i<polyCount; ++i)
	{
		textureNames.insert(polygons[i]->auxiliary_data());
	}

	// Load the textures.
	for(std::set<std::string>::const_iterator it=textureNames.begin(), iend=textureNames.end(); it!=iend; ++it)
	{
		std::string filename = "resources/textures/" + *it + ".bmp";
		Image24_Ptr image = BitmapLoader::load_image24(filename);
		m_textures.insert(std::make_pair(*it, TextureFactory::create_texture24(image)));
	}
}

//#################### PUBLIC METHODS ####################
void UnlitLevelRenderer::render(const std::vector<int>& polyIndices) const
{
	// TEMPORARY: This should be replaced with render_proper() once the proper version is ready.
	render_simple(polyIndices);
}

//#################### PRIVATE METHODS ####################
void UnlitLevelRenderer::render_simple(const std::vector<int>& polyIndices) const
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glEnable(GL_TEXTURE_2D);
	glColor3d(1,1,1);

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		TexturedPolygon_Ptr poly = m_polygons[polyIndices[i]];

		// Note:	If we got to this point, all textures were loaded successfully,
		//			so the texture's definitely in the map.
		std::map<std::string,Texture_Ptr>::const_iterator jt = m_textures.find(poly->auxiliary_data());
		jt->second->bind();

		int vertCount = poly->vertex_count();
		glBegin(GL_POLYGON);
			for(int j=0; j<vertCount; ++j)
			{
				const TexturedVector3d& v = poly->vertex(j);
				glTexCoord2d(v.u, v.v);
				glVertex3d(v.x, v.y, v.z);
			}
		glEnd();
	}

	glPopAttrib();
}

}
