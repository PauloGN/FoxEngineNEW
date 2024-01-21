#include "Precompiled.h"
#include "GameObject.h"
#include "TransformComponent.h"

using namespace FoxEngine;

static uint32_t gUniqueId = 0;

void FoxEngine::GameObject::Initialize()
{
	ASSERT(!mInitialized, "Game Object is already initialized");

	for (auto& component : mComponents)
	{
		component->Initialize();
	}

	mUniqueId = ++gUniqueId;
	mInitialized = true;

	ASSERT(HasA<TransformComponent>(), "Game Object must have a trasnform");
}

void FoxEngine::GameObject::Terminate()
{

	for (auto& component : mComponents)
	{
		component->Terminate();
	}
}

void FoxEngine::GameObject::DebugUI()
{
	for (auto& component : mComponents)
	{
		component->DebugUI();
	}
}