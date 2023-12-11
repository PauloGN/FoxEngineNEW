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

void FoxEngine::Graphics::RotateAlongCamera(RenderGroup& renderGroup, bool updateCharacter, FoxMath::Vector3& cameraForward)
{
	if (updateCharacter)
	{
		// Set character rotation based on camera forward vector
		FoxMath::Vector3 lookAt = FoxMath::Normalize(FoxMath::Vector3(cameraForward.x, 0.0f, cameraForward.z));
		float yaw = atan2(lookAt.x, lookAt.z);
		FoxMath::Quaternion rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, -yaw, 0.0f);

		for (auto& a : renderGroup)
		{
			a.transform.rotation = rotation;
		}
	}
}

void FoxEngine::Graphics::MoveAlongCamera(RenderGroup& renderGroup, bool updateCharacter, FoxMath::Vector3& target, float deltaTime, float speed)
{
	// Linear interpolation for smooth movement
	if (updateCharacter)
	{
		FoxMath::Vector3 Offset{ 0.0f, 0.0f, 2.0f };
		FoxMath::Vector3 newTarget = { target.x, 0.0f, target.z };

		for (auto& a : renderGroup)
		{
			a.transform.position = FoxMath::Lerp(a.transform.position, (newTarget + Offset), speed * deltaTime);
		}
	}
}
