#pragma once
#include "Animator.h"
#include "Material.h"
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Skeleton.h"

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

		ModelId modelId = 0;

		MeshBuffer meshBuffer;

		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]]RenderGroup CreateRenderGroup(ModelId modelId, const Animator* animator = nullptr);
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, const Animator* animator = nullptr);
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
