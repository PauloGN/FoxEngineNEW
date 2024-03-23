#include "CustomFactory.h"
#include "TypeIds.h"

using namespace FoxEngine;

bool CustomFactory::CustomComponentMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
{
	if (strcmp(componentName, "NewComponent") == 0)
	{
		// NewComponent* newComponent = gameObject.AddComponent<NewComponent>();
		//newComponent->Deserialize(value);
		return true;
	}
	return false;
}

bool CustomFactory::CustomServiceMake(const char* componentName, const rapidjson::Value& value, GameWorld& gameWorld)
{
	if (strcmp(componentName, "NewService") == 0)
	{
		// NewService* newService = gameWorld.AddComponent<NewService>();
		//newService->Deserialize(value);
		return true;
	}
	return false;
}