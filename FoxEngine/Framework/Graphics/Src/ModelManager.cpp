#include "Precompiled.h"
#include "ModelManager.h"
#include "ModelIO.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sModelManager;
}

void FoxEngine::Graphics::ModelManager::StaticInitialize()
{
	ASSERT(sModelManager == nullptr, "ModelManager: is already initialized");
	sModelManager = std::make_unique<ModelManager>();
}

void FoxEngine::Graphics::ModelManager::StaticTerminate()
{
	sModelManager.reset();
}

ModelManager* FoxEngine::Graphics::ModelManager::Get()
{
	ASSERT(sModelManager != nullptr, "ModelManager needs to be initialized");
	return sModelManager.get();
}

ModelId FoxEngine::Graphics::ModelManager::GetModelId(const std::filesystem::path& filePath)
{
	return std::filesystem::hash_value(filePath);
}

ModelId FoxEngine::Graphics::ModelManager::LoadModel(const std::filesystem::path& filePath)
{
	const ModelId modelId = GetModelId(filePath);
	auto [iter, success] = mInventory.insert({ modelId, nullptr });
	if (success)
	{
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		ModelIO::LoadModel(filePath, *modelPtr);
		ModelIO::LoadMaterial(filePath, *modelPtr);
		ModelIO::LoadSkeleton(filePath, *modelPtr);
		ModelIO::LoadAnimations(filePath, *modelPtr);
	}
	return modelId;
}

const Model* FoxEngine::Graphics::ModelManager::GetModel(ModelId id)
{
	auto model = mInventory.find(id);
	if (model != mInventory.end())
	{
		return model->second.get();
	}

	return nullptr;
}
