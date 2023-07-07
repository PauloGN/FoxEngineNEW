#pragma once
#include "FoxEngine/Inc/FoxEngine.h"

using namespace FoxEngine::Graphics;

class GameState : public FoxEngine::AppState
{
public:

	virtual ~GameState() = default;
	void Initialize() override;
	void DebugUI() override;
	void Render() override;
	void Terminate() override;
	void Update(float deltaTime) override;

private:
	//Controllers
	void EngineCameraController(float deltaTime);

	//Main Camera and objects
	Camera mCamera;
	RenderObject mRenderObject; //Gets a mesh and apply texture to it
	RenderObject mBasktBall;
	SimpleEffect mSimpleEffect;

	//Render Target
	RenderTarget mRenderTarget;
	Camera mRenderTargetCamera;
	SimpleEffect mSimpleEffectRenderTarget;

};


class SecondGameState: public GameState
{


	void Initialize() override{}
	void DebugUI() override{}
	void Render() override{}
	void Terminate() override{}
	void Update(float deltaTime) override
	{

	auto input = FoxEngine::Input::InputSystem::Get();

	if (input->IsKeyPressed(FoxEngine::Input::KeyCode::UP))
	{
		FoxEngine::MainApp().ChangeState("SecondGameState");
	}
	else if (input->IsKeyPressed(FoxEngine::Input::KeyCode::DOWN))
	{
		FoxEngine::MainApp().ChangeState("GameState");
	}

	}

};