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
		void Deserialize(const rapidjson::Value& value)override;
	private:

		void UpdateMouseInput(const float deltaTime);
		void LateUpdate(Graphics::Camera& camera);
		void Rotate(const FoxMath::Vector3& cameraForward);

		TransformComponent* mTransformComponent = nullptr;
		CameraComponent* mCameraComponet = nullptr;
		float mMoveSpeed = 1.0f;
		float mTurnpeed = 0.1f;

		float distance = 7.0f;
		float sensitivity = 0.8f;
		float minYAngle = -30.0f;
		float maxYAngle = 70.0f;

		float currentX = 0.0f;
		float currentY = 0.0f;
	};
}