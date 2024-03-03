#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"
#include "CameraService.h"
#include "PhysicsService.h"
#include "UpdateService.h"
#include "RenderService.h"

//components
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

using namespace FoxEngine;

namespace 
{
	CustomService TryServiceMake;
	std::string sEditTemplateName = "";
}

void GameWorld::SetCustomServiceMake(CustomService customService)
{
	TryServiceMake = customService;
}

void GameWorld::SetEditObject(const std::string& objectName)
{
	sEditTemplateName = objectName;
}

void FoxEngine::GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld: is already initialized");

	for (auto& service : mServices)
	{
		service->Initialize();
	}

	GameObjectFactory::InitializeComponentFactories();

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);
	mInitialized = true;
}

void FoxEngine::GameWorld::Terminate()
{
	ASSERT(!mUpdating, "GameWorld: cant terminate while updating");

	if (!mInitialized)
	{
		return;
	}

	for (auto& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->Terminate();
			slot.gameObject.reset();
		}
	}

	for (auto& service : mServices)
	{
		service->Terminate();
		service.reset();
	}

	if(hasSkySphere)
	{
		mSkySphere.Terminate();
		mSimpleEffect.Terminate();
	}

	mServices.clear();
	mInitialized = false;
}

void FoxEngine::GameWorld::Update(float deltaTime)
{
	for (auto& service : mServices)
	{
		service->Update(deltaTime);
	}

	if(hasSkySphere)
	{
		mSkySphere.transform.vrotation.y += Constants::HalfPi * deltaTime * mSkyRotationRate;
	}
}

void FoxEngine::GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
	}

	if(hasSkySphere)
	{
		mSimpleEffect.Begin();
			mSimpleEffect.Render(mSkySphere);
		mSimpleEffect.End();
	}
}

void FoxEngine::GameWorld::DebugUI()
{
	for (auto& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->DebugUI();
		}
	}

	for (auto& service : mServices)
	{
		service->DebugUI();
	}
}

void GameWorld::EditorUI()
{
	for (auto& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->EditorUI();
		}
	}

	//for (auto& service : mServices)
	//{
	//	service->DebugUI();
	//}
}

GameObject* FoxEngine::GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
	ASSERT(mInitialized, "GameWorld: is not initialized");

	if (mFreeSlots.empty())
	{
		ASSERT(false, "GameWorld: is not initialized");
		return nullptr;
	}
	//Get a free slot
	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	//allocate and construct GameOBJ
	Slot& slot = mGameObjectSlots[freeSlot];
	std::unique_ptr<GameObject>& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	//Deserialize game obj and add components
	GameObjectFactory::Make(templateFile, *newObject);

	//set world, handle and initialize
	newObject->mWorld = this;
	newObject->mHandle.mIndex = freeSlot;
	newObject->mHandle.mGeneration = slot.generation;
	newObject->Initialize();

	return newObject.get();
}

void GameWorld::CreateSkySphere(const std::filesystem::path& templateFile, const float radius, const float skyRotationSpeed)
{
	Graphics::MeshPX sphere = Graphics::MeshBuilder::CreateSkySpherePX(256, 128, radius);
	mSkySphere.meshBuffer.Initialize(sphere);
	mSkySphere.diffuseMapId = Graphics::TextureManager::Get()->LoadTexture(templateFile);

	hasSkySphere = true;

	mSimpleEffect.Initialize();
	auto& camera = GetService<CameraService>()->GetMain();
	mSimpleEffect.SetCamera(camera);
	mSkyRotationRate = skyRotationSpeed;
}

GameObject* FoxEngine::GameWorld::GetGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	{
		return nullptr;
	}

	return mGameObjectSlots[handle.mIndex].gameObject.get();
}

void FoxEngine::GameWorld::DestroyObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	{
		return;
	}

	Slot& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++;
	mToBeDestroyed.push_back(handle.mIndex);
	ProcessDestroyList();
}

