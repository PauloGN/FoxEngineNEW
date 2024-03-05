#include "Precompiled.h"
#include "MagnetifyComponent.h"

#include "FPSCameraComponent.h"

#include "TransformComponent.h"
#include "GameWorld.h"
#include "UpdateService.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void FoxEngine::MagnetifyComponent::Initialize()
{
	mPosition = &GetOwner().GetComponent<TransformComponent>()->position;
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "Magnetify Component: Update service is unavailable ");
	updateService->Register(this);

}

void FoxEngine::MagnetifyComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "Magnetify Component: Update service is unavailable ");
	updateService->Unregister(this);
}

void FoxEngine::MagnetifyComponent::Update(float deltaTime)
{
	timer += deltaTime;
	if(timer >= timeUpdateRate)
	{
		UpdateInRangeComponentsList();
		UpdateOutOfRangeComponentsList();
		timer = 0.0f;
	}
}

void MagnetifyComponent::UpdateInRangeComponentsList()
{
	std::list<TransformComponent*>::iterator it = mInRangeComponents.begin();
	while (it != mInRangeComponents.end())
	{
		const float dist = Distance((*it)->position , *mPosition);

		if(dist > mExitRadius)
		{
			mOutOfRangeComponents.splice(mOutOfRangeComponents.end(), mInRangeComponents, it++);
			//mOutOfRangeComponents.insert();
			//mInRangeComponents.erase(it++);
		}else
		{
			++it;
		}
	}
}

void MagnetifyComponent::UpdateOutOfRangeComponentsList()
{
	std::list<TransformComponent*>::iterator it = mOutOfRangeComponents.begin();
	while (it != mOutOfRangeComponents.end())
	{
		const float dist = Distance((*it)->position, *mPosition);

		if (dist < mEntryRadius)
		{
			mInRangeComponents.splice(mInRangeComponents.end(), mOutOfRangeComponents, it++);
		}
		else
		{
			++it;
		}
	}
}

void MagnetifyComponent::AttractionEffect(const float dt)
{
	// Checar lista antes de deletar.
	// 
	
	for (auto& obj : mInRangeComponents)
	{
		if(Distance(*mPosition, obj->position) <= minDistance)
		{
			
		}
		
		Vector3 dir = *mPosition - obj->position;
		obj->position += dir * mMoveSpeed * dt;
	}
}

void MagnetifyComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	//TODO
}

void FoxEngine::MagnetifyComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("MoveSpeed"))
	{
		mMoveSpeed = value["MoveSpeed"].GetFloat();
	}
}
