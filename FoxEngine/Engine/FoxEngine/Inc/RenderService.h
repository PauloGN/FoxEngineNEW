#pragma once

#include "Service.h"


namespace FoxEngine
{
	class CameraService;

	class RenderService final: public Service
	{
	public:

		SET_TYPE_ID(ServiceId::Render)

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void Deserialize(rapidjson::Value& value) override;

	private:

		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::ShadowEffect mShadowEffect;

		float mFPS = 0.0f;


	};
}
