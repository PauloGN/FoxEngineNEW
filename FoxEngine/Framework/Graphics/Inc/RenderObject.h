#pragma once
#include "Material.h"
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureManager.h"

namespace FoxEngine::Graphics
{
	struct Model;

	class RenderObject
	{
	public:

		void Terminate();

		Transform transform;
		Material material;

		TextureId diffuseMapId;
		TextureId normalMapId;
		TextureId bumpMapId;
		TextureId specMapId;

		MeshBuffer meshBuffer;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]]RenderGroup CreateRenderGroup(const Model& model);
	void CleanupRenderGroup(RenderGroup& renderGroup);

	template<class Effect>
	void DrawrenderGroup(Effect& effect,  const RenderGroup& renderGroup) 
	{
		for (auto& renderObject : renderGroup)
		{
			effect.Render(renderObject);
		}
	}
}
