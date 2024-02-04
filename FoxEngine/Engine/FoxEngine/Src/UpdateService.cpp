#include "Precompiled.h"
#include "UpdateService.h"
#include "Component.h"

using namespace FoxEngine;

void FoxEngine::UpdateService::Update(float deltaTime)
{
	mIsUpdating = true;
	for (Component* component : mUpdateComponent)
	{
		component->Update(deltaTime);
	}
	mIsUpdating = false;

	RegisterPendingUpdates();
}

void UpdateService::Deserialize(rapidjson::Value& value)
{

}

void FoxEngine::UpdateService::Register(Component * component)
{
	if (mIsUpdating)
	{
		mPendingUpdateComponent.push_back(component);
	}
	else
	{
		mUpdateComponent.push_back(component);
	}
}

void FoxEngine::UpdateService::Unregister(Component * component)
{
	auto iter = std::find(mUpdateComponent.begin(), mUpdateComponent.end(), component);
	if (iter != mUpdateComponent.end())
	{
		mUpdateComponent.erase(iter);
	}

	iter = std::find(mPendingUpdateComponent.begin(), mPendingUpdateComponent.end(), component);
	if (iter != mPendingUpdateComponent.end())
	{
		mPendingUpdateComponent.erase(iter);
	}
}

void FoxEngine::UpdateService::RegisterPendingUpdates()
{
	for (Component* component : mPendingUpdateComponent)
	{
		mUpdateComponent.push_back(component);
	}
	mPendingUpdateComponent.clear();
}
