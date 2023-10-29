#pragma once

#include "Keyframe.h"
#include "Transform.h"

namespace FoxEngine::Graphics
{
	class Animation
	{
	public:
		Transform GetTransform(float time) const;
		float GetDuration() const;

	private:

		FoxMath::Vector3 GetPosition(float time) const;
		FoxMath::Quaternion GetRotation(float time) const;
		FoxMath::Vector3 GetScale(float time) const;

		friend class AnimationBuilder;
		friend class AnimationIO;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration;
	};
}