#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "Texture.h"

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
