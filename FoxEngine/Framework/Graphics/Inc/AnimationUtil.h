#pragma once

#include "ModelManager.h"

namespace FoxEngine::Graphics::AnimationUtil
{
	using BoneTransforms = std::vector<FoxMath::Matrix4>;

	void ComputeBoneTransform(ModelId modelId, BoneTransforms& boneTransforms);
	void ApplyBoneOffsets(ModelId modelId, BoneTransforms& boneTransforms);
	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
}