#include "EditTemplateState.h"
#include "Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"
#include "CustomFactory.h"

using namespace FoxEngine;
using namespace FoxEngine::Colors;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void EditTemplateState::Initialize()
{
	GameObjectFactory::SetCustomMake(CustomFactory::CustomComponentMake);
	GameWorld::SetCustomServiceMake(CustomFactory::CustomServiceMake);

	mGameworld.LoadLevel("../../Assets/Templates/Levels/test_Level.json");

	PhysicsService* ps = mGameworld.GetService<PhysicsService>();
	if (ps != nullptr)
	{
		ps->SetEnabled(false);
	}
}
void EditTemplateState::Terminate()
{
	mGameworld.Terminate();
}
void EditTemplateState::Update(float deltaTime)
{
	mGameworld.Update(deltaTime);

	SwapCamera();
}
void EditTemplateState::Render()
{
	mGameworld.Render();
}
void EditTemplateState::DebugUI()
{
	ImGui::Begin("Edit Template State", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameworld.EditorUI();

	if (ImGui::Button("Save: Edit Template"))
	{
		GameObject* go = mGameworld.GetGameObject(GameWorld::GetEditObject());
		mGameworld.SaveTemplate(go->GetTemplatePath(), go->GetHandle());
	}
	ImGui::SameLine();
	if (ImGui::Button("Reload: Edit Template"))
	{
		MainApp().ChangeState("EditTemplateState");
	}
	ImGui::SameLine();
	if (ImGui::Button("Exit: Edit Template"))
	{
		GameWorld::SetEditObject("");
		MainApp().ChangeState("EditorState");
	}
	ImGui::End();
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