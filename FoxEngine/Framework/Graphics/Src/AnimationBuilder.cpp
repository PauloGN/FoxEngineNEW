#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float t, EaseType easeType)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= t, "AnimationBuilder: cannot add keyframe back ");
		keyframes.emplace_back(value, t, easeType);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const FoxMath::Vector3& position, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mPositionKeys ,position, time, easeType);
	mWorkingCopy.mDuration = FoxMath::Max(mWorkingCopy.mDuration, time);

	return *this;
}

AnimationBuilder& FoxEngine::Graphics::AnimationBuilder::AddRotationKey(const FoxMath::Quaternion& rotation, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mRotationKeys, rotation, time, easeType);
	mWorkingCopy.mDuration = FoxMath::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& FoxEngine::Graphics::AnimationBuilder::AddScaleKey(const FoxMath::Vector3& scale, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mScaleKeys, scale, time, easeType);
	mWorkingCopy.mDuration = FoxMath::Max(mWorkingCopy.mDuration, time);
	return *this;
}

Animation FoxEngine::Graphics::AnimationBuilder::Build()
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty() || !mWorkingCopy.mRotationKeys.empty() || !mWorkingCopy.mScaleKeys.empty(), "AnimationBuilder: no animation keys available");

	return std::move(mWorkingCopy);
}