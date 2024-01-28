#include "GameState.h"
#include "Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"

using namespace FoxEngine;
using namespace FoxEngine::Colors;
using namespace FoxEngine::Input;
using namespace FoxEngine::Graphics;

void GameState::Initialize() 
{
	CameraComponent* cameraComponent = mCameraGameObject01.AddComponent<CameraComponent>();
	cameraComponent->GetCamera().SetPosition({0.0f, 1.5f,-4.0f});
	cameraComponent->GetCamera().SetLookAt({0.0f, 0.0f, 0.0f});
	mCameraService.Register(cameraComponent);

	cameraComponent = mCameraGameObject02.AddComponent<CameraComponent>();
	cameraComponent->GetCamera().SetPosition({ 0.0f, 1.5f,-4.0f });
	cameraComponent->GetCamera().SetLookAt({ 0.0f, 0.0f, 0.0f });
	mCameraService.Register(cameraComponent);

	GameObjectFactory::Make("../../Assets/Templates/test_objects.json", mGameObject);
	//mGameObject.AddComponent<TransformComponent>();
	mGameObject.Initialize();
}
void GameState::Terminate() 
{
	mGameObject.Terminate();
}
void GameState::Update(float deltaTime) 
{
	CameraComponent* cameraComponent = mCameraGameObject01.GetComponent<CameraComponent>();
	Camera& camera = cameraComponent->GetCamera();

	ChangeScreenColor();
	EngineCameraController(deltaTime, camera);
}
void GameState::Render() 
{
}
void GameState::DebugUI()
{
	mGameObject.DebugUI();
	SimpleDraw::Render(mCameraService.GetMain());
}

void GameState::EngineCameraController(float deltaTime, Camera& camera)
{
	const InputSystem* input = Input::InputSystem::Get();
	const int moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10 : 1;
	const float displacement = 0.01f * moveSpeed;

	if (input->IsKeyPressed(KeyCode::ONE))
	{
		mCameraService.SetMainCamera(0);
	}
	else if (input->IsKeyPressed(KeyCode::TWO))
	{
		mCameraService.SetMainCamera(1);
	}

	//Foward and Backward
	if (input->IsKeyDown(KeyCode::W))
	{
		camera.Walk(displacement);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		camera.Walk(-displacement);
	}
	//Right and Left
	if (input->IsKeyDown(KeyCode::D))
	{
		camera.Strafe(displacement);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		camera.Strafe(-displacement);
	}

	//Rotation
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		const int turnSpeedMultiplyer = input->IsKeyDown(KeyCode::LSHIFT) ? 2 : 1;

		const float x = input->GetMouseMoveX() * deltaTime * turnSpeedMultiplyer;
		const float y = input->GetMouseMoveY() * deltaTime * turnSpeedMultiplyer;

		camera.Yaw(x);
		camera.Pitch(y);
	}

	//UP and Down

	if (input->IsKeyDown(KeyCode::Q))
	{
		camera.Rise(-displacement);
	}
	else if (input->IsKeyDown(KeyCode::E))
	{
		camera.Rise(displacement);
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