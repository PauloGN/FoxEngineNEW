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

		void UpdateMouseInput(const float deltaTime);
		void LateUpdate(Graphics::Camera& camera);
		void Rotate(const FoxMath::Vector3& cameraForward);

		TransformComponent* mTransformComponent = nullptr;
		CameraComponent* mCameraComponet = nullptr;

		float mTargetDistance = 7.0f;
		float mtargetZoomDistance = 2.0f;
		float mTargetHeight = 1.0f;
		float mtargetMaxDistance = 7.0f;
		float mSensitivity = 0.8f;

		float currentX = 0.0f;
		float currentY = 0.0f;
	};
}