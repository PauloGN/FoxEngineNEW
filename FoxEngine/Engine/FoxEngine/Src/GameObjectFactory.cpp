#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "TransformComponent.h"

using namespace FoxEngine;

namespace rj = rapidjson;

void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, templatePath.u8string().c_str(), "r");

	ASSERT(err == 0, "GameObjectFactory: failed to open file %s", templatePath.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream (file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rj::Document doc;
	doc.ParseStream(readStream);

	auto components = doc["Components"].GetObj();

	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();

		if (strcmp(componentName, "TransformComponent") == 0)
		{
			TransformComponent* transformComponent = gameObject.AddComponent<TransformComponent>();
			transformComponent->Deserialize(component.value);
		}
		else
		{
			ASSERT(false, "GameObject: %s was not defined", componentName);
		}
	}

}