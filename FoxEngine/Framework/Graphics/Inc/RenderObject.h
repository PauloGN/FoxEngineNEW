#pragma once
#include "Material.h"
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureManager.h"

namespace FoxEngine::Graphics
{
	class RenderObject
	{
	public:

		void Terminate()
		{
			meshBuffer.Terminate();
		}

		Transform transform;
		Material material;
		TextureId diffuseMapId;
		TextureId normalMap;
		MeshBuffer meshBuffer;
	};
}
