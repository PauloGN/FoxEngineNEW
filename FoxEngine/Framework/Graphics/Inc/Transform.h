#pragma once

#include "Common.h"

namespace FoxEngine::Graphics
{
	struct Transform
	{
		FoxMath::Vector3 position = FoxMath::Vector3::Zero;
		FoxMath::Quaternion rotation = FoxMath::Quaternion::Identity;
		FoxMath::Vector3 scale = FoxMath::Vector3::One;

		FoxMath::Matrix4 GetMatrix4() const
		{
			return { FoxMath::Matrix4::Translation(position)*
				 FoxMath::Matrix4::Scaling(scale)*
				FoxMath::Matrix4::MatrixRotationQuaternion(rotation)
			};
		}
	};
}
