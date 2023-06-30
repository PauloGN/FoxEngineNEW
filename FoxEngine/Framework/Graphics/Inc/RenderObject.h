#pragma once
#include "MeshBuffer.h"
#include "Texture.h"
#include "Transform.h"

namespace FoxEngine::Graphics
{
	class RenderObject
	{
	public:

		void Terminate();

		Transform transform;
		MeshBuffer meshBuffer;
		Texture mDiffuseTexture;
	};
}
