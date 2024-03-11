#pragma once

#include "Component.h"

namespace FoxEngine
{
	class CameraComponent;
	class TransformComponent;

	class TPSCameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::TPSCamera)

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;

		void EditorUI() override;

		void Deserialize(const rapidjson::Value& value)override;
	private:

		struct TiltData
		{
			bool hasTiltMechanic = true;
			float rollAngle = 1.0f;
			float interpolationRate = 0.2f;
			float backInterpolationRate = 2.0f;
		};

		struct CameraSettings
		{
			// Control camera height - adjust this as needed
			float minHeight = 5.0f;
			float maxHeight = -4.f;
			bool  OrientRotationToMovement = true;
			float sensitivity = 0.8f;
		};

		void UpdateMouseInput(const float deltaTime);
		void LateUpdate(Graphics::Camera& camera);
		void Tilt(float deltaTime);
		void DefineMoveSpeed(float deltaTime);
		void Rotate(const FoxMath::Vector3& cameraForward, const float deltaTime);

		TransformComponent* mTransformComponent = nullptr;
		CameraComponent* mCameraComponet = nullptr;

		Vector3 mEulerOffsetRotation{};

		float mTargetDistance = 7.0f;
		float mtargetZoomDistance = 2.0f;
		float mTargetHeight = 1.0f;
		float mtargetMaxDistance = 7.0f;

		TiltData mTiltData;
		CameraSettings mCameraSettings;

		float currentX = 0.0f;
		float currentY = 0.0f;
	};
}