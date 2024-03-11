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
	float movementSpeed = 0.0f;
	bool isCameraTooHigh;
	bool isCameraTooLow;

	float lastMouseY;

	const InputSystem* input;
	constexpr float zoomLerpSpeed = 0.02f;

	float gLastBaseMoveSpeed = 0.0f;
	float gLastMaxMoveSpeed = 0.0f;
	
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
	Rotate(-camera.GetDirection(), deltaTime);
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

void TPSCameraComponent::EditorUI()
{
	const std::string headerTag = "TPS Controller: " + GetOwner().GetName();
	if (ImGui::CollapsingHeader(headerTag.c_str()))
	{
		//Camera Settings
		ImGui::Text("Camera Settings");
		ImGui::DragFloat("Sensitivity: ", &mCameraSettings.sensitivity);
		ImGui::DragFloat("Min. Camera Height: ", &mCameraSettings.minHeight);
		ImGui::DragFloat("Max. Camera Height: ", &mCameraSettings.maxHeight);
		ImGui::Checkbox("Orient Rotation To Movement", &mCameraSettings.OrientRotationToMovement);

		//Object Settings
		ImGui::Spacing();
		ImGui::Text("Object Settings");
		ImGui::DragFloat("TargetMaxDistance: ", &mtargetMaxDistance);
		ImGui::DragFloat("TargetHeight: ", &mTargetHeight);
		ImGui::DragFloat("Zoom Distance: ", &mtargetZoomDistance);
		ImGui::DragFloat3("OffSetRotation: ", &mEulerOffsetRotation.x);

		//Tilt Settings
		ImGui::Spacing();
		ImGui::Text("Tilt Settings");
		ImGui::Checkbox("Activate Tilt", &mTiltData.hasTiltMechanic);
		ImGui::DragFloat("BackInterpolationRate: ", &mTiltData.backInterpolationRate);
		ImGui::DragFloat("InterpolationRate: ", &mTiltData.interpolationRate);
		ImGui::DragFloat("Roll Angle: ", &mTiltData.rollAngle);
	}
}

void TPSCameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);

	//Camera Settings
	SaveUtil::SaveFloat("Sensitivity", mCameraSettings.sensitivity, doc, componentValue);
	SaveUtil::SaveFloat("MinHeight", mCameraSettings.minHeight, doc, componentValue);
	SaveUtil::SaveFloat("MaxHeight", mCameraSettings.maxHeight, doc, componentValue);
	SaveUtil::SaveBool("OrientRotationToMovement", mCameraSettings.OrientRotationToMovement, doc, componentValue);

	//Object Settings
	SaveUtil::SaveFloat("TargetMaxDistance", mtargetMaxDistance, doc, componentValue);
	SaveUtil::SaveFloat("TargetZoomDistance", mtargetZoomDistance, doc, componentValue);
	SaveUtil::SaveFloat("TargetHeight", mTargetHeight, doc, componentValue);
	if(mEulerOffsetRotation.x != 0.0f ||
		mEulerOffsetRotation.y != 0.0f||
		mEulerOffsetRotation.z != 0.0f)
	{
		SaveUtil::SaveVector3("OffSetRotation", mEulerOffsetRotation, doc, componentValue);
	}

	//Tilt Settings
	if(mTiltData.hasTiltMechanic)
	{
		SaveUtil::SaveBool("TiltMechanic", mTiltData.hasTiltMechanic, doc, componentValue);
		SaveUtil::SaveFloat("BackInterpolationRate", mTiltData.backInterpolationRate, doc, componentValue);
		SaveUtil::SaveFloat("InterpolationRate", mTiltData.interpolationRate, doc, componentValue);
		SaveUtil::SaveFloat("RollAngle", mTiltData.rollAngle, doc, componentValue);
	}

	//Save component name/data 
	value.AddMember("TPSCameraComponent", componentValue, doc.GetAllocator());
}

