#include "Precompiled.h"
#include "CameraService.h"
#include "CameraComponent.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void FoxEngine::CameraService::DebugUI()
{
	SimpleDraw::Render(GetMain());
}

const Camera& CameraService::GetMain() const
{
	ASSERT(mMainCamera != nullptr, "CameraServices: has no main camera");
	//return mMainCamera->GetOwner();
	return mMainCamera->GetCamera();
}

void FoxEngine::CameraService::SetMainCamera(uint32_t index)
{
	if (index < mCameraEntries.size())
	{
		mMainCamera->GetCamera().SetIsActive(false);
		mMainCamera = mCameraEntries[index];
		mMainCamera->GetCamera().SetIsActive(true);
	}
}

void CameraService::Register( CameraComponent* cameraComponent)
{
	mCameraEntries.push_back(cameraComponent);
	if(mMainCamera == nullptr)
	{
		mMainCamera = cameraComponent;
	}
}

void CameraService::Unregister(const CameraComponent* cameraComponent)
{
	auto iter = std::find(mCameraEntries.begin(), mCameraEntries.end(), cameraComponent);

	if(iter != mCameraEntries.end())
	{
		if(mMainCamera == *iter)
		{
			mMainCamera = nullptr;
		}

		mCameraEntries.erase(iter);

		if(mMainCamera == nullptr && !mCameraEntries.empty())
		{
			mMainCamera = mCameraEntries.front();
		}
	}
}
