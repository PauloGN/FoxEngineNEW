#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameWorld.h"
#include "CameraService.h"
#include "SaveUtil.h"

using namespace FoxEngine;

void FoxEngine::CameraComponent::Initialize()
{
	//Component::Initialize();
	CameraService* cameraServive = GetOwner().GetWorld().GetService<CameraService>();
	ASSERT(cameraServive != nullptr, "CameraComponent: CameraService is not available");
	cameraServive->Register(this);
}

void FoxEngine::CameraComponent::Terminate()
{
	//Component::Terminate();
	CameraService* cameraServive = GetOwner().GetWorld().GetService<CameraService>();
	ASSERT(cameraServive != nullptr, "CameraComponent: CameraService is not available");
	cameraServive->Unregister(this);
}

void CameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Position", mStartingPosition, doc, componentValue);
	SaveUtil::SaveVector3("LookAt", mStartingLookAt, doc, componentValue);

	//Save component name/data 
	value.AddMember("CameraComponent", componentValue, doc.GetAllocator());
}

void FoxEngine::CameraComponent::Deserialize(const rapidjson::Value& value)
{
	// position, direction , look at
	// in case read Mode
	// if perspective, fov, aspect ratio
	// if orthographic, read size
	// new/far planes
	// zoom

	if (value.HasMember("Position"))
	{
		const auto& pos = value["Position"].GetArray();

		float x = pos[0].GetFloat();
		float y = pos[1].GetFloat();
		float z = pos[2].GetFloat();

		mStartingPosition = {x, y, z};
		mCamera.SetPosition(mStartingPosition);
	}

	if (value.HasMember("LookAt"))
	{
		const auto& lookAt = value["LookAt"].GetArray();

		float x = lookAt[0].GetFloat();
		float y = lookAt[1].GetFloat();
		float z = lookAt[2].GetFloat();

		mStartingLookAt = { x, y, z };
		mCamera.SetLookAt(mStartingLookAt);
	}
}
