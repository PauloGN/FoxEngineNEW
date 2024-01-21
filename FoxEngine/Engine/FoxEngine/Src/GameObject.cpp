#include "Precompiled.h"
#include "GameObject.h"

using namespace FoxEngine;

static uint32_t gUniqueId = 0;

void FoxEngine::GameObject::Initialize()
{
	ASSERT(!mInitialized, "Game Object is already initialized");
	mInitialized = true;
	mUniqueId = ++gUniqueId;
}

void FoxEngine::GameObject::Terminate()
{
	//later
}

void FoxEngine::GameObject::DebugUI()
{
	//later
}