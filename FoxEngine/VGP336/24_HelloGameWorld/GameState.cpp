#include "GameState.h"
#include "Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Colors;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void GameState::Initialize() 
{
	mGameworld.LoadLevel("../../Assets/Templates/Levels/test_Level.json");
}
void GameState::Terminate() 
{
	mGameworld.Terminate();
}
void GameState::Update(float deltaTime) 
{
	mGameworld.Update(deltaTime);

	ChangeScreenColor();
	//SwapCamera();
}
void GameState::Render() 
{
	mGameworld.Render();
}
void GameState::DebugUI()
{
	mGameworld.DebugUI();
}

void GameState::SwapCamera()
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
void GameState::ChangeScreenColor()
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