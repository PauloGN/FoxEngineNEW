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
	mCamera.SetPosition({0.0f, 1.5f,-4.0f});
	mCamera.SetLookAt({0.0f, 0.0f, 0.0f});

	GameObjectFactory::InitializeComponentFactories();
	GameObjectFactory::SetCustomMake(CustomComponentMake);
	GameWorld::SetCustomServiceMake(CustomServiceMake);

	GameObjectFactory::Make("../../Assets/Templates/test_objects_Empty.json", mGameObject);
	//mGameObject.AddComponent<TransformComponent>();
	mGameObject.Initialize();
}
void GameState::Terminate() 
{
	mGameObject.Terminate();
}
void GameState::Update(float deltaTime) 
{
	ChangeScreenColor();
	EngineCameraController(deltaTime);
}
void GameState::Render() 
{
}
void GameState::DebugUI()
{
	mGameObject.DebugUI();
	SimpleDraw::Render(mCamera);
}

void GameState::EngineCameraController(float deltaTime)
{
	const InputSystem* input = Input::InputSystem::Get();
	const int moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10 : 1;
	const float displacement = 0.01f * moveSpeed;

	//Foward and Backward
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(displacement);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-displacement);
	}
	//Right and Left
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(displacement);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-displacement);
	}

	//Rotation
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		const int turnSpeedMultiplyer = input->IsKeyDown(KeyCode::LSHIFT) ? 2 : 1;

		const float x = input->GetMouseMoveX() * deltaTime * turnSpeedMultiplyer;
		const float y = input->GetMouseMoveY() * deltaTime * turnSpeedMultiplyer;

		mCamera.Yaw(x);
		mCamera.Pitch(y);
	}

	//UP and Down

	if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-displacement);
	}
	else if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(displacement);
	}
}
void GameState::ChangeScreenColor()
{
	const auto I = InputSystem::Get();
	const auto GS = Graphics::GraphicsSystem::Get();

	if (I->IsKeyPressed(KeyCode::UP))
	{
		GS->SetClearColor(Colors::Red);
	}
	if (I->IsKeyPressed(KeyCode::DOWN))
	{
		GS->SetClearColor(Colors::Black);
	}
}