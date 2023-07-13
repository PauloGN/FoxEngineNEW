#include "Precompiled.h"
#include "TextureManager.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sInstance;
}

void FoxEngine::Graphics::TextureManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sInstance == nullptr, "Texture Manager is already initialized");
	sInstance = std::make_unique<TextureManager>();
	sInstance->SetRootDirectory(root);
}

void FoxEngine::Graphics::TextureManager::StaticTerminate()
{
	sInstance.reset();
}

TextureManager* FoxEngine::Graphics::TextureManager::Get()
{
	ASSERT(sInstance != nullptr, "TextureManager is not initialized");
	return sInstance.get();
}

FoxEngine::Graphics::TextureManager::~TextureManager()
{
	for (auto& [id, texture]: mInventory)
	{
		texture->Terminate();
	}
	
	mInventory.clear();
}

TextureId TextureManager::LoadTexture(const std::filesystem::path& fileName, bool useRootDir)
{
	auto textureId = std::filesystem::hash_value(fileName);
	auto [iter, success] = mInventory.insert({textureId, nullptr});
	if (success)
	{
		auto& texturePtr = iter->second;
		texturePtr = std::make_unique<Texture>();
		texturePtr->Initialize((useRootDir)? mRootDirectory / fileName: fileName);
	}
	return textureId;
}

//const Texture* TextureManager::GetTexture(TextureId id)
//{
//	auto iter = mInventory.find(id);
//
//	if (iter != mInventory.end())
//	{
//		return iter->second.get();
//	}
//
//	return nullptr;
//}

void FoxEngine::Graphics::TextureManager::BindVS(TextureId id, uint32_t slot) const
{
	if (id == 0)
	{
		return;
	}

	auto iter = mInventory.find(id);

	if (iter != mInventory.end())
	{
		return iter->second->BindVS(slot);
	}
}

void FoxEngine::Graphics::TextureManager::BindPS(TextureId id, uint32_t slot) const
{
	if (id == 0)
	{
		return;
	}

	auto iter = mInventory.find(id);

	if (iter != mInventory.end())
	{
		return iter->second->BindPS(slot);
	}
}
