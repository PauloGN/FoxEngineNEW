#include "Precompiled.h"
#include "TransformComponent.h"

using namespace FoxEngine;

void FoxEngine::TransformComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(GetMatrix4());
}
