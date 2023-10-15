#include "Precompiled.h"
#include "Animation.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{
	float GetLerpTime(float startTime, float endTime, float time)
	{
		return(time - startTime) / (endTime - startTime);
	}
}

Transform Animation::GetTransform(float time) const
{
	Transform transform;
	transform.position = GetPosition(time);
	transform.rotation = GetRotation(time);
	transform.scale = GetScale(time);

	return transform;
}

float FoxEngine::Graphics::Animation::GetDuration() const
{
	return mDuration;
}

FoxMath::Vector3 FoxEngine::Graphics::Animation::GetPosition(float time) const
{
	for (uint32_t i = 0; i < mPositionKeys.size(); ++i)
	{
		if (time < mPositionKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time);
				return FoxMath::Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, t);
			}
			return mPositionKeys[i].key;
		}
	}
	if (!mPositionKeys.empty())
	{
		return mPositionKeys.back().key;
	}
	return FoxMath::Vector3::Zero;
}

FoxMath::Quaternion FoxEngine::Graphics::Animation::GetRotation(float time) const
{
	for (uint32_t i = 0; i < mRotationKeys.size(); ++i)
	{
		if (time < mRotationKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time);
				return FoxMath::Quaternion::Slerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, t);
			}

			return mRotationKeys[i].key;
		}
	}
	if (!mRotationKeys.empty())
	{
		return mRotationKeys.back().key;
	}

	return FoxMath::Quaternion::Identity;
}

FoxMath::Vector3 FoxEngine::Graphics::Animation::GetScale(float time) const
{
	for (uint32_t i = 0; i < mScaleKeys.size(); ++i)
	{
		if (time < mScaleKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time);
				return FoxMath::Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, t);
			}
			return mScaleKeys[i].key;
		}
	}
	if (!mScaleKeys.empty())
	{
		return mScaleKeys.back().key;
	}
	return FoxMath::Vector3::One;
}
