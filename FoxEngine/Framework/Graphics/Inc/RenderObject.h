#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureManager.h"

namespace FoxEngine::Graphics
{
	class RenderObject
	{
	public:

		void Terminate();

		Transform transform;
		MeshBuffer meshBuffer;
		TextureId diffuseMapId;
	};
}
