#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void FoxEngine::Graphics::RenderObject::Terminate()
{
	diffuseMapId = 0;
	normalMapId = 0;
	bumpMapId = 0;
	specMapId = 0;
	modelId = 0;
	meshBuffer.Terminate();
}

RenderGroup FoxEngine::Graphics::CreateRenderGroup(ModelId modelId, const Animator* animator)
{
	auto model = ModelManager::Get()->GetModel(modelId);
	ASSERT(model != nullptr, "RenderGroup: ModelId %d is not loaded", modelId);
	RenderGroup renderGroup = CreateRenderGroup(*model, animator);

	for (auto& renderObject : renderGroup)
	{
		renderObject.modelId = modelId;
	}
	return renderGroup;
}

RenderGroup FoxEngine::Graphics::CreateRenderGroup(const Model& model, const Animator* animator)
{
	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());

	auto TryLoadTexture = [](const auto& textureName) -> TextureId
	{
		if (textureName.empty())
		{
			return 0;
		}

		return TextureManager::Get()->LoadTexture(textureName, false);
	};

	for(auto& meshData : model.meshData)
	{
		auto& renderObject = renderGroup.emplace_back();
		if (meshData.materialIndex < model.materialData.size())
		{
			auto& materialData = model.materialData[meshData.materialIndex];
			renderObject.material = materialData.material;
			renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseMapName);
			renderObject.specMapId = TryLoadTexture(materialData.specularMapName);
			renderObject.bumpMapId = TryLoadTexture(materialData.bumpMapName);
			renderObject.normalMapId = TryLoadTexture(materialData.normalMapName);
		}

		renderObject.meshBuffer.Initialize(meshData.mesh);
		renderObject.skeleton = model.skeleton.get();
		renderObject.animator = animator;
	}
	return renderGroup;
}

void FoxEngine::Graphics::CleanupRenderGroup(RenderGroup& renderGroup)
{

	for (auto& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}
