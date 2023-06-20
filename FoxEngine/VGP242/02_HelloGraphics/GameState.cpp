#include "GameState.h"
#include "Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"


using namespace FoxEngine;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void GameState::Initialize() 
{
}
void GameState::Terminate() 
{
}
void GameState::Update(float deltaTime) 
{
	ChangeScreenColor();
}
void GameState::Render() 
{
}
void GameState::DebugUI()
{
}

void GameState::ChangeScreenColor()
{

	auto I = InputSystem::Get();
	auto GS = Graphics::GraphicsSystem::Get();

	if (I->IsKeyPressed(KeyCode::UP))
	{
		GS->SetClearColor(Colors::Red);
	}
	if (I->IsKeyPressed(KeyCode::DOWN))
	{
		GS->SetClearColor(Colors::Black);
	}
}