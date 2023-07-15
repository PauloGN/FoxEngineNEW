#pragma once

#include "Colors.h"

namespace FoxEngine::Graphics
{
	struct DirectionalLight
	{
		FoxMath::Vector3 direction = FoxMath::Vector3::ZAxis;
		float padding = 0.0f;
		Color ambient = Colors::White;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
	};
}