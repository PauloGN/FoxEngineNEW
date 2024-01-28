#include "Precompiled.h"
#include "CameraService.h"
#include "CameraComponent.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

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
		mMainCamera = mCameraEntries[index];
	}
}

void CameraService::Register(const CameraComponent* cameraComponent)
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
