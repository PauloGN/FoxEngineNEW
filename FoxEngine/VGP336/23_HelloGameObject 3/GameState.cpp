#include "GameState.h"
#include "Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Colors;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

namespace
{
	bool CustomComponentMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		if (strcmp(componentName, "NewComponent") == 0)
		{
			// NewComponent* newComponent = gameObject.AddComponent<NewComponent>();
			//newComponent->Deserialize(value);
			return true;
		}
		return false;
	}

	bool CustomServiceMake(const char* componentName, const rapidjson::Value& value, GameWorld& gameWorld)
	{
		if (strcmp(componentName, "NewService") == 0)
		{
			// NewService* newService = gameWorld.AddComponent<NewService>();
			//newService->Deserialize(value);
			return true;
		}
		return false;
	}
}

void GameState::Initialize() 
{
	//pre-load framework components when game world initializes
	mGameWorld.AddService<PhysicsService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<UpdateService>();
	mGameWorld.Initialize(1000);

	//set custom components
	GameObjectFactory::SetCustomMake(CustomComponentMake);
	GameWorld::SetCustomServiceMake(CustomServiceMake);

	auto gameObject = mGameWorld.CreateGameObject("../../Assets/Templates/test_objects_Empty.json");
	mGameObjHandle = gameObject->GetHandle();

	auto gameObject2 = mGameWorld.CreateGameObject("../../Assets/Templates/test_PW5.json");
	mGameObjHandle2 = gameObject2->GetHandle();

	mGameWorld.CreateGameObject("../../Assets/Templates/test_fps_Camera.json");
}
void GameState::Terminate() 
{
	mGameWorld.Terminate();
}
void GameState::Update(float deltaTime) 
{
	ChangeScreenColor();

	mGameWorld.Update(deltaTime);
}
void GameState::Render() 
{
	mGameWorld.Render();
}
void GameState::DebugUI()
{
	auto gameObj = mGameWorld.GetGameObject(mGameObjHandle);
	auto gameObj2 = mGameWorld.GetGameObject(mGameObjHandle2);

	mGameWorld.DebugUI();

	/*if(gameObj != nullptr)
	{
		gameObj->DebugUI();
	}
	if (gameObj2 != nullptr)
	{
		gameObj2->DebugUI();
	}
	SimpleDraw::Render(mCamera);*/
}

void GameState::ChangeScreenColor()
{
	const auto I = InputSystem::Get();
	const auto GS = Graphics::GraphicsSystem::Get();

	if (I->IsKeyPressed(KeyCode::UP))
	{
		GS->SetClearColor(Colors::DarkGray);
	}
	if (I->IsKeyPressed(KeyCode::DOWN))
	{
		GS->SetClearColor(Colors::Black);
		//Destroy game object
		//mGameWorld.DestroyObject(mGameObjHandle2);
		mGameWorld.DestroyObject(mGameObjHandle);
	}
}