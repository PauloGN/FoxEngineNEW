#pragma once
#include <FoxEngine/Inc/TypeIds.h>

enum class CustomComponentID
{
	NewComponent = static_cast<uint32_t>(FoxEngine::ComponentId::Count),
};

enum class CustomServiceID
{
	NewService = static_cast<uint32_t>(FoxEngine::ServiceId::Count),
};