#include "EditorState.h"

#include "CustomFactory.h"
#include "Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Colors;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void EditorState::Initialize()
{
	GameObjectFactory::SetCustomMake(CustomFactory::CustomComponentMake);
	GameWorld::SetCustomServiceMake(CustomFactory::CustomServiceMake);

	mGameworld.LoadLevel("../../Assets/Templates/Levels/GamePlayLevel.json");
	PhysicsService* ps = mGameworld.GetService<PhysicsService>();
	if (ps != nullptr)
	{
		ps->SetEnabled(false);
	}
}
void EditorState::Terminate()
{
	mGameworld.Terminate();
}
void EditorState::Update(float deltaTime)
{
	mGameworld.Update(deltaTime);

	ChangeScreenColor();
	SwapCamera();
}
void EditorState::Render()
{
	mGameworld.Render();
}
void EditorState::DebugUI()
{
	ImGui::Begin("Editor State", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameworld.EditorUI();

	if (ImGui::Button("Save : Editor State"))
	{
		mGameworld.SaveLevel(mGameworld.GetLevelFile());
	}
	ImGui::SameLine();
	if (ImGui::Button("Exit : Editor State"))
	{
		MainApp().ChangeState("GameState");
	}

	ImGui::End();
}
void EditorState::SwapCamera()
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
void EditorState::ChangeScreenColor()
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