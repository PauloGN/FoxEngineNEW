#pragma once

#include "Colors.h"

namespace FoxEngine::Graphics
{
	struct DirectionalLight
	{
		FoxEngine::Color ambient = FoxEngine::Colors::White;
		FoxEngine::Color diffuse = FoxEngine::Colors::White;
		FoxEngine::Color specular = FoxEngine::Colors::White;
		FoxEngine::FoxMath::Vector3 direction = FoxMath::Vector3::ZAxis;
		float padding = 0.0f;
	};
}