void FoxEngine::TPSCameraComponent::Deserialize(const rapidjson::Value& value)
{

	//Camera Settings
	if (value.HasMember("Sensitivity"))
	{
		mCameraSettings.sensitivity = value["Sensitivity"].GetFloat();
	}
	if (value.HasMember("MinHeight"))
	{
		mCameraSettings.minHeight = value["MinHeight"].GetFloat();
	}
	if (value.HasMember("MaxHeight"))
	{
		mCameraSettings.maxHeight = value["MaxHeight"].GetFloat();
	}
	if (value.HasMember("OrientRotationToMovement"))
	{
		mCameraSettings.sensitivity = value["OrientRotationToMovement"].GetBool();
	}

	//Object Settings
	if (value.HasMember("TargetZoomDistance"))
	{
		mtargetZoomDistance = value["TargetZoomDistance"].GetFloat();
	}
	if (value.HasMember("TargetMaxDistance"))
	{
		mtargetMaxDistance = value["TargetMaxDistance"].GetFloat();
	}
	if (value.HasMember("TargetHeight"))
	{
		mTargetHeight = value["TargetHeight"].GetFloat();
	}
	if (value.HasMember("OffSetRotation"))
	{
		const auto& rot = value["OffSetRotation"].GetArray();
		mEulerOffsetRotation.x = rot[0].GetFloat();
		mEulerOffsetRotation.y = rot[1].GetFloat();
		mEulerOffsetRotation.z = rot[2].GetFloat();
	}

	//Tilt Settings
	if (value.HasMember("TiltMechanic"))
	{
		mTiltData.hasTiltMechanic = value["TiltMechanic"].GetBool();
	}

	if (value.HasMember("BackInterpolationRate"))
	{
		mTiltData.backInterpolationRate = value["BackInterpolationRate"].GetFloat();
	}

	if (value.HasMember("InterpolationRate"))
	{
		mTiltData.interpolationRate = value["InterpolationRate"].GetFloat();
	}

	if (value.HasMember("RollAngle"))
	{
		mTiltData.rollAngle = value["RollAngle"].GetFloat();
	}

}

void TPSCameraComponent::UpdateMouseInput(const float deltaTime)
{
	const float mouseX = input->GetMouseMoveX();
	const float mouseY = input->GetMouseMoveY();

	// Use mouseX for vertical rotation
	currentX += mouseX * mCameraSettings.sensitivity * deltaTime;
	// Use mouseY for horizontal rotation
	currentY -= mouseY * mCameraSettings.sensitivity * deltaTime;
}

void TPSCameraComponent::LateUpdate(Camera& camera)
{
	float targetDistance = mtargetMaxDistance;
	const float targetHeight = 121.5;

	// Set target zoom positions based on bCamZoom
	const bool bCamZoom = input->IsMouseDown(MouseButton::RBUTTON);
	if (bCamZoom)
	{
		targetDistance = mtargetZoomDistance;
	}

	if(isCameraTooHigh && currentY > lastMouseY)
	{
		currentY = lastMouseY;
	}
	else if(isCameraTooLow && currentY < lastMouseY)
	{
		currentY = lastMouseY;
	}else
	{
		lastMouseY = currentY;
	}

	// Smoothly interpolate the mZdistance and height values separately
	mTargetDistance = FoxMath::Lerp(mTargetDistance, targetDistance, zoomLerpSpeed);
	float height = FoxMath::Lerp(0.0f, targetHeight, zoomLerpSpeed);

	// Calculate the character position height
	FoxMath::Quaternion rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, currentX, currentY);

	FoxMath::Vector3 charPosHeight = rotation.Rotate(FoxMath::Vector3(0.0f, mTargetHeight + height, 0.0f));

	// Calculate the offset for camera position
	FoxMath::Vector3 offset = rotation.Rotate(FoxMath::Vector3(0.0f, 0.0f, -mTargetDistance));

	// Limit the pitch angle to avoid looking directly up or down
	constexpr float minPitch = -1.0f; // Adjust as needed
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
		currentY = lastMouseY;
	}

	// Adjust pitch angle for camera rotation based on limits
	rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, pitch * 180.0f / FoxMath::Constants::Pi, currentX);

	// Calculate the target position for the camera
	FoxMath::Vector3 targetPosition = mTransformComponent->position + offset;

	// Check if the camera is too high or too low
	isCameraTooHigh = mTransformComponent->position.y - camera.GetPosition().y < mCameraSettings.maxHeight;
	isCameraTooLow = mTransformComponent->position.y - camera.GetPosition().y > mCameraSettings.minHeight;

	// Set the position and look-at for the camera
	camera.SetPosition(targetPosition);
	camera.SetLookAt(mTransformComponent->position + charPosHeight);
}

