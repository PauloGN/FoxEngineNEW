#include "Precompiled.h"
#include "Animation.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{
	float GetLerpTime(float startTime, float endTime, float time, EaseType easeType)
	{
		float t = (time - startTime) / (endTime - startTime);

		switch (easeType)
		{
		case FoxEngine::Graphics::EaseType::Linear:	break;
		case FoxEngine::Graphics::EaseType::EaseInQuad: { t = t* t;} break;
		case FoxEngine::Graphics::EaseType::EaseOutQuad: { t = t * (2.0f - t); } break;
		case FoxEngine::Graphics::EaseType::EaseInOutQuad:
		{
			t *= 2.0f;
			if (t < 1.0f)
			{
				t = 0.5f * t * t;
			}
			else
			{
				t -= 1.0f;
				t = -0.5f * ((t * (t - 2.0f)) - 1.0f);
			}
		}
		break;
		default:
		{
			ASSERT(false, "Animation: ease type not supported");
		}
			break;
		}

		return t;
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
				float t = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time, mPositionKeys[i].easeType);
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
				float t = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time, mRotationKeys[i].easeType);
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
				float t = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time, mScaleKeys[i].easeType);
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
