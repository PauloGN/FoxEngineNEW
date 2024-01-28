#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"
using namespace FoxEngine;

void FoxEngine::GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld: is already initialized");

	for (auto& service : mServices)
	{
		service->Initialize();
	}

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);
	mInitialized = true;
}

void FoxEngine::GameWorld::Terminate()
{
	ASSERT(!mUdating, "GameWorld: cant terminate while updating");

	if (mInitialized)
	{
		return;
	}

	for (auto& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->Terminate();
		}
	}

	for (auto& service : mServices)
	{
		service->Terminate();
	}

	mInitialized = false;
}

void FoxEngine::GameWorld::Update(float deltaTime)
{
	for (auto& service : mServices)
	{
		service->Update(deltaTime);
	}
}

void FoxEngine::GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
	}
}

void FoxEngine::GameWorld::DebugUI()
{
	for (auto& service : mServices)
	{
		service->DebugUI();
	}
}

GameObject* FoxEngine::GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
	ASSERT(mInitialized, "GameWorld: is not initialized");

	if (mFreeSlots.empty())
	{
		ASSERT(false, "GameWorld: is not initialized");
		return nullptr;
	}

	const uint32_t freeslot = mFreeSlots.back();
	mFreeSlots.pop_back();

	Slot& slot = mGameObjectSlots[freeslot];
	std::unique_ptr<GameObject>& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	GameObjectFactory::Make(templateFile, *newObject);

	newObject->mWorld = this;
	newObject->mHandle.mIndex = freeslot;
	newObject->mHandle.mGeneration = slot.generation;
	newObject->Initialize();

	return newObject.get();
}

GameObject* FoxEngine::GameWorld::GetGameObject(const GameObjectHandle& handle)
{
	return nullptr;
}

void FoxEngine::GameWorld::DestroyObject(const GameObjectHandle& handle)
{}

bool FoxEngine::GameWorld::IsValid(GameObjectHandle & handle)
{
	return false;
}

void FoxEngine::GameWorld::ProcessDestroyList()
{}

