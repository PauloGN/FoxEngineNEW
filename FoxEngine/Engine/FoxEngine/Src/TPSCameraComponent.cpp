#include "Precompiled.h"
#include "TPSCameraComponent.h"

#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "UpdateService.h"

using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

namespace
{
	const InputSystem* input;
	constexpr float zoomLerpSpeed = 0.02f;
}


void FoxEngine::TPSCameraComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mCameraComponet = GetOwner().GetComponent<CameraComponent>();
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponente: Update service is unavailable ");
	updateService->Register(this);
	input = Input::InputSystem::Get();
}

void FoxEngine::TPSCameraComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "FPSCameraComponente: Update service is unavailable ");
	updateService->Unregister(this);
	mCameraComponet = nullptr;
}

void FoxEngine::TPSCameraComponent::Update(float deltaTime)
{
	Camera& camera = mCameraComponet->GetCamera();
	if (!camera.GetIsActive())
	{
		return;
	}

	UpdateMouseInput(deltaTime);
	LateUpdate(camera);
	Rotate(-camera.GetDirection());
	// Third-person specific adjustments
	//const float yOffset = 4.0f;
	//const float distanceFromObject = 3.0f;

	//// Calculate the target position for the camera based on the game object's position
	//FoxMath::Vector3 targetPosition = mTransformComponent->position + FoxMath::Vector3(0.0f, yOffset, distanceFromObject);

	//// Set the camera position to be behind the game object at the specified distance
	//camera.SetPosition(targetPosition);

	//// Set the camera look-at direction to point at the game object's position
	//camera.SetLookAt(mTransformComponent->position);

}

void FoxEngine::TPSCameraComponent::Deserialize(const rapidjson::Value& value)
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

void TPSCameraComponent::UpdateMouseInput(const float deltaTime)
{
	const float mouseX = input->GetMouseMoveX();
	const float mouseY = input->GetMouseMoveY();

	// Use mouseY for horizontal rotation
	currentX -= mouseY * sensitivity * deltaTime;

	// Use mouseX for vertical rotation
	currentY += mouseX * sensitivity * deltaTime;

	// Clamp vertical rotation to stay within the specified limits
	currentY = FoxMath::Clamp(currentY, minYAngle, maxYAngle);

	// Ensure that the vertical rotation does not go beyond the specified limits
	if (currentY > maxYAngle)
	{
		currentY = minYAngle;
	}
	else if (currentY < minYAngle)
	{
		currentY = minYAngle;
	}
}

void TPSCameraComponent::LateUpdate(Camera& camera)
{
	float targetDistance = 7.0f;
	float targetHeight = 121.5;

	// Set target zoom positions based on bCamZoom
	const bool bCamZoom = input->IsMouseDown(MouseButton::RBUTTON);
	if (bCamZoom)
	{
		targetDistance = 2.0f;

		float shoulderHeight = 120.0f;  // Set the actual shoulder height of your character here
		targetHeight = shoulderHeight;  // Adjust as needed
	}

	// Smoothly interpolate the mZdistance and height values separately
	mZdistance = FoxMath::Lerp(mZdistance, targetDistance, zoomLerpSpeed);
	float height = FoxMath::Lerp(0.0f, targetHeight, zoomLerpSpeed);

	// Calculate the character position height
	FoxMath::Quaternion rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, currentY, currentX);

	float characterHeight = 1.0f;  // Set the actual height of your character here
	FoxMath::Vector3 charPosHeight = rotation.Rotate(FoxMath::Vector3(0.0f, characterHeight + height, 0.0f));

	// Calculate the offset for camera position
	FoxMath::Vector3 offset = rotation.Rotate(FoxMath::Vector3(0.0f, 0.0f, -mZdistance));

	// Limit the pitch angle to avoid looking directly up or down
	constexpr float minPitch = -35.0f; // Adjust as needed
	constexpr float maxPitch = 45.0f;  // Adjust as needed

	// Extract pitch angle from quaternion manually
	float pitch = asin(2.0f * (rotation.y * rotation.z - rotation.w * rotation.x));

	// Convert degrees to radians manually
	pitch = pitch * FoxMath::Constants::Pi / 180.0f;

	// Clamp pitch angle
	pitch = FoxMath::Clamp(pitch, -maxPitch * FoxMath::Constants::Pi / 180.0f, -minPitch * FoxMath::Constants::Pi / 180.0f);

	// Check if the pitch angle is exactly at the limit and stop further rotation
	if (pitch == -maxPitch * FoxMath::Constants::Pi / 180.0f || pitch == -minPitch * FoxMath::Constants::Pi / 180.0f)
	{
		currentY = 0.0f;
	}

	// Adjust pitch angle for camera rotation based on limits
	rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, pitch * 180.0f / FoxMath::Constants::Pi, currentX);

	// Calculate the target position for the camera
	FoxMath::Vector3 targetPosition = mTransformComponent->position + offset;

	// Set the position and look-at for the camera
	camera.SetPosition(targetPosition);
	camera.SetLookAt(mTransformComponent->position + charPosHeight);
}

void TPSCameraComponent::Rotate(const FoxMath::Vector3& cameraForward)
{
	// Set character rotation based on camera forward vector
	FoxMath::Vector3 lookAt = FoxMath::Normalize(FoxMath::Vector3(cameraForward.x, 0.0f, cameraForward.z));
	float yaw = atan2(lookAt.x, lookAt.z);

	//for (auto& c : character)
	//{
	//	c.transform.rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, yaw, 0.0f);
	//}
	mTransformComponent->rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, yaw, 0.0f);
}