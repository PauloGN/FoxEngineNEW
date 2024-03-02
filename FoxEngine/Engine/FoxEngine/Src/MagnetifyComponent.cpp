#include "Precompiled.h"
#include "MagnetifyComponent.h"

#include "FPSCameraComponent.h"

#include "CameraComponent.h"
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
	UpdateInRangeComponentsList();
}

void MagnetifyComponent::UpdateInRangeComponentsList()
{
	std::list<TransformComponent*>::iterator it = mInRangeComponents.begin();
	while (it != mInRangeComponents.end())
	{
		const float dist = Distance((*it)->position , *mPosition);

		if(dist > mExitRadius)
		{

			mInRangeComponents.splice(it++, mOutOfRangeComponents);

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



}

void FoxEngine::MagnetifyComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("MoveSpeed"))
	{
		mMoveSpeed = value["MoveSpeed"].GetFloat();
	}
}