void GameWorld::LoadLevel(const std::filesystem::path& levelFile)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameWorld: failed to load level %s", levelFile.u8string().c_str());
	//reference to the loaded level
	mLevelFile = levelFile;

	char readBuffer[65536];
	rapidjson::FileReadStream readStream (file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rapidjson::Document doc;
	doc.ParseStream(readStream);

	auto services = doc["Services"].GetObj();
	for (auto& service : services)
	{
		const char* ServiceName = service.name.GetString();

		if(TryServiceMake(ServiceName, service.value, *this))
		{
			//Custom service, project level

		}
		else if(strcmp(ServiceName, "CameraService") == 0)
		{
			CameraService* cameraService = AddService<CameraService>();
			cameraService->Deserialize(service.value);
		}
		else if(strcmp(ServiceName, "UpdateService") == 0)
		{
			UpdateService* updateService = AddService<UpdateService>();
			updateService->Deserialize(service.value);
		}
		else if (strcmp(ServiceName, "RenderService") == 0)
		{
			RenderService* renderService = AddService<RenderService>();
			renderService->Deserialize(service.value);
		}
		else if (strcmp(ServiceName, "PhysicsService") == 0)
		{
			PhysicsService* physicsService = AddService<PhysicsService>();
			physicsService->Deserialize(service.value);
		}
		else
		{
			ASSERT(false, "GameWorld: service %s not defined", ServiceName);
		}
	}

	uint32_t capacity = static_cast<int32_t>(doc["Capacity"].GetInt());
	Initialize(capacity);

	//OBJECTS
	auto gameObjects = doc["GameObjects"].GetObj();
	for(auto& gameObject : gameObjects)
	{
		std::string name = gameObject.name.GetString();
		if (!sEditTemplateName.empty() && sEditTemplateName != name)
		{
			continue;
		}
		const char* templateFile = gameObject.value["Template"].GetString();
		GameObject* obj = CreateGameObject(templateFile);
		
		if(obj != nullptr)
		{
			obj->SetName(name);

			if(sEditTemplateName.empty() && gameObject.value.HasMember("Position"))
			{
				const auto& pos = gameObject.value["Position"].GetArray();
				const float x = pos[0].GetFloat();
				const float y = pos[1].GetFloat();
				const float z = pos[2].GetFloat();

				TransformComponent* transformComponent = obj->GetComponent<TransformComponent>();
				transformComponent->position = Vector3(x, y, z);

				RigidBodyComponent* rigidBodyComponent = obj->GetComponent<RigidBodyComponent>();
				if(rigidBodyComponent != nullptr)
				{
					rigidBodyComponent->SetPosition(transformComponent->position);
				}
			}
		}
	}
}

void GameWorld::SaveLevel(const std::filesystem::path& levelFile)
{
}

void GameWorld::SaveTemplate(const std::filesystem::path& templateFile, const GameObjectHandle& handle)
{
	GameObject* go = GetGameObject(handle);
	if(go != nullptr)
	{
		rapidjson::Document doc;
		go->Serialize(doc);

		FILE* file = nullptr;
		auto err = fopen_s(&file, templateFile.u8string().c_str(), "w");
		ASSERT(err == 0 && file != nullptr, "GameWorld: save template file failed %s", templateFile.u8string().c_str());

		char writeBuffer[65536];
		rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));
		rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
		doc.Accept(writer);
		fclose(file);
	}
}

void GameWorld::SetSkySphereRenderCamera(Graphics::Camera& camera)
{
	if(hasSkySphere)
	{
		mSimpleEffect.SetCamera(camera);
	}
}

bool FoxEngine::GameWorld::IsValid(const GameObjectHandle& handle)
{
	if (handle.mIndex < 0 || handle.mIndex >= mGameObjectSlots.size())
	{
		return false;
	}

	if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
	{
		return false;
	}

	return true;
}

void FoxEngine::GameWorld::ProcessDestroyList()
{
	ASSERT(!mUpdating, "GameWorld: cant destroy while updating");

	for (uint32_t index : mToBeDestroyed)
	{
		Slot& slot = mGameObjectSlots[index];

		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld: object is still alive");

		gameObject->Terminate();
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}

	mToBeDestroyed.clear();
}