void TPSCameraComponent::Tilt(const float deltaTime)
{
	// Introduce roll angle based on A or D key press
	float rollAngle = 0.0f;
	float interpolationRate = mTiltData.backInterpolationRate;

	if (input->IsKeyDown(KeyCode::A))
	{
		rollAngle = -mTiltData.rollAngle; // Adjust as needed
		interpolationRate = mTiltData.interpolationRate;
	}
	else if (input->IsKeyDown(KeyCode::D))
	{
		rollAngle = mTiltData.rollAngle; // Adjust as needed
		interpolationRate = mTiltData.interpolationRate;;
	}

	rollAngle = FoxMath::Lerp(mTransformComponent->vrotation.y, rollAngle, interpolationRate * deltaTime);
	mTransformComponent->vrotation.y = rollAngle;
}

void TPSCameraComponent::DefineMoveSpeed(const float deltaTime)
{
	float baseMovementSpeed = 50.0f * deltaTime;
	float sprintMovementSpeed = 200.0f * deltaTime;

	if(baseMovementSpeed > gLastBaseMoveSpeed)
	{
		gLastBaseMoveSpeed = baseMovementSpeed;
	}
	if (sprintMovementSpeed > gLastMaxMoveSpeed)
	{
		gLastMaxMoveSpeed = sprintMovementSpeed;
	}

	// Define interpolation rate for movement speed
	float lerpRate = 0.02f;

	// Update object movement speed based on input
	float targetMovementSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? gLastMaxMoveSpeed : gLastBaseMoveSpeed;

	if(targetMovementSpeed > movementSpeed)
	{
		movementSpeed = FoxMath::Lerp(movementSpeed, targetMovementSpeed, lerpRate);
	}else if(movementSpeed > 0.0f)
	{
		movementSpeed = FoxMath::Lerp(movementSpeed, 0.0f, lerpRate);
	}
}

void TPSCameraComponent::Rotate(const FoxMath::Vector3& cameraForward, const float deltaTime)
{
	// Set character rotation based on camera forward vector
	const FoxMath::Vector3 lookAt = FoxMath::Normalize(FoxMath::Vector3(cameraForward.x, 0.0f, cameraForward.z));
	float yaw = atan2(lookAt.x, lookAt.z);

	yaw += mEulerOffsetRotation.y;

	if (mTiltData.hasTiltMechanic)
	{
		Tilt(deltaTime);
	}

	// Calculate movement direction based on camera forward vector
	FoxMath::Vector3 movementDirection = mCameraSettings.OrientRotationToMovement ? lookAt : FoxMath::GetForwardVectorFromQuaternion(mTransformComponent->rotation);

	if(!mCameraSettings.OrientRotationToMovement)
	{
		movementDirection.y = 0.0f;
	}

	// Define movement speeds
	DefineMoveSpeed(deltaTime);

	// Update object position based on input
	if (input->IsKeyDown(KeyCode::W))
	{
		mTransformComponent->position -= movementDirection * movementSpeed;
	}

	if (input->IsKeyDown(KeyCode::S))
	{
		mTransformComponent->position += movementDirection * movementSpeed;
	}

	// Perpendicular vector to the forward direction for strafing
	FoxMath::Vector3 strafeDirection = FoxMath::Cross(FoxMath::Vector3::YAxis, movementDirection);

	if (input->IsKeyDown(KeyCode::A))
	{
		mTransformComponent->position += strafeDirection * movementSpeed;
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		mTransformComponent->position -= strafeDirection * movementSpeed;
	}

	// Translate up and down based on input
	if (input->IsKeyDown(KeyCode::E)) // Up
	{
		mTransformComponent->position += FoxMath::Vector3::YAxis * movementSpeed;
	}

	if (input->IsKeyDown(KeyCode::Q)) // Down
	{
		mTransformComponent->position -= FoxMath::Vector3::YAxis * movementSpeed;
	}

	// Update object rotation
	if(mCameraSettings.OrientRotationToMovement)
	{
		mTransformComponent->rotation = FoxMath::Quaternion::CreateFromYawPitchRoll(0.0f, yaw, mEulerOffsetRotation.z);
	}

	if (input->IsKeyPressed(KeyCode::F1))
	{
		mCameraSettings.OrientRotationToMovement = !mCameraSettings.OrientRotationToMovement;
	}
}
