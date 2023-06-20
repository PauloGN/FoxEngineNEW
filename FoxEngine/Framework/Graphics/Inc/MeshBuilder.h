#pragma once
#include "MeshTypes.h"

namespace FoxEngine::Graphics
{

	class MeshBuilder
	{

	public:

		//Cubes
		static MeshPC CreateCubePC(float size, const Color& color);
		static MeshPX CreateCubePX(float size);
		static Mesh CreateCube(float size);
		//Rectangles
		static MeshPC CreateRectPC(float width, float height, float depth, const Color& color);	
		static MeshPX CreateRectPX(float width, float height, float depth);
		//Plane
		static MeshPC CreatePlanePC(float numRows, int numColums, float spacing);
		static MeshPX CreatePlanePX(uint32_t numRows, uint32_t numColums, float spacing);//**********
		//CylinderPC
		static MeshPC CreateCylinderPC(int slices, int rings);
		//Sphere
		static MeshPC CreateSpherePC(int slices, int rings, float radius);
		static MeshPX CreateSpherePX(int slices, int rings, float radius);
		static Mesh CreateSphere(int slices, int rings, float radius);
		//Sky
		static MeshPX CreateSkyboxPX(float size);
		static MeshPX CreateSkySpherePX(int slices, int rings, float radius);

	};
}