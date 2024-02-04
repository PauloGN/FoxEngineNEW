#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"
#include "CameraService.h"
#include "UpdateService.h"

//components
#include "TransformComponent.h"

using namespace FoxEngine;

void FoxEngine::GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld: is already initialized");

	for (auto& service : mServices)
	{
		service->Initialize();
	}

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);
	mInitialized = true;
}

void FoxEngine::GameWorld::Terminate()
{
	ASSERT(!mUpdating, "GameWorld: cant terminate while updating");

	if (mInitialized)
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
	}

	mInitialized = false;
}

void FoxEngine::GameWorld::Update(float deltaTime)
{
	for (auto& service : mServices)
	{
		service->Update(deltaTime);
	}
}

void FoxEngine::GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
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

GameObject* FoxEngine::GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
	ASSERT(mInitialized, "GameWorld: is not initialized");

	if (mFreeSlots.empty())
	{
		ASSERT(false, "GameWorld: is not initialized");
		return nullptr;
	}

	const uint32_t freeslot = mFreeSlots.back();
	mFreeSlots.pop_back();

	Slot& slot = mGameObjectSlots[freeslot];
	std::unique_ptr<GameObject>& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	GameObjectFactory::Make(templateFile, *newObject);

	newObject->mWorld = this;
	newObject->mHandle.mIndex = freeslot;
	newObject->mHandle.mGeneration = slot.generation;
	newObject->Initialize();

	return newObject.get();
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
	mTobeDestroyed.push_back(handle.mIndex);
}

void GameWorld::LoadLevel(const std::filesystem::path& levelFile)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameWorld: failed to load level %s", levelFile.u8string().c_str());

	char readBuffer[65536];
	rapidjson::FileReadStream readStream (file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rapidjson::Document doc;
	doc.ParseStream(readStream);

	auto services = doc["Services"].GetObj();
	for (auto& service : services)
	{
		const char* ServiceName = service.name.GetString();
		if(strcmp(ServiceName, "CameraService") == 0)
		{
			CameraService* cameraService = AddService<CameraService>();
			cameraService->Deserialize(service.value);
		}
		else if(strcmp(ServiceName, "UpdateService") == 0)
		{
			UpdateService* updateService = AddService<UpdateService>();
			updateService->Deserialize(service.value);
		}else
		{
			ASSERT(false, "GameWorld: service %s not defined", ServiceName);
		}
	}

	uint32_t capacity = static_cast<int32_t>(doc["Capacity"].GetInt());
	Initialize(capacity);

	auto gameObjects = doc["GameObjects"].GetObj();
	for(auto& gameObject : gameObjects)
	{
		const char* templateFile = gameObject.value["Template"].GetString();
		GameObject* obj = CreateGameObject(templateFile);
		if(obj != nullptr)
		{
			std::string name = gameObject.name.GetString();
			obj->SetName(name);

			if(gameObject.value.HasMember("Position"))
			{
				const auto& pos = gameObject.value["Position"].GetArray();
				const float x = pos[0].GetFloat();
				const float y = pos[1].GetFloat();
				const float z = pos[2].GetFloat();

				TransformComponent* transformComponent = obj->GetComponent<TransformComponent>();
				transformComponent->position = Vector3(x, y, z);
			}
		}
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

	for (uint32_t index : mTobeDestroyed)
	{
		Slot& slot = mGameObjectSlots[index];

		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld: object is still alive");

		gameObject->Terminate();
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}

	mTobeDestroyed.clear();
}

