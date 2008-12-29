/***
 * hlight: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <source/level/lighting/LightmapGenerator.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

int main(int argc, char *argv[])
{
	std::vector<TexturedVector3d> vertices;
	vertices.push_back(TexturedVector3d(2,3,5,0,0));
	vertices.push_back(TexturedVector3d(2,1,5,0,1));
	vertices.push_back(TexturedVector3d(10,3,6,1,0));
	TexturedPolygon poly(vertices, "TEST");
	std::vector<TexCoords> vertexLightmapCoords;
	LightmapGrid grid(poly, vertexLightmapCoords);
	return 0;
}
