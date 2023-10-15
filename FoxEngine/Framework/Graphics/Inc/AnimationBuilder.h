#pragma once

#include "Animation.h"

namespace FoxEngine::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& AddPositionKey(const FoxMath::Vector3& position, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddRotationKey(const FoxMath::Quaternion& rotation, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddScaleKey(const FoxMath::Vector3& scale, float time, EaseType easeType = EaseType::Linear);


		[[nodiscard]]Animation Build();

	private:

		Animation mWorkingCopy;

	};
}