#pragma once

#include "ModelManager.h"
#include "Animator.h"

namespace FoxEngine::Graphics::AnimationUtil
{
	using BoneTransforms = std::vector<FoxMath::Matrix4>;

	void ComputeBoneTransform(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator = nullptr);
	void ApplyBoneOffsets(ModelId modelId, BoneTransforms& boneTransforms);
	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
}