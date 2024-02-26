#include "EditTemplateState.h"
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
		if(strcmp(componentName, "NewComponent") == 0)
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

void EditTemplateState::Initialize()
{
	GameObjectFactory::SetCustomMake(CustomComponentMake);
	GameWorld::SetCustomServiceMake(CustomServiceMake);

	mGameworld.LoadLevel("../../Assets/Templates/Levels/test_Level.json");
}
void EditTemplateState::Terminate()
{
	mGameworld.Terminate();
}
void EditTemplateState::Update(float deltaTime)
{
	mGameworld.Update(deltaTime);

	ChangeScreenColor();
	SwapCamera();
}
void EditTemplateState::Render()
{
	mGameworld.Render();
}
void EditTemplateState::DebugUI()
{
	mGameworld.DebugUI();
}

void EditTemplateState::SwapCamera()
{
	const InputSystem* input = Input::InputSystem::Get();
	CameraService* cameraService = mGameworld.GetService<CameraService>();

	if (input->IsKeyPressed(KeyCode::ONE))
	{
		cameraService->SetMainCamera(0);

	}
	else if (input->IsKeyPressed(KeyCode::TWO))
	{
		cameraService->SetMainCamera(1);
	}
}
void EditTemplateState::ChangeScreenColor()
{
	const auto I = InputSystem::Get();
	const auto GS = Graphics::GraphicsSystem::Get();

	if (I->IsKeyPressed(KeyCode::UP))
	{
		GS->SetClearColor(Colors::DeepSkyBlue);
	}
	if (I->IsKeyPressed(KeyCode::DOWN))
	{
		GS->SetClearColor(Colors::Black);
	}
}