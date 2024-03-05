#include "Precompiled.h"
#include "TPSCameraComponent.h"

#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "SaveUtil.h"
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
	ASSERT(updateService != nullptr, "TPSCameraComponente: Update service is unavailable ");
	updateService->Register(this);
	input = Input::InputSystem::Get();
}

void FoxEngine::TPSCameraComponent::Terminate()
{
	UpdateService* updateService = GetOwner().GetWorld().GetService<UpdateService>();
	ASSERT(updateService != nullptr, "TPSCameraComponente: Update service is unavailable ");
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

void TPSCameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);

	SaveUtil::SaveFloat("TargetMaxDistance", mtargetMaxDistance, doc, componentValue);
	SaveUtil::SaveFloat("TargetZoomDistance", mtargetZoomDistance, doc, componentValue);
	SaveUtil::SaveFloat("Sensitivity", mSensitivity, doc, componentValue);
	SaveUtil::SaveFloat("TargetHeight", mTargetHeight, doc, componentValue);

	//Save component name/data 
	value.AddMember("TPSCameraComponent", componentValue, doc.GetAllocator());
}

void TPSCameraComponent::EditorUI()
{
	std::string headerTag = "TPS Controller: " + GetOwner().GetName();
	if (ImGui::CollapsingHeader(headerTag.c_str()))
	{
		bool bUpdate = false;

		if (ImGui::DragFloat("Sensitivity: ", &mSensitivity))
		{
			bUpdate = true;
		}
		if (ImGui::DragFloat("TargetMaxDistance: ", &mtargetMaxDistance))
		{
			bUpdate = true;
		}
		if (ImGui::DragFloat("TargetHeight: ", &mTargetHeight))
		{
			bUpdate = true;
		}
		if (ImGui::DragFloat("Zoom Distance: ", &mtargetZoomDistance))
		{
			bUpdate = true;
		}
	}
}

void FoxEngine::TPSCameraComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("TargetMaxDistance"))
	{
		mtargetMaxDistance = value["TargetMaxDistance"].GetFloat();
	}

	if (value.HasMember("Sensitivity"))
	{
		mSensitivity = value["Sensitivity"].GetFloat();
	}

	if (value.HasMember("TargetHeight"))
	{
		mTargetHeight = value["TargetHeight"].GetFloat();
	}

	if (value.HasMember("TargetZoomDistance"))
	{
		mtargetZoomDistance = value["TargetZoomDistance"].GetFloat();
	}
}

void TPSCameraComponent::UpdateMouseInput(const float deltaTime)
{
	const float mouseX = input->GetMouseMoveX();
	const float mouseY = input->GetMouseMoveY();

	// Use mouseY for horizontal rotation
	currentX -= mouseY * mSensitivity * deltaTime;

	// Use mouseX for vertical rotation
	currentY += mouseX * mSensitivity * deltaTime;
}

void TPSCameraComponent::LateUpdate(Camera& camera)
{
	float targetDistance = mtargetMaxDistance;
	float targetHeight = 121.5;

	// Set target zoom positions based on bCamZoom
	const bool bCamZoom = input->IsMouseDown(MouseButton::RBUTTON);
	if (bCamZoom)
	{
		targetDistance = mtargetZoomDistance;
	}

	// Smoothly interpolate the mZdistance and height values separately
	mTargetDistance = FoxMath::Lerp(mTargetDistance, targetDistance, zoomLerpSpeed);
	float height = FoxMath::Lerp(0.0f, targetHeight, zoomLerpSpeed);

	// Calculate the character position height
	FoxMath::Quaternion rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, currentY, currentX);

	FoxMath::Vector3 charPosHeight = rotation.Rotate(FoxMath::Vector3(0.0f, mTargetHeight + height, 0.0f));

	// Calculate the offset for camera position
	FoxMath::Vector3 offset = rotation.Rotate(FoxMath::Vector3(0.0f, 0.0f, -mTargetDistance));

	// Limit the pitch angle to avoid looking directly up or down
	constexpr float minPitch = -15.0f; // Adjust as needed
	constexpr float maxPitch = 15.0f;  // Adjust as needed

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

	// Control camera height - adjust this as needed
	float minHeight = -3.0f;
	float maxHeight = mtargetMaxDistance - (mtargetMaxDistance * 0.1);

	// Check if the camera is too high or too low
	if (targetPosition.y > maxHeight || targetPosition.y < minHeight)
	{
		// Camera is out of bounds, return without making further adjustments
		return;
	}

	// Set the position and look-at for the camera
	camera.SetPosition(targetPosition);
	camera.SetLookAt(mTransformComponent->position + charPosHeight);
}

void TPSCameraComponent::Rotate(const FoxMath::Vector3& cameraForward)
{
	// Set character rotation based on camera forward vector
	FoxMath::Vector3 lookAt = FoxMath::Normalize(FoxMath::Vector3(cameraForward.x, 0.0f, cameraForward.z));
	float yaw = atan2(lookAt.x, lookAt.z);

	mTransformComponent->rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, yaw, 0.0f);
}