#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "SoundBankComponent.h"
#include "SoundEffectComponent.h"
#include "TransformComponent.h"
#include "AnimatorComponent.h"

using namespace FoxEngine;
namespace rj = rapidjson;

namespace 
{
	CustomMake TryMake;

	//RegisterComponentFactories
	std::unordered_map<std::string, std::function<void(rj::Value&, GameObject&)>> componentFactories;

	void RegisterComponentFactory(const std::string& componentName, std::function<void(rj::Value&, GameObject&)> factoryFunction)
	{
		componentFactories[componentName] = factoryFunction;
	}
}

void GameObjectFactory::InitializeComponentFactories()
{
	RegisterComponentFactories();
}

void GameObjectFactory::SetCustomMake(CustomMake customMake)
{
	TryMake = customMake;
}

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

		if (TryMake(componentName, component.value, gameObject))
		{
			// Custom and handled in the project side
		}
		else if (componentFactories.find(componentName) != componentFactories.end())
		{
			componentFactories[componentName](component.value, gameObject);
		}
		else
		{
			ASSERT(false, "GameObject: %s was not defined check ....RegisterComponentFactories()", componentName);
		}
	}

	//for (auto& component : components)
	//{
	//	const char* componentName = component.name.GetString();

	//	if(TryMake(componentName, component.value, gameObject))
	//	{
	//		//custom and we handle on the project
	//	}
	//	else if	(strcmp(componentName, "TransformComponent") == 0)
	//	{
	//		TransformComponent* transformComponent = gameObject.AddComponent<TransformComponent>();
	//		transformComponent->Deserialize(component.value);
	//	}
	//	else if (strcmp(componentName, "CameraComponent") == 0)
	//	{
	//		CameraComponent* cameraComponent = gameObject.AddComponent<CameraComponent>();
	//		cameraComponent->Deserialize(component.value);
	//	}
	//	else if (strcmp(componentName, "FPSCameraComponent") == 0)
	//	{
	//		FPSCameraComponent* fpsCameraComponent = gameObject.AddComponent<FPSCameraComponent>();
	//		fpsCameraComponent->Deserialize(component.value);
	//	}
	//	else if (strcmp(componentName, "ModelComponent") == 0)
	//	{
	//		ModelComponent* modelComponent = gameObject.AddComponent<ModelComponent>();
	//		modelComponent->Deserialize(component.value);
	//	}
	//	else if (strcmp(componentName, "MeshComponent") == 0)
	//	{
	//		MeshComponent* meshComponent = gameObject.AddComponent<MeshComponent>();
	//		meshComponent->Deserialize(component.value);
	//	}
	//	else if (strcmp(componentName, "RigidBodyComponent") == 0)
	//	{
	//		RigidBodyComponent* rigidBodyComponent = gameObject.AddComponent<RigidBodyComponent>();
	//		rigidBodyComponent->Deserialize(component.value);
	//	}
	//	else if (strcmp(componentName, "ColliderComponent") == 0)
	//	{
	//		ColliderComponent* colliderComponent = gameObject.AddComponent<ColliderComponent>();
	//		colliderComponent->Deserialize(component.value);
	//	}
	//	else
	//	{
	//		ASSERT(false, "GameObject: %s was not defined", componentName);
	//	}
	//}
}

void GameObjectFactory::RegisterComponentFactories()
{
	RegisterComponentFactory("TransformComponent", [](rj::Value& value, GameObject& gameObject) {
		TransformComponent* transformComponent = gameObject.AddComponent<TransformComponent>();
		transformComponent->Deserialize(value);
		});

	RegisterComponentFactory("CameraComponent", [](rj::Value& value, GameObject& gameObject) {
		CameraComponent* cameraComponent = gameObject.AddComponent<CameraComponent>();
		cameraComponent->Deserialize(value);
		});

	RegisterComponentFactory("FPSCameraComponent", [](rj::Value& value, GameObject& gameObject) {
		FPSCameraComponent* fpsCameraComponent = gameObject.AddComponent<FPSCameraComponent>();
		fpsCameraComponent->Deserialize(value);
		});

	RegisterComponentFactory("ModelComponent", [](rj::Value& value, GameObject& gameObject) {
		ModelComponent* modelComponent = gameObject.AddComponent<ModelComponent>();
		modelComponent->Deserialize(value);
		});

	RegisterComponentFactory("MeshComponent", [](rj::Value& value, GameObject& gameObject) {
		MeshComponent* meshComponent = gameObject.AddComponent<MeshComponent>();
		meshComponent->Deserialize(value);
		});

	RegisterComponentFactory("RigidBodyComponent", [](rj::Value& value, GameObject& gameObject) {
		RigidBodyComponent* rigidBodyComponent = gameObject.AddComponent<RigidBodyComponent>();
		rigidBodyComponent->Deserialize(value);
		});

	RegisterComponentFactory("ColliderComponent", [](rj::Value& value, GameObject& gameObject) {
		ColliderComponent* colliderComponent = gameObject.AddComponent<ColliderComponent>();
		colliderComponent->Deserialize(value);
		});

	RegisterComponentFactory("SoundEffectComponent", [](rj::Value& value, GameObject& gameObject) {
		SoundEffectComponent* soundEffectComponent = gameObject.AddComponent<SoundEffectComponent>();
		soundEffectComponent->Deserialize(value);
		});

	RegisterComponentFactory("SoundBankComponent", [](rj::Value& value, GameObject& gameObject) {
		SoundBankComponent* soundBankComponent = gameObject.AddComponent<SoundBankComponent>();
		soundBankComponent->Deserialize(value);
		});

	RegisterComponentFactory("AnimatorComponent", [](rj::Value& value, GameObject& gameObject) {
		AnimatorComponent* animatorComponent = gameObject.AddComponent<AnimatorComponent>();
		animatorComponent->Deserialize(value);
		});

	// Add new components as needed
}
