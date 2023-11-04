#include "Precompiled.h"
#include "Animator.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;

void FoxEngine::Graphics::Animator::Initialize(ModelId id)
{
	mModelId = id;
	mIsLooping = false;
	mAnimationTick = 0.0f;
	mClipIndex = -1;
}

void FoxEngine::Graphics::Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mAnimationTick = 0.0f;
}

void FoxEngine::Graphics::Animator::Update(float deltaTime)
{
	if (mClipIndex < 0)
	{
		return;
	}

	auto model = ModelManager::Get()->GetModel(mModelId);
	const auto& animClip = model->animationClips[mClipIndex];
	mAnimationTick += animClip.ticksPerSecond * deltaTime; // note: ticks to frames
	if (mAnimationTick > animClip.tickDuration)
	{
		if (mIsLooping)
		{
			while (mAnimationTick >= animClip.tickDuration)
			{
				mAnimationTick -= animClip.ticksPerSecond;
			}
		}
		else
		{
			mAnimationTick = animClip.tickDuration;
		}
	}
}

bool FoxEngine::Graphics::Animator::IsFinished() const
{
	if (mIsLooping || mClipIndex < 0)
	{
		return false;
	}

	auto model = ModelManager::Get()->GetModel(mModelId);
	const auto& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.tickDuration;
}

size_t FoxEngine::Graphics::Animator::GetAnimationsCount() const
{
	auto model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

FoxMath::Matrix4 FoxEngine::Graphics::Animator::GetParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
	{
		return bone->toParentTransform;
	}

	auto model = ModelManager::Get()->GetModel(mModelId);
	const auto& animClip = model->animationClips[mClipIndex];
	const auto& animation = animClip.boneAnimations[bone->index];
	if (animation == nullptr)
	{
		return FoxMath::Matrix4::Identity;
	}

	Transform transform = animation->GetTransform(mAnimationTick);
	return transform.GetMatrix4();
}