#pragma once

#include "Service.h"

namespace FoxEngine
{
	class CameraComponent;

	class CameraService final : public Service
	{

	public:
		SET_TYPE_ID(ServiceId::Camera)

		void DebugUI()override;

		const Graphics::Camera& GetMain() const;
		void SetMainCamera(uint32_t index);

	private:

		friend class CameraComponent;

		void Register(CameraComponent* cameraComponent);
		void Unregister(const CameraComponent* cameraComponent);
		using CameraEntries = std::vector<CameraComponent*>;
		CameraEntries mCameraEntries;
		CameraComponent* mMainCamera = nullptr;
	};

}
