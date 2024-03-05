#pragma once
#include <FoxEngine/Inc/FoxEngine.h>

namespace CustomFactory
{
	bool CustomComponentMake(const char* componentName, const rapidjson::Value& value, FoxEngine::GameObject& gameObject);
	bool CustomServiceMake(const char* componentName, const rapidjson::Value& value, FoxEngine::GameWorld& gameWorld);
}
