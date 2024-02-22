#pragma once
#include "Component.h"

namespace FoxEngine
{
	class CameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Camera)

		void Initialize() override;
		void Terminate() override;

		void Deserialize(const rapidjson::Value& value)override;

		Graphics::Camera& GetCamera() { return mCamera; }
		const Graphics::Camera& GetCamera() const { return mCamera; }

	private:

		Graphics::Camera mCamera;
	};
}
