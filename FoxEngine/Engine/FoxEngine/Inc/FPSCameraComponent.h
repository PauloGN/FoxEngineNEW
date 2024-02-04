#pragma once

#include "Component.h"

namespace FoxEngine
{
	class CameraComponent;
	class TransformComponent;

	class FPSCameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FPSCamera)

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Deserialize(rapidjson::Value& value)override;
	private:

		TransformComponent* mTransformComponent = nullptr;
		CameraComponent* mCameraComponet = nullptr;
		float mMoveSpeed = 1.0f;
		float mTurnpeed = 0.1f;
	};
}