#pragma once

#include "Common.h"

namespace FoxEngine::Graphics
{
	enum class EaseType
	{
		Linear
	};

	template <class T>
	struct Keyframe
	{
		Keyframe() = default;
		Keyframe(const T& k, float t, EaseType e = EaseType::Linear)
			: key(k)
			, time(t)
			, easeType(e)
		{}

		T key = T();
		float time = 0.0f;
		EaseType easeType = EaseType::Linear;
	};

	template <class T>
	using Keyframes = std::vector<Keyframe<T>>;

	using PositionKeys = Keyframes<FoxMath::Vector3>;
	using RotationKeys = Keyframes<FoxMath::Quaternion>;
	using ScaleKeys = Keyframes<FoxMath::Vector3>;
	//using ColorKeys = Keyframes<Color>;
}