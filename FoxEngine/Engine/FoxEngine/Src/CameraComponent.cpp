#include "Precompiled.h"
#include "CameraComponent.h"

#include "GameWorld.h"
#include "CameraService.h"

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

		mCamera.SetPosition({x, y, z});
	}

	if (value.HasMember("LookAt"))
	{
		const auto& lookAt = value["LookAt"].GetArray();

		float x = lookAt[0].GetFloat();
		float y = lookAt[1].GetFloat();
		float z = lookAt[2].GetFloat();

		mCamera.SetLookAt({ x, y, z });
	}
}
