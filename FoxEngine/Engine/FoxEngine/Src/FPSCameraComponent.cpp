#include "Precompiled.h"
#include "FPSCameraComponent.h"

#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "UpdateService.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void FoxEngine::FPSCameraComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mCameraComponet = GetOwner().GetComponent<CameraComponent>();
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponente: Update service is unavailable ");
	updateService->Register(this);
}

void FoxEngine::FPSCameraComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponente: Update service is unavailable ");
	updateService->Unregister(this);
	mCameraComponet = nullptr;
}

void FoxEngine::FPSCameraComponent::Update(float deltaTime)
{
	Camera& camera = mCameraComponet->GetCamera();
	if (!camera.GetIsActive())
	{
		return;
	}

	const InputSystem* input = Input::InputSystem::Get();
	const int moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10 : mMoveSpeed;
	const float displacement = mTurnpeed * moveSpeed;


	//Foward and Backward
	if (input->IsKeyDown(KeyCode::W))
	{
		camera.Walk(displacement);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		camera.Walk(-displacement);
	}
	//Right and Left
	if (input->IsKeyDown(KeyCode::D))
	{
		camera.Strafe(displacement);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		camera.Strafe(-displacement);
	}

	//Rotation
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		const int turnSpeedMultiplyer = input->IsKeyDown(KeyCode::LSHIFT) ? 2 : 1;

		const float x = input->GetMouseMoveX() * deltaTime * turnSpeedMultiplyer;
		const float y = input->GetMouseMoveY() * deltaTime * turnSpeedMultiplyer;

		camera.Yaw(x);
		camera.Pitch(y);
	}

	//UP and Down
	if (input->IsKeyDown(KeyCode::Q))
	{
		camera.Rise(-displacement);
	}
	else if (input->IsKeyDown(KeyCode::E))
	{
		camera.Rise(displacement);
	}

	if (mTransformComponent != nullptr)
	{
		mTransformComponent->position = camera.GetPosition();
	}
}

void FoxEngine::FPSCameraComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("MoveSpeed"))
	{
		mMoveSpeed = value["MoveSpeed"].GetFloat();
	}

	if (value.HasMember("TurnSpeed"))
	{
		mTurnpeed = value["TurnSpeed"].GetFloat();
	}
}