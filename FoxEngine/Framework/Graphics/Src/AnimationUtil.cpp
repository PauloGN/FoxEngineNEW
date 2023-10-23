#include "Precompiled.h"
#include "AnimationUtil.h"
#include "Colors.h"
#include "SimpleDraw.h"

using namespace FoxEngine;
using namespace FoxEngine::Graphics;
using namespace FoxEngine::Graphics::AnimationUtil;


namespace
{
	void ComputeBoneTransformRecursive(const Bone* bone, BoneTransforms& boneTransforms)
	{
		if (bone != nullptr)
		{
			boneTransforms[bone->index] = bone->toParentTransform;
			if (bone->parent != nullptr)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
			}

			for (auto child : bone->children)
			{
				ComputeBoneTransformRecursive(child, boneTransforms);
			}
		}
	}
}


void AnimationUtil::ComputeBoneTransform(ModelId modelId, BoneTransforms& boneTransforms)
{
	auto model = ModelManager::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		boneTransforms.resize(model->skeleton->bones.size(), FoxMath::Matrix4::Identity);
		ComputeBoneTransformRecursive(model->skeleton->root, boneTransforms);
	}
}

void AnimationUtil::ApplyBoneOffsets(ModelId modelId, BoneTransforms & boneTransforms)
{
	auto model = ModelManager::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransfrom * boneTransforms[bone->index];
		}
	}
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms & boneTransforms)
{
	auto model = ModelManager::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr && bone->children.size() > 0)
			{
				const auto posA = FoxMath::GetTranslation(boneTransforms[bone->index]);
				const auto posB = FoxMath::GetTranslation(boneTransforms[bone->parentIndex]);
				SimpleDraw::AddSphere(10, 10, 0.02f, posB, Colors::Green);
				SimpleDraw::AddLine(posA, posB, Colors::HotPink);
			}
		}
	}
}