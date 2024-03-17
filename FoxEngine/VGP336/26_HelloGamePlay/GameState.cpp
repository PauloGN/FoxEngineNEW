#include "GameState.h"

#include "CustomFactory.h"
#include "Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Colors;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void GameState::Initialize()
{
	GameObjectFactory::SetCustomMake(CustomFactory::CustomComponentMake);
	GameWorld::SetCustomServiceMake(CustomFactory::CustomServiceMake);

	mGameworld.LoadLevel("../../Assets/Templates/Levels/GamePlayLevel.json");
	mGameworld.CreateSkySphere(L"Space03.jpg", 3000);

	MagnetifyComponent* MC = mGameworld.GetGameObject("TPSSpaceShip")->GetComponent<MagnetifyComponent>();
	GameObject* s1 = mGameworld.GetGameObject("S1");
	s1->mHasAttraction = true;
	MC->AddObject(*s1);

	LOAD_MAGNETIFY(mGameworld, "TPSSpaceShip", "../../Assets/Loadfiles/MagnetifyObjList.txt");

	//PhysicsService* ps = mGameworld.GetService<PhysicsService>();
	//if (ps != nullptr)
	//{
	//	ps->SetEnabled(true);
	//}

}
void GameState::Terminate()
{
	mGameworld.Terminate();
}
void GameState::Update(float deltaTime)
{
	mGameworld.Update(deltaTime);

	ChangeScreenColor();
	SwapCamera();
}
void GameState::Render()
{
	mGameworld.Render();
}
void GameState::DebugUI()
{
	ImGui::Begin("GameState", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameworld.DebugUI();

	if (ImGui::Button("Edit: Game World"))
	{
		MainApp().ChangeState("EditorState");
	}

	ImGui::End();
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
	else if (input->IsKeyPressed(KeyCode::THREE))
	{
		cameraService->SetMainCamera(2);
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