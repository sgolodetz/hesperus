/***
 * hlight: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <source/level/lighting/LightmapGenerator.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

int main(int argc, char *argv[])
{
#if 0
	std::vector<TexturedVector3d> vertices;
	vertices.push_back(TexturedVector3d(2,3,5,0,0));
	vertices.push_back(TexturedVector3d(2,1,5,0,1));
	vertices.push_back(TexturedVector3d(10,3,6,1,0));
	TexturedPolygon poly(vertices, "TEST");
	std::vector<TexCoords> vertexLightmapCoords;
	LightmapGrid grid(poly, vertexLightmapCoords);
#else
	std::vector<TexturedPolygon_Ptr> polys;
	{
		// Projected onto x-y plane.
		std::vector<TexturedVector3d> vertices;
		vertices.push_back(TexturedVector3d(2,3,5,0,0));
		vertices.push_back(TexturedVector3d(2,1,5,0,1));
		vertices.push_back(TexturedVector3d(10,3,6,1,0));
		polys.push_back(TexturedPolygon_Ptr(new TexturedPolygon(vertices, "TEST")));
	}
	{
		// Projected onto x-z plane.
		std::vector<TexturedVector3d> vertices;
		vertices.push_back(TexturedVector3d(1,5,1,0,1));
		vertices.push_back(TexturedVector3d(3,5,10,1,0));
		vertices.push_back(TexturedVector3d(3,6,1,1,1));
		polys.push_back(TexturedPolygon_Ptr(new TexturedPolygon(vertices, "TEST")));
	}
	{
		// Projected onto y-z plane.
		std::vector<TexturedVector3d> vertices;
		vertices.push_back(TexturedVector3d(-2,0,0,0,0));
		vertices.push_back(TexturedVector3d(-2,10,-10,1,1));
		vertices.push_back(TexturedVector3d(-3,0,-10,0,1));
		polys.push_back(TexturedPolygon_Ptr(new TexturedPolygon(vertices, "TEST")));
	}
	LightmapGenerator lg(polys, std::vector<Light>(), BSPTree_Ptr(), LeafVisTable_Ptr());
	lg.generate_lightmaps();
#endif

	return 0;
}
