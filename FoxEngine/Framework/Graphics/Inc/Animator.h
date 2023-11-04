#pragma once

#include "ModelManager.h"
#include "Bone.h"

namespace FoxEngine::Graphics
{
	class Animator
	{
	public:
		void Initialize(ModelId is);
		void PlayAnimation(int clipIndex, bool looping);
		void Update(float deltaTime);

		bool IsFinished() const;
		size_t GetAnimationsCount() const;
		FoxMath::Matrix4 GetParentTransform(const Bone* bone) const;

	private:
		ModelId mModelId = 0;
		float mClipIndex = -1;
		float mAnimationTick = 0.0f;
		bool mIsLooping = false;
	};
}