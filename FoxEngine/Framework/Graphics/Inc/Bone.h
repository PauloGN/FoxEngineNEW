#pragma once

#include "Common.h"

namespace FoxEngine::Graphics
{
	struct Bone
	{
		std::string name;
		int  index = -1;

		Bone* parent = nullptr;
		int parentIndex = -1;

		std::vector<Bone*> children;
		std::vector<int> childrenIndices;

		FoxMath::Matrix4 toParentTransform;
		FoxMath::Matrix4 offsetTransfrom;
	};
}